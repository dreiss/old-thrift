%%% Copyright (c) 2007- Facebook
%%% Distributed under the Thrift Software License
%%%
%%% See accompanying file LICENSE or visit the Thrift site at:
%%% http://developers.facebook.com/thrift/

-module(thrift_compact_protocol).
-compile(export_all).

-behavior(thrift_protocol).

-include("thrift_constants.hrl").
-include("thrift_protocol.hrl").

-export([new/1,
         read/2,
         write/2,
         flush_transport/1,
         close_transport/1,

         new_protocol_factory/2
        ]).

-record(compact_protocol, {transport,
                           bool_field = undefined,
                           last_field_id_stack = [0]
                         }).

-define(VERSION, 1).
-define(PROTOCOL_ID, 16#82).

-define(CTYPE_BOOLEAN_TRUE,  1).
-define(CTYPE_BOOLEAN_FALSE, 2).
-define(CTYPE_BYTE,          3).
-define(CTYPE_I16,           4).
-define(CTYPE_I32,           5).
-define(CTYPE_I64,           6).
-define(CTYPE_DOUBLE,        7).
-define(CTYPE_BINARY,        8).
-define(CTYPE_LIST,          9).
-define(CTYPE_SET,           16#0A).
-define(CTYPE_MAP,           16#0B).
-define(CTYPE_STRUCT,        16#0C).


new(Transport) ->
    gen_server:start_link(?MODULE, [Transport], []).


flush_transport(Pid) ->
    gen_server:call(Pid, {flush_transport}).

close_transport(Pid) ->
    gen_server:call(Pid, {close_transport}).

write(Pid, Data) ->
    gen_server:call(Pid, {write, Data}).

read(Pid, Spec) ->
    gen_server:call(Pid, {read, Spec}).


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
init([Transport]) ->
    {ok, #compact_protocol{transport = Transport}}.

handle_call({flush_transport}, _From, State) ->
    thrift_transport:flush(State#compact_protocol.transport),
    {reply, ok, State};

handle_call({close_transport}, _From, State) ->
    thrift_transport:close(State#compact_protocol.transport),
    {reply, ok, State};

handle_call({write, Data}, _From, State) ->
    {Reply, NewState} = write(State, Data),
    {reply, Reply, NewState};

handle_call({read, Spec}, _From, State) ->
    case int_read(State, Spec) of
        {ok, NewState, Response} ->
            {reply, {ok, Response}, NewState};
        {ok, NewState} ->
            {reply, ok, NewState};
        Else ->
            {reply, Else, State}
    end.



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


int_write(State, #protocol_message_begin{
        name = Name,
        type = Type, %% :: ?tMessageType_CALL | ?tMessageType_EXCEPTION | ?tMessageType_REPLY
        seqid = Seqid}) ->
    fold_writes(State,
                [<<?PROTOCOL_ID:8, Type:3, ?VERSION:5>>,
                 {varint32, Seqid},
                 {string, Name}]);

int_write(State, message_end) -> {ok, State};

int_write(State, Field = #protocol_field_begin{
       name = _Name,
       type = Type,
       id = Id}) ->
    case Type of
        ?tType_BOOL ->
            {ok, State#compact_protocol{bool_field = Field}};
        _ ->
            int_write(State, {field_begin_internal, Field, no_override})
    end;


int_write(State = #compact_protocol{
        last_field_id_stack = [LastFieldId | FieldRest]
       },
      {field_begin_internal,
       Field = #protocol_field_begin{
         id = FieldId,
         type = FieldType
        }, Override}) ->
    TypeToWrite = case Override of
                      no_override -> compact_type(FieldType);
                      _ -> Override
                  end,
    
    ToWrite =
        if is_integer(LastFieldId) andalso FieldId > LastFieldId andalso FieldId - LastFieldId =< 15 ->
                Delta = FieldId - LastFieldId,
                [<<Delta:4, TypeToWrite:4>>];
           true  ->
                [{byte_direct, TypeToWrite},
                 {i16, FieldId}]
        end,
    {ok, NewState} = fold_writes(State, ToWrite),
    {ok, NewState#compact_protocol{last_field_id_stack = [FieldId | FieldRest]}};


int_write(State, field_stop) ->
    int_write(State, {byte_direct, ?tType_STOP});

int_write(State, field_end) -> ok;

int_write(State, #protocol_map_begin{
       ktype = Ktype,
       vtype = Vtype,
       size = Size}) ->
    case Size of
        0 ->
            int_write(State, {byte_direct, 0});
        _ ->
            CompactKT = compact_type(Ktype),
            CompactVT = compact_type(Vtype),
            fold_writes(State, [{varint32, Size},
                                <<CompactKT:4, CompactVT:4>>])
    end;

int_write(State, map_end) -> {ok, State};

int_write(State, #protocol_list_begin{
        etype = Etype,
        size = Size}) ->
    write_collection_begin(State, Etype, Size);

int_write(State, list_end) -> {ok, State};

int_write(State, #protocol_set_begin{
        etype = Etype,
        size = Size}) ->
    write_collection_begin(State, Etype, Size);

int_write(State, set_end) -> {ok, State};

int_write(State = #compact_protocol{last_field_id_stack = Stack}, #protocol_struct_begin{}) ->
    {ok, State#compact_protocol{last_field_id_stack = [0 | Stack]}};

int_write(State = #compact_protocol{last_field_id_stack = [_ | StackRest]}, struct_end) ->
    {ok, State#compact_protocol{last_field_id_stack = StackRest}};


int_write(State = #compact_protocol{bool_field = undefined},
      {bool, Val}) ->
    int_write(State, case Val of
                     true -> {byte_direct, ?CTYPE_BOOLEAN_TRUE};
                     false -> {byte_direct, ?CTYPE_BOOLEAN_FALSE}
                 end);

int_write(State = #compact_protocol{bool_field = Field},
      {bool, Val}) ->
    BoolType = case Val of
                   true -> {byte_direct, ?CTYPE_BOOLEAN_TRUE};
                   false -> {byte_direct, ?CTYPE_BOOLEAN_FALSE}
               end,
    {ok, State1} = int_write(State, {field_begin_internal, Field, BoolType}),
    {ok, State1#compact_protocol{bool_field = undefined}};


int_write(State, {byte, Byte}) ->
    int_write(State, <<Byte:8/big-signed>>);
int_write(State, {byte_direct, Byte}) ->
    int_write(State, <<Byte:8/big-signed>>);

int_write(State, {i16, I16}) ->
    int_write(State, {varint32, zigzag_encode(int, I16)});

int_write(State, {i32, I32}) ->
    int_write(State, {varint32, zigzag_encode(int, I32)});

int_write(State, {i64, I64}) ->
    int_write(State, {varint64, zigzag_encode(long, I64)});

int_write(State, {double, Double}) ->
    int_write(State, <<Double:64/big-signed-float>>);

int_write(State, {string, Str}) when is_list(Str) ->
    int_write(State, {varint32, length(Str)}),
    int_write(State, list_to_binary(Str));

int_write(State, {string, Bin}) when is_binary(Bin) ->
    int_write(State, {varint32, size(Bin)}),
    int_write(State, Bin);

int_write(State, {varint32, Int}) when is_integer(Int) ->
    write(State, varint_encode(<<Int:32/big-unsigned>>));
int_write(State, {varint64, Int}) when is_integer(Int) ->
    write(State, varint_encode(<<Int:64/big-unsigned>>));

%% Data :: iolist()
int_write(State, Data) ->
    thrift_transport:int_write(State#compact_protocol.transport, Data).

write_collection_begin(State, ElemType, Size) ->
    CType = compact_type(ElemType),
    if Size =< 15 ->
            int_write(State, <<Size:4, CType:4>>);
       true ->
            fold_writes(State, [<<2#1111:4, CType:4>>,
                                {varint32, Size}])
    end.


fold_writes(State, []) ->
    {ok, State};
fold_writes(State, [ToWrite | Rest]) ->
    {ok, NewState} = int_write(State, ToWrite),
    fold_writes(NewState, Rest).


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


int_read(State, message_begin) ->
    {ok, S1, <<?PROTOCOL_ID:8, Type:3, ?VERSION:5>>} = int_read(State, 2),
    {ok, S2, SeqId} = int_read(S1, varint32),
    {ok, S3, Name} = int_read(S1, string),
    {ok, S3, #protocol_message_begin{name = Name,
                                     seqid = SeqId,
                                     type = Type}};

int_read(State, message_end) -> {ok, State};

int_read(State = #compact_protocol{last_field_id_stack = Stack}, struct_begin) ->
    {ok, State#compact_protocol{last_field_id_stack = [0 | Stack]}};
int_read(State = #compact_protocol{last_field_id_stack = [_ | Stack]}, struct_end) ->
    {ok, State#compact_protocol{last_field_id_stack = Stack}};


int_read(State = #compact_protocol{last_field_id_stack = [FIDStackTop | FIDStackRest]},
     field_begin) ->
    case read(State, 1) of
        {ok, S1, <<0:4, ?tType_STOP:4>>} ->
            {ok, S1, #protocol_field_begin{type = ?tType_STOP}};
        Else ->
            case Else of
                {ok, S1, <<0:4, Type:4>>} ->
                    {ok, S2, FieldId} = int_read(S1, i16);
                {ok, S1, <<Delta:4, Type:4>>} ->
                    FieldId = FIDStackTop + Delta,
                    S2 = S1
            end,
            NewBoolVal = case Type of
                             ?CTYPE_BOOLEAN_TRUE -> true;
                             ?CTYPE_BOOLEAN_FALSE -> false;
                             _ -> undefined
                         end,
            S3 = S2#compact_protocol{
                   last_field_id_stack = [FieldId | FIDStackRest],
                   bool_field = NewBoolVal},
            {ok, S3, #protocol_field_begin{type = ttype(Type),
                                           id = FieldId}}
    end;

int_read(State, field_end) -> {ok, State};

int_read(State, map_begin) ->
    {ok, S1, Size} = int_read(State, varint32),
    case Size of
        0 ->
            #protocol_map_begin{size = 0};
        _ ->
            {ok, S2, <<CKType:4, CVType:4>>} = int_read(S1, 1),
            #protocol_map_begin{ktype = ttype(CKType),
                                vtype = ttype(CVType),
                                size = Size}
    end;

int_read(State, map_end) -> ok;

int_read(State, list_begin) ->
    {ok, S1, <<Size:4, Type:4>>} = int_read(State, 1),
    case Size of
        15 ->
            {ok, S2, RealSize} = int_read(S1, varint32);
        _ ->
            {S2, RealSize} = {S1, Size}
    end,
    {ok, S2, #protocol_list_begin{etype = ttype(Type),
                                  size = RealSize}};

int_read(State, list_end) -> {ok, State};


int_read(State, set_begin) ->
    {ok, S1, <<Size:4, Type:4>>} = int_read(State, 1),
    case Size of
        15 ->
            {ok, S2, RealSize} = int_read(S1, varint32);
        _ ->
            {S2, RealSize} = {S1, Size}
    end,
    {ok, S2, #protocol_set_begin{etype = ttype(Type),
                                 size = RealSize}};

int_read(State, set_end) -> {ok, State};


int_read(State = #compact_protocol{bool_field = undefined}, bool) ->
    {ok, S1, Byte} = int_read(State, byte),
    {ok, S1, (Byte /= 0)};
int_read(State = #compact_protocol{bool_field = BoolVal}, bool) ->
    {ok, State#compact_protocol{bool_field = undefined}, BoolVal};


int_read(State, byte) ->
    case int_read(State, 1) of
        {ok, <<Val:8/integer-signed-big, _/binary>>} -> {ok, State, Val};
        Else -> Else
    end;

int_read(State, i16) ->
    case int_read(State, varint32) of
        {ok, S1, Int} -> zigzag_decode(int, Int);
        Else -> Else
    end;
int_read(State, i32) ->
    case int_read(State, varint32) of
        {ok, S1, Int} -> zigzag_decode(int, Int);
        Else -> Else
    end;
int_read(State, i64) ->
    case int_read(State, varint64) of
        {ok, S1, Int} -> zigzag_decode(long, Int);
        Else -> Else
    end;

int_read(State, double) ->
    case read(State, 8) of
        {ok, <<Val:64/float-signed-big, _/binary>>} -> {ok, Val};
        Else -> Else
    end;

int_read(State, varint32) -> read_varint32(State);
int_read(State, varint64) -> read_varint64(State);


% returns a binary directly, call binary_to_list if necessary
int_read(State, string) ->
    {ok, S1, Sz}  = int_read(State, varint32),
    read(S1, Sz);

int_read(State, 0) -> {ok, State, <<>>};
int_read(State, Len) when is_integer(Len), Len >= 0 ->
    case thrift_transport:read(State#compact_protocol.transport, Len) of
        {ok, Data} ->
            {ok, State, Data};
        Else ->
            Else
    end.


%%%% FACTORY GENERATION %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% returns a (fun() -> thrift_protocol())
new_protocol_factory(TransportFactory, []) ->
    F = fun() ->
                {ok, Transport} = TransportFactory(),
                thrift_compact_protocol:new(Transport)
        end,
    {ok, F}.



%%%%% INTEGER ENCODING/DECODING %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

varint_encode(<<0:64>>) -> <<0>>;
varint_encode(<<0:57, B1:7>>) -> <<B1>>;
varint_encode(<<0:50, B1:7, B2:7>>) ->
    <<1:1, B2:7, B1>>;
varint_encode(<<0:43, B1:7, B2:7, B3:7>>) ->
    <<1:1, B3:7, 1:1, B2:7, B1>>;
varint_encode(<<0:36, B1:7, B2:7, B3:7, B4:7>>) ->
    <<1:1, B4:7, 1:1, B3:7, 1:1, B2:7, B1>>;
varint_encode(<<0:29, B1:7, B2:7, B3:7, B4:7, B5:7>>) ->
    <<1:1, B5:7, 1:1, B4:7, 1:1, B3:7, 1:1, B2:7, B1>>;
varint_encode(<<0:22, B1:7, B2:7, B3:7, B4:7, B5:7, B6:7>>) ->
    <<1:1, B6:7, 1:1, B5:7, 1:1, B4:7, 1:1, B3:7, 1:1, B2:7, B1>>;
varint_encode(<<0:15, B1:7, B2:7, B3:7, B4:7, B5:7, B6:7, B7:7>>) ->
    <<1:1, B7:7, 1:1, B6:7, 1:1, B5:7, 1:1, B4:7, 1:1, B3:7, 1:1, B2:7, B1>>;
varint_encode(<<0:8, B1:7, B2:7, B3:7, B4:7, B5:7, B6:7, B7:7, B8:7>>) ->
    <<1:1, B8:7, 1:1, B7:7, 1:1, B6:7, 1:1, B5:7, 1:1, B4:7, 1:1, B3:7, 1:1, B2:7, B1>>;
varint_encode(<<0:1, B1:7, B2:7, B3:7, B4:7, B5:7, B6:7, B7:7, B8:7, B9:7>>) ->
    <<1:1, B9:7, 1:1, B8:7, 1:1, B7:7, 1:1, B6:7, 1:1, B5:7, 1:1, B4:7, 1:1, B3:7, 1:1, B2:7, B1>>;
varint_encode(<<B1:1, B2:7, B3:7, B4:7, B5:7, B6:7, B7:7, B8:7, B9:7, B10:7>>) ->
    <<1:1, B10:7, 1:1, B9:7, 1:1, B8:7, 1:1, B7:7, 1:1, B6:7, 1:1, B5:7, 1:1, B4:7, 1:1, B3:7, 1:1, B2:7, B1>>;


varint_encode(<<0:32>>) -> <<0>>;
varint_encode(<<0:25, B1:7>>) -> <<B1>>;
varint_encode(<<0:18, B1:7, B2:7>>) ->
    <<1:1, B2:7, B1>>;
varint_encode(<<0:11, B1:7, B2:7, B3:7>>) ->
    <<1:1, B3:7, 1:1, B2:7, B1>>;
varint_encode(<<0:4, B1:7, B2:7, B3:7, B4:7>>) ->
    <<1:1, B4:7, 1:1, B3:7, 1:1, B2:7, B1>>;
varint_encode(<<B1:4, B2:7, B3:7, B4:7, B5:7>>) ->
    <<1:1, B5:7, 1:1, B4:7, 1:1, B3:7, 1:1, B2:7, B1>>.

varint_decode(Bin) ->
    varint_decode(Bin, []).
varint_decode(<<1:1, Bits:7, Rest/binary>>, Acc) ->
    varint_decode(Rest, [Bits | Acc]);
varint_decode(<<0:1, NewBits:7>>, Acc) ->
    NewAcc = [NewBits | Acc],
    %% Now we have the bits in most sig to least sig
    lists:foldl(fun(Bits, AccIn) ->
                        AccIn bsl 7 + Bits
                end,
                0,
                NewAcc).

read_varint32(State) ->
    {ok, S1, ZigInt} = read_varint(State, <<>>, 32),
    {ok, S1, zigzag_decode(int, ZigInt)}.
read_varint64(State) ->
    {ok, S1, ZigInt} = read_varint(State, <<>>, 64),
    {ok, S1, zigzag_decode(long, ZigInt)}.


read_varint(State, Acc, BitsLeft) when BitsLeft >= 0 ->
    {ok, S1, Data = <<ContBit:1, DataBits:7>>} = int_read(State, 1),
    NewAcc = <<Acc/binary, Data/binary>>,
    case ContBit of
        1 -> read_varint(S1, NewAcc, BitsLeft - 8);
        0 -> {ok, S1, varint_decode(NewAcc)}
    end.
              

zigzag_encode(int, Int) ->
    (Int bsl 1) bxor (Int bsr 31);
zigzag_encode(long, Int) ->
    (Int bsl 1) bxor (Int bsr 63).

zigzag_decode(int, ZigInt) ->
    (ZigInt bsr 1) bxor -(ZigInt band 1);
zigzag_decode(long, ZigInt) ->
    (ZigInt bsr 1) bxor -(ZigInt band 1).


compact_type(?tType_STOP) -> ?tType_STOP;
compact_type(?tType_BOOL) -> ?CTYPE_BOOLEAN_TRUE;
compact_type(?tType_BYTE) -> ?CTYPE_BYTE;
compact_type(?tType_I16) -> ?CTYPE_I16;
compact_type(?tType_I32) -> ?CTYPE_I32;
compact_type(?tType_I64) -> ?CTYPE_I64;
compact_type(?tType_DOUBLE) -> ?CTYPE_DOUBLE;
compact_type(?tType_STRING) -> ?CTYPE_BINARY;
compact_type(?tType_LIST) -> ?CTYPE_LIST;
compact_type(?tType_SET) -> ?CTYPE_SET;
compact_type(?tType_MAP) -> ?CTYPE_MAP;
compact_type(?tType_STRUCT) -> ?CTYPE_STRUCT.

ttype(?tType_STOP) -> ?tType_STOP;
ttype(?CTYPE_BOOLEAN_TRUE) -> ?tType_BOOL;
ttype(?CTYPE_BYTE) -> ?tType_BYTE;
ttype(?CTYPE_I16) -> ?tType_I16;
ttype(?CTYPE_I32) -> ?tType_I32;
ttype(?CTYPE_I64) -> ?tType_I64;
ttype(?CTYPE_DOUBLE) -> ?tType_DOUBLE;
ttype(?CTYPE_BINARY) -> ?tType_STRING;
ttype(?CTYPE_LIST) -> ?tType_LIST;
ttype(?CTYPE_SET) -> ?tType_SET;
ttype(?CTYPE_MAP) -> ?tType_MAP;
ttype(?CTYPE_STRUCT) -> ?tType_STRUCT.

