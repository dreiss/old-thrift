
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "thrifty.yy"

// Copyright (c) 2006- Facebook
// Distributed under the Thrift Software License
//
// See accompanying file LICENSE or visit the Thrift site at:
// http://developers.facebook.com/thrift/

/**
 * Thrift parser.
 *
 * This parser is used on a thrift definition file.
 *
 */

#define __STDC_LIMIT_MACROS
#define __STDC_FORMAT_MACROS
#include <stdio.h>
#include <inttypes.h>
#include <limits.h>
#include "main.h"
#include "globals.h"
#include "parse/t_program.h"
#include "parse/t_scope.h"

/**
 * This global variable is used for automatic numbering of field indices etc.
 * when parsing the members of a struct. Field values are automatically
 * assigned starting from -1 and working their way down.
 */
int y_field_val = -1;
int g_arglist = 0;



/* Line 189 of yacc.c  */
#line 108 "thrifty.cc"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     tok_identifier = 258,
     tok_literal = 259,
     tok_doctext = 260,
     tok_st_identifier = 261,
     tok_int_constant = 262,
     tok_dub_constant = 263,
     tok_include = 264,
     tok_namespace = 265,
     tok_cpp_namespace = 266,
     tok_cpp_include = 267,
     tok_cpp_type = 268,
     tok_php_namespace = 269,
     tok_py_module = 270,
     tok_perl_package = 271,
     tok_java_package = 272,
     tok_xsd_all = 273,
     tok_xsd_optional = 274,
     tok_xsd_nillable = 275,
     tok_xsd_namespace = 276,
     tok_xsd_attrs = 277,
     tok_ruby_namespace = 278,
     tok_smalltalk_category = 279,
     tok_smalltalk_prefix = 280,
     tok_cocoa_prefix = 281,
     tok_csharp_namespace = 282,
     tok_void = 283,
     tok_bool = 284,
     tok_byte = 285,
     tok_string = 286,
     tok_binary = 287,
     tok_slist = 288,
     tok_senum = 289,
     tok_i16 = 290,
     tok_i32 = 291,
     tok_i64 = 292,
     tok_double = 293,
     tok_map = 294,
     tok_list = 295,
     tok_set = 296,
     tok_oneway = 297,
     tok_typedef = 298,
     tok_struct = 299,
     tok_xception = 300,
     tok_throws = 301,
     tok_extends = 302,
     tok_service = 303,
     tok_enum = 304,
     tok_const = 305,
     tok_required = 306,
     tok_optional = 307
   };
#endif
/* Tokens.  */
#define tok_identifier 258
#define tok_literal 259
#define tok_doctext 260
#define tok_st_identifier 261
#define tok_int_constant 262
#define tok_dub_constant 263
#define tok_include 264
#define tok_namespace 265
#define tok_cpp_namespace 266
#define tok_cpp_include 267
#define tok_cpp_type 268
#define tok_php_namespace 269
#define tok_py_module 270
#define tok_perl_package 271
#define tok_java_package 272
#define tok_xsd_all 273
#define tok_xsd_optional 274
#define tok_xsd_nillable 275
#define tok_xsd_namespace 276
#define tok_xsd_attrs 277
#define tok_ruby_namespace 278
#define tok_smalltalk_category 279
#define tok_smalltalk_prefix 280
#define tok_cocoa_prefix 281
#define tok_csharp_namespace 282
#define tok_void 283
#define tok_bool 284
#define tok_byte 285
#define tok_string 286
#define tok_binary 287
#define tok_slist 288
#define tok_senum 289
#define tok_i16 290
#define tok_i32 291
#define tok_i64 292
#define tok_double 293
#define tok_map 294
#define tok_list 295
#define tok_set 296
#define tok_oneway 297
#define tok_typedef 298
#define tok_struct 299
#define tok_xception 300
#define tok_throws 301
#define tok_extends 302
#define tok_service 303
#define tok_enum 304
#define tok_const 305
#define tok_required 306
#define tok_optional 307




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 222 of yacc.c  */
#line 39 "thrifty.yy"

  char*          id;
  int64_t        iconst;
  double         dconst;
  bool           tbool;
  t_doc*         tdoc;
  t_type*        ttype;
  t_base_type*   tbase;
  t_typedef*     ttypedef;
  t_enum*        tenum;
  t_enum_value*  tenumv;
  t_const*       tconst;
  t_const_value* tconstv;
  t_struct*      tstruct;
  t_service*     tservice;
  t_function*    tfunction;
  t_field*       tfield;
  char*          dtext;
  t_field::e_req ereq;
  t_annotation*  tannot;



/* Line 222 of yacc.c  */
#line 272 "thrifty.cc"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 284 "thrifty.cc"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   198

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  65
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  56
/* YYNRULES -- Number of rules.  */
#define YYNRULES  119
/* YYNRULES -- Number of states.  */
#define YYNSTATES  205

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   307

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      61,    62,     2,     2,    53,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    60,    54,
      63,    57,    64,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    58,     2,    59,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    55,     2,    56,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     7,     8,    12,    13,    15,    19,
      22,    25,    28,    31,    34,    37,    40,    43,    46,    49,
      52,    55,    58,    62,    63,    65,    67,    69,    71,    73,
      75,    77,    79,    83,    85,    87,    88,    94,    97,    98,
     104,   108,   114,   117,   118,   121,   128,   130,   132,   134,
     136,   138,   140,   144,   148,   149,   153,   159,   160,   168,
     170,   171,   173,   174,   176,   177,   182,   183,   189,   198,
     199,   200,   203,   204,   207,   208,   218,   220,   221,   226,
     227,   230,   231,   242,   245,   246,   248,   250,   251,   254,
     255,   257,   259,   261,   263,   265,   267,   269,   271,   273,
     275,   277,   279,   281,   283,   285,   287,   290,   292,   294,
     296,   304,   310,   316,   319,   320,   324,   325,   328,   329
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      66,     0,    -1,    69,    72,    -1,    -1,    -1,    69,    68,
      70,    -1,    -1,    71,    -1,    10,     3,     3,    -1,    11,
       3,    -1,    12,     4,    -1,    14,     3,    -1,    15,     3,
      -1,    16,     3,    -1,    23,     3,    -1,    24,     6,    -1,
      25,     3,    -1,    17,     3,    -1,    26,     3,    -1,    21,
       4,    -1,    27,     3,    -1,     9,     4,    -1,    72,    67,
      73,    -1,    -1,    83,    -1,    74,    -1,    95,    -1,    75,
      -1,    77,    -1,    80,    -1,    89,    -1,    94,    -1,    43,
     108,     3,    -1,    53,    -1,    54,    -1,    -1,    49,     3,
      55,    78,    56,    -1,    78,    79,    -1,    -1,    67,     3,
      57,     7,    76,    -1,    67,     3,    76,    -1,    34,     3,
      55,    81,    56,    -1,    81,    82,    -1,    -1,     4,    76,
      -1,    50,   110,     3,    57,    84,    76,    -1,     7,    -1,
       8,    -1,     4,    -1,     3,    -1,    85,    -1,    87,    -1,
      58,    86,    59,    -1,    86,    84,    76,    -1,    -1,    55,
      88,    56,    -1,    88,    84,    60,    84,    76,    -1,    -1,
      44,     3,    90,    55,   103,    56,   118,    -1,    18,    -1,
      -1,    19,    -1,    -1,    20,    -1,    -1,    22,    55,   103,
      56,    -1,    -1,    45,     3,    55,   103,    56,    -1,    48,
       3,    98,    55,    96,    99,    97,    56,    -1,    -1,    -1,
      47,     3,    -1,    -1,    99,   100,    -1,    -1,    67,   101,
     109,     3,    61,   103,    62,   102,    76,    -1,    42,    -1,
      -1,    46,    61,   103,    62,    -1,    -1,   103,   104,    -1,
      -1,    67,   105,   106,   110,     3,   107,    91,    92,    93,
      76,    -1,     7,    60,    -1,    -1,    51,    -1,    52,    -1,
      -1,    57,    84,    -1,    -1,   111,    -1,   112,    -1,   110,
      -1,    28,    -1,     3,    -1,   111,    -1,   112,    -1,    31,
      -1,    32,    -1,    33,    -1,    29,    -1,    30,    -1,    35,
      -1,    36,    -1,    37,    -1,    38,    -1,   113,   118,    -1,
     114,    -1,   115,    -1,   116,    -1,    39,   117,    63,   110,
      53,   110,    64,    -1,    41,   117,    63,   110,    64,    -1,
      40,    63,   110,    64,   117,    -1,    13,     4,    -1,    -1,
      61,   119,    62,    -1,    -1,   119,   120,    -1,    -1,     3,
      57,     4,    76,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   212,   212,   225,   236,   245,   250,   255,   259,   267,
     275,   282,   291,   300,   309,   318,   327,   336,   345,   354,
     363,   373,   385,   393,   398,   406,   417,   431,   438,   445,
     452,   459,   468,   476,   478,   481,   484,   492,   499,   505,
     526,   536,   543,   550,   557,   564,   582,   591,   597,   602,
     615,   620,   627,   634,   641,   648,   655,   662,   669,   682,
     687,   692,   697,   702,   707,   712,   717,   722,   731,   740,
     745,   750,   763,   768,   775,   781,   791,   796,   801,   811,
     816,   827,   834,   857,   866,   871,   882,   894,   899,   908,
     913,   918,   925,   930,   937,   952,   957,   964,   969,   974,
     979,   984,   989,   994,   999,  1004,  1010,  1021,  1026,  1031,
    1038,  1048,  1058,  1068,  1073,  1078,  1084,  1089,  1097,  1103
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "tok_identifier", "tok_literal",
  "tok_doctext", "tok_st_identifier", "tok_int_constant",
  "tok_dub_constant", "tok_include", "tok_namespace", "tok_cpp_namespace",
  "tok_cpp_include", "tok_cpp_type", "tok_php_namespace", "tok_py_module",
  "tok_perl_package", "tok_java_package", "tok_xsd_all",
  "tok_xsd_optional", "tok_xsd_nillable", "tok_xsd_namespace",
  "tok_xsd_attrs", "tok_ruby_namespace", "tok_smalltalk_category",
  "tok_smalltalk_prefix", "tok_cocoa_prefix", "tok_csharp_namespace",
  "tok_void", "tok_bool", "tok_byte", "tok_string", "tok_binary",
  "tok_slist", "tok_senum", "tok_i16", "tok_i32", "tok_i64", "tok_double",
  "tok_map", "tok_list", "tok_set", "tok_oneway", "tok_typedef",
  "tok_struct", "tok_xception", "tok_throws", "tok_extends", "tok_service",
  "tok_enum", "tok_const", "tok_required", "tok_optional", "','", "';'",
  "'{'", "'}'", "'='", "'['", "']'", "':'", "'('", "')'", "'<'", "'>'",
  "$accept", "Program", "CaptureDocText", "DestroyDocText", "HeaderList",
  "Header", "Include", "DefinitionList", "Definition", "TypeDefinition",
  "Typedef", "CommaOrSemicolonOptional", "Enum", "EnumDefList", "EnumDef",
  "Senum", "SenumDefList", "SenumDef", "Const", "ConstValue", "ConstList",
  "ConstListContents", "ConstMap", "ConstMapContents", "Struct", "XsdAll",
  "XsdOptional", "XsdNillable", "XsdAttributes", "Xception", "Service",
  "FlagArgs", "UnflagArgs", "Extends", "FunctionList", "Function",
  "Oneway", "Throws", "FieldList", "Field", "FieldIdentifier",
  "FieldRequiredness", "FieldValue", "DefinitionType", "FunctionType",
  "FieldType", "BaseType", "ContainerType", "SimpleContainerType",
  "MapType", "SetType", "ListType", "CppType", "TypeAnnotations",
  "TypeAnnotationList", "TypeAnnotation", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,    44,    59,   123,   125,    61,    91,    93,
      58,    40,    41,    60,    62
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    65,    66,    67,    68,    69,    69,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,    70,    70,
      70,    71,    72,    72,    73,    73,    73,    74,    74,    74,
      74,    74,    75,    76,    76,    76,    77,    78,    78,    79,
      79,    80,    81,    81,    82,    83,    84,    84,    84,    84,
      84,    84,    85,    86,    86,    87,    88,    88,    89,    90,
      90,    91,    91,    92,    92,    93,    93,    94,    95,    96,
      97,    98,    98,    99,    99,   100,   101,   101,   102,   102,
     103,   103,   104,   105,   105,   106,   106,   106,   107,   107,
     108,   108,   109,   109,   110,   110,   110,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   112,   113,   113,   113,
     114,   115,   116,   117,   117,   118,   118,   119,   119,   120
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     0,     0,     3,     0,     1,     3,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     3,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     1,     0,     5,     2,     0,     5,
       3,     5,     2,     0,     2,     6,     1,     1,     1,     1,
       1,     1,     3,     3,     0,     3,     5,     0,     7,     1,
       0,     1,     0,     1,     0,     4,     0,     5,     8,     0,
       0,     2,     0,     2,     0,     9,     1,     0,     4,     0,
       2,     0,    10,     2,     0,     1,     1,     0,     2,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       7,     5,     5,     2,     0,     3,     0,     2,     0,     4
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       6,     0,     4,     1,     0,     3,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,     7,     0,    21,     0,     9,    10,    11,    12,    13,
      17,    19,    14,    15,    16,    18,    20,     0,     0,     0,
       0,     0,     0,     0,    22,    25,    27,    28,    29,    24,
      30,    31,    26,     8,     0,   100,   101,    97,    98,    99,
     102,   103,   104,   105,   114,     0,   114,     0,    90,    91,
     116,   107,   108,   109,    60,     0,    72,     0,    94,     0,
      95,    96,    43,     0,     0,     0,     0,    32,   118,   106,
      59,     0,    81,     0,     0,    38,     0,     0,   113,     0,
       0,     0,     0,    81,     3,    71,    69,     3,     0,    35,
      41,    42,     0,   114,     0,     0,   115,   117,     3,    67,
      84,    80,    74,    36,     0,    37,    49,    48,    46,    47,
      57,    54,    35,    50,    51,    33,    34,    44,     0,   112,
     111,     0,   116,     0,    87,     3,    35,     0,     0,    45,
       0,    35,    58,    83,    85,    86,     0,    77,     0,    73,
       0,    40,    55,     0,    52,    35,   110,   119,     0,    76,
       0,    68,    35,     0,    53,    89,    93,     0,    92,    39,
      35,     0,    62,     0,    56,    88,    61,    64,    81,    63,
      66,     3,     0,    35,    79,    81,    82,     0,    35,     3,
      81,    75,    65,     3,    78
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   120,     4,     2,    20,    21,     5,    44,    45,
      46,   137,    47,   107,   125,    48,    97,   111,    49,   132,
     133,   148,   134,   147,    50,    91,   187,   190,   193,    51,
      52,   122,   158,    94,   145,   159,   170,   198,   104,   121,
     144,   156,   182,    67,   177,    79,    80,    81,    70,    71,
      72,    73,    84,    89,   102,   117
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -134
static const yytype_int16 yypact[] =
{
    -134,    33,    32,  -134,   121,    34,    31,    35,    36,    37,
      39,    41,    42,    43,    45,    44,    46,    47,    50,    53,
    -134,  -134,    51,  -134,    55,  -134,  -134,  -134,  -134,  -134,
    -134,  -134,  -134,  -134,  -134,  -134,  -134,    64,   120,    67,
      76,    80,    81,    88,  -134,  -134,  -134,  -134,  -134,  -134,
    -134,  -134,  -134,  -134,   -18,  -134,  -134,  -134,  -134,  -134,
    -134,  -134,  -134,  -134,    77,    -9,    77,    85,  -134,  -134,
      28,  -134,  -134,  -134,    74,    54,    87,    84,  -134,    94,
    -134,  -134,  -134,   118,    78,    88,    91,  -134,  -134,  -134,
    -134,   107,  -134,   140,   108,  -134,   109,     9,  -134,    88,
     100,    88,     1,  -134,   111,  -134,  -134,   112,     4,   -26,
    -134,  -134,   116,    77,   101,   113,  -134,  -134,   115,  -134,
     165,  -134,  -134,  -134,   170,  -134,  -134,  -134,  -134,  -134,
    -134,  -134,   -26,  -134,  -134,  -134,  -134,  -134,    88,  -134,
    -134,   171,    28,   114,   -21,   122,   -28,    16,     2,  -134,
     117,   -26,  -134,  -134,  -134,  -134,    88,   134,   123,  -134,
     173,  -134,  -134,   124,  -134,   -26,  -134,  -134,   174,  -134,
      75,  -134,   -26,     4,  -134,   125,  -134,   180,  -134,  -134,
     -26,     4,   166,   126,  -134,  -134,  -134,   168,  -134,  -134,
     164,   127,   135,   -26,   145,  -134,  -134,   131,   -26,   137,
    -134,  -134,  -134,   132,  -134
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -134,  -134,    -5,  -134,  -134,  -134,  -134,  -134,  -134,  -134,
    -134,  -129,  -134,  -134,  -134,  -134,  -134,  -134,  -134,  -133,
    -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,
    -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,  -102,  -134,
    -134,  -134,  -134,  -134,  -134,   -83,   157,   158,  -134,  -134,
    -134,  -134,   -45,    56,  -134,  -134
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -71
static const yytype_int16 yytable[] =
{
      22,   118,   100,   149,   115,   126,   127,   126,   127,   128,
     129,   128,   129,   109,   163,   165,   112,   161,   114,   126,
     127,    86,   167,   128,   129,   135,   136,   135,   136,   160,
     154,   155,   -23,     3,    -2,    23,   174,    82,    24,    25,
     180,    26,    27,   179,    28,    29,    30,    32,   185,    31,
      34,   184,    33,    35,    85,   150,    36,   130,    53,   130,
     131,   164,   131,   116,   196,   110,   -23,    54,   139,   201,
      74,   130,   162,   168,   131,   -23,   -23,   -23,    78,    75,
     -23,   -23,   -23,    76,    77,    37,   191,   178,    87,    88,
      83,    78,    90,   199,    38,    39,    40,    96,   203,    41,
      42,    43,   124,   176,    55,    56,    57,    58,    59,    92,
      60,    61,    62,    63,    64,    65,    66,    55,    56,    57,
      58,    59,    98,    60,    61,    62,    63,    64,    65,    66,
       6,     7,     8,     9,    93,    10,    11,    12,    13,    95,
     157,    99,    14,   105,    15,    16,    17,    18,    19,    55,
      56,    57,    58,    59,   101,    60,    61,    62,    63,    64,
      65,    66,   103,   106,   113,   140,   108,   119,   123,   138,
     141,   142,   143,   146,   153,   151,   169,   175,   -70,   171,
     172,   166,   181,   183,   173,   186,   192,   188,   189,   194,
     195,   197,   200,   202,   204,    68,    69,     0,   152
};

static const yytype_int16 yycheck[] =
{
       5,   103,    85,   132,     3,     3,     4,     3,     4,     7,
       8,     7,     8,     4,   147,   148,    99,   146,   101,     3,
       4,    66,   151,     7,     8,    53,    54,    53,    54,    57,
      51,    52,     0,     0,     0,     4,   165,    55,     3,     3,
     173,     4,     3,   172,     3,     3,     3,     3,   181,     4,
       3,   180,     6,     3,    63,   138,     3,    55,     3,    55,
      58,    59,    58,    62,   193,    56,    34,     3,   113,   198,
       3,    55,    56,   156,    58,    43,    44,    45,     3,     3,
      48,    49,    50,     3,     3,    34,   188,   170,     3,    61,
      13,     3,    18,   195,    43,    44,    45,     3,   200,    48,
      49,    50,   107,    28,    29,    30,    31,    32,    33,    55,
      35,    36,    37,    38,    39,    40,    41,    29,    30,    31,
      32,    33,     4,    35,    36,    37,    38,    39,    40,    41,
       9,    10,    11,    12,    47,    14,    15,    16,    17,    55,
     145,    63,    21,     3,    23,    24,    25,    26,    27,    29,
      30,    31,    32,    33,    63,    35,    36,    37,    38,    39,
      40,    41,    55,    55,    64,    64,    57,    56,    56,    53,
      57,    56,     7,     3,    60,     4,    42,     3,    56,    56,
       7,    64,    57,     3,    60,    19,    22,    61,    20,    62,
      55,    46,    61,    56,    62,    38,    38,    -1,   142
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    66,    69,     0,    68,    72,     9,    10,    11,    12,
      14,    15,    16,    17,    21,    23,    24,    25,    26,    27,
      70,    71,    67,     4,     3,     3,     4,     3,     3,     3,
       3,     4,     3,     6,     3,     3,     3,    34,    43,    44,
      45,    48,    49,    50,    73,    74,    75,    77,    80,    83,
      89,    94,    95,     3,     3,    29,    30,    31,    32,    33,
      35,    36,    37,    38,    39,    40,    41,   108,   111,   112,
     113,   114,   115,   116,     3,     3,     3,     3,     3,   110,
     111,   112,    55,    13,   117,    63,   117,     3,    61,   118,
      18,    90,    55,    47,    98,    55,     3,    81,     4,    63,
     110,    63,   119,    55,   103,     3,    55,    78,    57,     4,
      56,    82,   110,    64,   110,     3,    62,   120,   103,    56,
      67,   104,    96,    56,    67,    79,     3,     4,     7,     8,
      55,    58,    84,    85,    87,    53,    54,    76,    53,   117,
      64,    57,    56,     7,   105,    99,     3,    88,    86,    76,
     110,     4,   118,    60,    51,    52,   106,    67,    97,   100,
      57,    76,    56,    84,    59,    84,    64,    76,   110,    42,
     101,    56,     7,    60,    76,     3,    28,   109,   110,    76,
      84,    57,   107,     3,    76,    84,    19,    91,    61,    20,
      92,   103,    22,    93,    62,    55,    76,    46,   102,   103,
      61,    76,    56,   103,    62
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 213 "thrifty.yy"
    {
      pdebug("Program -> Headers DefinitionList");
      /*
      TODO(dreiss): Decide whether full-program doctext is worth the trouble.
      if ($1 != NULL) {
        g_program->set_doc($1);
      }
      */
      clear_doctext();
    }
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 225 "thrifty.yy"
    {
      if (g_parse_mode == PROGRAM) {
        (yyval.dtext) = g_doctext;
        g_doctext = NULL;
      } else {
        (yyval.dtext) = NULL;
      }
    }
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 236 "thrifty.yy"
    {
      if (g_parse_mode == PROGRAM) {
        clear_doctext();
      }
    }
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 246 "thrifty.yy"
    {
      pdebug("HeaderList -> HeaderList Header");
    }
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 250 "thrifty.yy"
    {
      pdebug("HeaderList -> ");
    }
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 256 "thrifty.yy"
    {
      pdebug("Header -> Include");
    }
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 260 "thrifty.yy"
    {
      pdebug("Header -> tok_namespace tok_identifier tok_identifier");
      if (g_parse_mode == PROGRAM) {
        g_program->set_namespace((yyvsp[(2) - (3)].id), (yyvsp[(3) - (3)].id));
      }
    }
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 268 "thrifty.yy"
    {
      pwarning(1, "'cpp_namespace' is deprecated. Use 'namespace cpp' instead");
      pdebug("Header -> tok_cpp_namespace tok_identifier");
      if (g_parse_mode == PROGRAM) {
        g_program->set_namespace("cpp", (yyvsp[(2) - (2)].id));
      }
    }
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 276 "thrifty.yy"
    {
      pdebug("Header -> tok_cpp_include tok_literal");
      if (g_parse_mode == PROGRAM) {
        g_program->add_cpp_include((yyvsp[(2) - (2)].id));
      }
    }
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 283 "thrifty.yy"
    {
      pwarning(1, "'php_namespace' is deprecated. Use 'namespace php' instead");
      pdebug("Header -> tok_php_namespace tok_identifier");
      if (g_parse_mode == PROGRAM) {
        g_program->set_namespace("php", (yyvsp[(2) - (2)].id));
      }
    }
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 292 "thrifty.yy"
    {
      pwarning(1, "'py_module' is deprecated. Use 'namespace py' instead");
      pdebug("Header -> tok_py_module tok_identifier");
      if (g_parse_mode == PROGRAM) {
        g_program->set_namespace("py", (yyvsp[(2) - (2)].id));
      }
    }
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 301 "thrifty.yy"
    {
      pwarning(1, "'perl_package' is deprecated. Use 'namespace perl' instead");
      pdebug("Header -> tok_perl_namespace tok_identifier");
      if (g_parse_mode == PROGRAM) {
        g_program->set_namespace("perl", (yyvsp[(2) - (2)].id));
      }
    }
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 310 "thrifty.yy"
    {
      pwarning(1, "'ruby_namespace' is deprecated. Use 'namespace rb' instead");
      pdebug("Header -> tok_ruby_namespace tok_identifier");
      if (g_parse_mode == PROGRAM) {
        g_program->set_namespace("rb", (yyvsp[(2) - (2)].id));
      }
    }
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 319 "thrifty.yy"
    {
      pwarning(1, "'smalltalk_category' is deprecated. Use 'namespace smalltalk.category' instead");
      pdebug("Header -> tok_smalltalk_category tok_st_identifier");
      if (g_parse_mode == PROGRAM) {
        g_program->set_namespace("smalltalk.category", (yyvsp[(2) - (2)].id));
      }
    }
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 328 "thrifty.yy"
    {
      pwarning(1, "'smalltalk_prefix' is deprecated. Use 'namespace smalltalk.prefix' instead");
      pdebug("Header -> tok_smalltalk_prefix tok_identifier");
      if (g_parse_mode == PROGRAM) {
        g_program->set_namespace("smalltalk.prefix", (yyvsp[(2) - (2)].id));
      }
    }
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 337 "thrifty.yy"
    {
      pwarning(1, "'java_package' is deprecated. Use 'namespace java' instead");
      pdebug("Header -> tok_java_package tok_identifier");
      if (g_parse_mode == PROGRAM) {
        g_program->set_namespace("java", (yyvsp[(2) - (2)].id));
      }
    }
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 346 "thrifty.yy"
    {
      pwarning(1, "'cocoa_prefix' is deprecated. Use 'namespace cocoa' instead");
      pdebug("Header -> tok_cocoa_prefix tok_identifier");
      if (g_parse_mode == PROGRAM) {
        g_program->set_namespace("cocoa", (yyvsp[(2) - (2)].id));
      }
    }
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 355 "thrifty.yy"
    {
      pwarning(1, "'xsd_namespace' is deprecated. Use 'namespace xsd' instead");
      pdebug("Header -> tok_xsd_namespace tok_literal");
      if (g_parse_mode == PROGRAM) {
        g_program->set_namespace("cocoa", (yyvsp[(2) - (2)].id));
      }
    }
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 364 "thrifty.yy"
    {
     pwarning(1, "'csharp_namespace' is deprecated. Use 'namespace csharp' instead");
     pdebug("Header -> tok_csharp_namespace tok_identifier");
     if (g_parse_mode == PROGRAM) {
       g_program->set_namespace("csharp", (yyvsp[(2) - (2)].id));
     }
   }
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 374 "thrifty.yy"
    {
      pdebug("Include -> tok_include tok_literal");
      if (g_parse_mode == INCLUDES) {
        std::string path = include_file(std::string((yyvsp[(2) - (2)].id)));
        if (!path.empty()) {
          g_program->add_include(path, std::string((yyvsp[(2) - (2)].id)));
        }
      }
    }
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 386 "thrifty.yy"
    {
      pdebug("DefinitionList -> DefinitionList Definition");
      if ((yyvsp[(2) - (3)].dtext) != NULL && (yyvsp[(3) - (3)].tdoc) != NULL) {
        (yyvsp[(3) - (3)].tdoc)->set_doc((yyvsp[(2) - (3)].dtext));
      }
    }
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 393 "thrifty.yy"
    {
      pdebug("DefinitionList -> ");
    }
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 399 "thrifty.yy"
    {
      pdebug("Definition -> Const");
      if (g_parse_mode == PROGRAM) {
        g_program->add_const((yyvsp[(1) - (1)].tconst));
      }
      (yyval.tdoc) = (yyvsp[(1) - (1)].tconst);
    }
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 407 "thrifty.yy"
    {
      pdebug("Definition -> TypeDefinition");
      if (g_parse_mode == PROGRAM) {
        g_scope->add_type((yyvsp[(1) - (1)].ttype)->get_name(), (yyvsp[(1) - (1)].ttype));
        if (g_parent_scope != NULL) {
          g_parent_scope->add_type(g_parent_prefix + (yyvsp[(1) - (1)].ttype)->get_name(), (yyvsp[(1) - (1)].ttype));
        }
      }
      (yyval.tdoc) = (yyvsp[(1) - (1)].ttype);
    }
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 418 "thrifty.yy"
    {
      pdebug("Definition -> Service");
      if (g_parse_mode == PROGRAM) {
        g_scope->add_service((yyvsp[(1) - (1)].tservice)->get_name(), (yyvsp[(1) - (1)].tservice));
        if (g_parent_scope != NULL) {
          g_parent_scope->add_service(g_parent_prefix + (yyvsp[(1) - (1)].tservice)->get_name(), (yyvsp[(1) - (1)].tservice));
        }
        g_program->add_service((yyvsp[(1) - (1)].tservice));
      }
      (yyval.tdoc) = (yyvsp[(1) - (1)].tservice);
    }
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 432 "thrifty.yy"
    {
      pdebug("TypeDefinition -> Typedef");
      if (g_parse_mode == PROGRAM) {
        g_program->add_typedef((yyvsp[(1) - (1)].ttypedef));
      }
    }
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 439 "thrifty.yy"
    {
      pdebug("TypeDefinition -> Enum");
      if (g_parse_mode == PROGRAM) {
        g_program->add_enum((yyvsp[(1) - (1)].tenum));
      }
    }
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 446 "thrifty.yy"
    {
      pdebug("TypeDefinition -> Senum");
      if (g_parse_mode == PROGRAM) {
        g_program->add_typedef((yyvsp[(1) - (1)].ttypedef));
      }
    }
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 453 "thrifty.yy"
    {
      pdebug("TypeDefinition -> Struct");
      if (g_parse_mode == PROGRAM) {
        g_program->add_struct((yyvsp[(1) - (1)].tstruct));
      }
    }
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 460 "thrifty.yy"
    {
      pdebug("TypeDefinition -> Xception");
      if (g_parse_mode == PROGRAM) {
        g_program->add_xception((yyvsp[(1) - (1)].tstruct));
      }
    }
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 469 "thrifty.yy"
    {
      pdebug("TypeDef -> tok_typedef DefinitionType tok_identifier");
      t_typedef *td = new t_typedef(g_program, (yyvsp[(2) - (3)].ttype), (yyvsp[(3) - (3)].id));
      (yyval.ttypedef) = td;
    }
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 477 "thrifty.yy"
    {}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 479 "thrifty.yy"
    {}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 481 "thrifty.yy"
    {}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 485 "thrifty.yy"
    {
      pdebug("Enum -> tok_enum tok_identifier { EnumDefList }");
      (yyval.tenum) = (yyvsp[(4) - (5)].tenum);
      (yyval.tenum)->set_name((yyvsp[(2) - (5)].id));
    }
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 493 "thrifty.yy"
    {
      pdebug("EnumDefList -> EnumDefList EnumDef");
      (yyval.tenum) = (yyvsp[(1) - (2)].tenum);
      (yyval.tenum)->append((yyvsp[(2) - (2)].tenumv));
    }
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 499 "thrifty.yy"
    {
      pdebug("EnumDefList -> ");
      (yyval.tenum) = new t_enum(g_program);
    }
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 506 "thrifty.yy"
    {
      pdebug("EnumDef -> tok_identifier = tok_int_constant");
      if ((yyvsp[(4) - (5)].iconst) < 0) {
        pwarning(1, "Negative value supplied for enum %s.\n", (yyvsp[(2) - (5)].id));
      }
      if ((yyvsp[(4) - (5)].iconst) > INT_MAX) {
        pwarning(1, "64-bit value supplied for enum %s.\n", (yyvsp[(2) - (5)].id));
      }
      (yyval.tenumv) = new t_enum_value((yyvsp[(2) - (5)].id), (yyvsp[(4) - (5)].iconst));
      if ((yyvsp[(1) - (5)].dtext) != NULL) {
        (yyval.tenumv)->set_doc((yyvsp[(1) - (5)].dtext));
      }
      if (g_parse_mode == PROGRAM) {
        g_scope->add_constant((yyvsp[(2) - (5)].id), new t_const(g_type_i32, (yyvsp[(2) - (5)].id), new t_const_value((yyvsp[(4) - (5)].iconst))));
        if (g_parent_scope != NULL) {
          g_parent_scope->add_constant(g_parent_prefix + (yyvsp[(2) - (5)].id), new t_const(g_type_i32, (yyvsp[(2) - (5)].id), new t_const_value((yyvsp[(4) - (5)].iconst))));
        }
      }
    }
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 527 "thrifty.yy"
    {
      pdebug("EnumDef -> tok_identifier");
      (yyval.tenumv) = new t_enum_value((yyvsp[(2) - (3)].id));
      if ((yyvsp[(1) - (3)].dtext) != NULL) {
        (yyval.tenumv)->set_doc((yyvsp[(1) - (3)].dtext));
      }
    }
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 537 "thrifty.yy"
    {
      pdebug("Senum -> tok_senum tok_identifier { SenumDefList }");
      (yyval.ttypedef) = new t_typedef(g_program, (yyvsp[(4) - (5)].tbase), (yyvsp[(2) - (5)].id));
    }
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 544 "thrifty.yy"
    {
      pdebug("SenumDefList -> SenumDefList SenumDef");
      (yyval.tbase) = (yyvsp[(1) - (2)].tbase);
      (yyval.tbase)->add_string_enum_val((yyvsp[(2) - (2)].id));
    }
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 550 "thrifty.yy"
    {
      pdebug("SenumDefList -> ");
      (yyval.tbase) = new t_base_type("string", t_base_type::TYPE_STRING);
      (yyval.tbase)->set_string_enum(true);
    }
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 558 "thrifty.yy"
    {
      pdebug("SenumDef -> tok_literal");
      (yyval.id) = (yyvsp[(1) - (2)].id);
    }
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 565 "thrifty.yy"
    {
      pdebug("Const -> tok_const FieldType tok_identifier = ConstValue");
      if (g_parse_mode == PROGRAM) {
        (yyval.tconst) = new t_const((yyvsp[(2) - (6)].ttype), (yyvsp[(3) - (6)].id), (yyvsp[(5) - (6)].tconstv));
        validate_const_type((yyval.tconst));

        g_scope->add_constant((yyvsp[(3) - (6)].id), (yyval.tconst));
        if (g_parent_scope != NULL) {
          g_parent_scope->add_constant(g_parent_prefix + (yyvsp[(3) - (6)].id), (yyval.tconst));
        }

      } else {
        (yyval.tconst) = NULL;
      }
    }
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 583 "thrifty.yy"
    {
      pdebug("ConstValue => tok_int_constant");
      (yyval.tconstv) = new t_const_value();
      (yyval.tconstv)->set_integer((yyvsp[(1) - (1)].iconst));
      if ((yyvsp[(1) - (1)].iconst) < INT32_MIN || (yyvsp[(1) - (1)].iconst) > INT32_MAX) {
        pwarning(1, "64-bit constant \"%"PRIi64"\" may not work in all languages.\n", (yyvsp[(1) - (1)].iconst));
      }
    }
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 592 "thrifty.yy"
    {
      pdebug("ConstValue => tok_dub_constant");
      (yyval.tconstv) = new t_const_value();
      (yyval.tconstv)->set_double((yyvsp[(1) - (1)].dconst));
    }
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 598 "thrifty.yy"
    {
      pdebug("ConstValue => tok_literal");
      (yyval.tconstv) = new t_const_value((yyvsp[(1) - (1)].id));
    }
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 603 "thrifty.yy"
    {
      pdebug("ConstValue => tok_identifier");
      t_const* constant = g_scope->get_constant((yyvsp[(1) - (1)].id));
      if (constant != NULL) {
        (yyval.tconstv) = constant->get_value();
      } else {
        if (g_parse_mode == PROGRAM) {
          pwarning(1, "Constant strings should be quoted: %s\n", (yyvsp[(1) - (1)].id));
        }
        (yyval.tconstv) = new t_const_value((yyvsp[(1) - (1)].id));
      }
    }
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 616 "thrifty.yy"
    {
      pdebug("ConstValue => ConstList");
      (yyval.tconstv) = (yyvsp[(1) - (1)].tconstv);
    }
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 621 "thrifty.yy"
    {
      pdebug("ConstValue => ConstMap");
      (yyval.tconstv) = (yyvsp[(1) - (1)].tconstv);
    }
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 628 "thrifty.yy"
    {
      pdebug("ConstList => [ ConstListContents ]");
      (yyval.tconstv) = (yyvsp[(2) - (3)].tconstv);
    }
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 635 "thrifty.yy"
    {
      pdebug("ConstListContents => ConstListContents ConstValue CommaOrSemicolonOptional");
      (yyval.tconstv) = (yyvsp[(1) - (3)].tconstv);
      (yyval.tconstv)->add_list((yyvsp[(2) - (3)].tconstv));
    }
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 641 "thrifty.yy"
    {
      pdebug("ConstListContents =>");
      (yyval.tconstv) = new t_const_value();
      (yyval.tconstv)->set_list();
    }
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 649 "thrifty.yy"
    {
      pdebug("ConstMap => { ConstMapContents }");
      (yyval.tconstv) = (yyvsp[(2) - (3)].tconstv);
    }
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 656 "thrifty.yy"
    {
      pdebug("ConstMapContents => ConstMapContents ConstValue CommaOrSemicolonOptional");
      (yyval.tconstv) = (yyvsp[(1) - (5)].tconstv);
      (yyval.tconstv)->add_map((yyvsp[(2) - (5)].tconstv), (yyvsp[(4) - (5)].tconstv));
    }
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 662 "thrifty.yy"
    {
      pdebug("ConstMapContents =>");
      (yyval.tconstv) = new t_const_value();
      (yyval.tconstv)->set_map();
    }
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 670 "thrifty.yy"
    {
      pdebug("Struct -> tok_struct tok_identifier { FieldList }");
      (yyvsp[(5) - (7)].tstruct)->set_xsd_all((yyvsp[(3) - (7)].tbool));
      (yyval.tstruct) = (yyvsp[(5) - (7)].tstruct);
      (yyval.tstruct)->set_name((yyvsp[(2) - (7)].id));
      if ((yyvsp[(7) - (7)].ttype) != NULL) {
        (yyval.tstruct)->annotations_ = (yyvsp[(7) - (7)].ttype)->annotations_;
        delete (yyvsp[(7) - (7)].ttype);
      }
    }
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 683 "thrifty.yy"
    {
      (yyval.tbool) = true;
    }
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 687 "thrifty.yy"
    {
      (yyval.tbool) = false;
    }
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 693 "thrifty.yy"
    {
      (yyval.tbool) = true;
    }
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 697 "thrifty.yy"
    {
      (yyval.tbool) = false;
    }
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 703 "thrifty.yy"
    {
      (yyval.tbool) = true;
    }
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 707 "thrifty.yy"
    {
      (yyval.tbool) = false;
    }
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 713 "thrifty.yy"
    {
      (yyval.tstruct) = (yyvsp[(3) - (4)].tstruct);
    }
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 717 "thrifty.yy"
    {
      (yyval.tstruct) = NULL;
    }
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 723 "thrifty.yy"
    {
      pdebug("Xception -> tok_xception tok_identifier { FieldList }");
      (yyvsp[(4) - (5)].tstruct)->set_name((yyvsp[(2) - (5)].id));
      (yyvsp[(4) - (5)].tstruct)->set_xception(true);
      (yyval.tstruct) = (yyvsp[(4) - (5)].tstruct);
    }
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 732 "thrifty.yy"
    {
      pdebug("Service -> tok_service tok_identifier { FunctionList }");
      (yyval.tservice) = (yyvsp[(6) - (8)].tservice);
      (yyval.tservice)->set_name((yyvsp[(2) - (8)].id));
      (yyval.tservice)->set_extends((yyvsp[(3) - (8)].tservice));
    }
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 740 "thrifty.yy"
    {
       g_arglist = 1;
    }
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 745 "thrifty.yy"
    {
       g_arglist = 0;
    }
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 751 "thrifty.yy"
    {
      pdebug("Extends -> tok_extends tok_identifier");
      (yyval.tservice) = NULL;
      if (g_parse_mode == PROGRAM) {
        (yyval.tservice) = g_scope->get_service((yyvsp[(2) - (2)].id));
        if ((yyval.tservice) == NULL) {
          yyerror("Service \"%s\" has not been defined.", (yyvsp[(2) - (2)].id));
          exit(1);
        }
      }
    }
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 763 "thrifty.yy"
    {
      (yyval.tservice) = NULL;
    }
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 769 "thrifty.yy"
    {
      pdebug("FunctionList -> FunctionList Function");
      (yyval.tservice) = (yyvsp[(1) - (2)].tservice);
      (yyvsp[(1) - (2)].tservice)->add_function((yyvsp[(2) - (2)].tfunction));
    }
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 775 "thrifty.yy"
    {
      pdebug("FunctionList -> ");
      (yyval.tservice) = new t_service(g_program);
    }
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 782 "thrifty.yy"
    {
      (yyvsp[(6) - (9)].tstruct)->set_name(std::string((yyvsp[(4) - (9)].id)) + "_args");
      (yyval.tfunction) = new t_function((yyvsp[(3) - (9)].ttype), (yyvsp[(4) - (9)].id), (yyvsp[(6) - (9)].tstruct), (yyvsp[(8) - (9)].tstruct), (yyvsp[(2) - (9)].tbool));
      if ((yyvsp[(1) - (9)].dtext) != NULL) {
        (yyval.tfunction)->set_doc((yyvsp[(1) - (9)].dtext));
      }
    }
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 792 "thrifty.yy"
    {
      (yyval.tbool) = true;
    }
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 796 "thrifty.yy"
    {
      (yyval.tbool) = false;
    }
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 802 "thrifty.yy"
    {
      pdebug("Throws -> tok_throws ( FieldList )");
      (yyval.tstruct) = (yyvsp[(3) - (4)].tstruct);
      if (g_parse_mode == PROGRAM && !validate_throws((yyval.tstruct))) {
        yyerror("Throws clause may not contain non-exception types");
        exit(1);
      }
    }
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 811 "thrifty.yy"
    {
      (yyval.tstruct) = new t_struct(g_program);
    }
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 817 "thrifty.yy"
    {
      pdebug("FieldList -> FieldList , Field");
      (yyval.tstruct) = (yyvsp[(1) - (2)].tstruct);
      if (!((yyval.tstruct)->validate_field((yyvsp[(2) - (2)].tfield)))) {
        yyerror("Field identifier %d for \"%s\" has already been used", (yyvsp[(2) - (2)].tfield)->get_key(), (yyvsp[(2) - (2)].tfield)->get_name().c_str());
        exit(1);
      }
      (yyval.tstruct)->append((yyvsp[(2) - (2)].tfield));
    }
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 827 "thrifty.yy"
    {
      pdebug("FieldList -> ");
      y_field_val = -1;
      (yyval.tstruct) = new t_struct(g_program);
    }
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 835 "thrifty.yy"
    {
      pdebug("tok_int_constant : Field -> FieldType tok_identifier");
      if ((yyvsp[(2) - (10)].iconst) < 0) {
        pwarning(2, "No field key specified for %s, resulting protocol may have conflicts or not be backwards compatible!\n", (yyvsp[(5) - (10)].id));
      }
      (yyval.tfield) = new t_field((yyvsp[(4) - (10)].ttype), (yyvsp[(5) - (10)].id), (yyvsp[(2) - (10)].iconst));
      (yyval.tfield)->set_req((yyvsp[(3) - (10)].ereq));
      if ((yyvsp[(6) - (10)].tconstv) != NULL) {
        validate_field_value((yyval.tfield), (yyvsp[(6) - (10)].tconstv));
        (yyval.tfield)->set_value((yyvsp[(6) - (10)].tconstv));
      }
      (yyval.tfield)->set_xsd_optional((yyvsp[(7) - (10)].tbool));
      (yyval.tfield)->set_xsd_nillable((yyvsp[(8) - (10)].tbool));
      if ((yyvsp[(1) - (10)].dtext) != NULL) {
        (yyval.tfield)->set_doc((yyvsp[(1) - (10)].dtext));
      }
      if ((yyvsp[(9) - (10)].tstruct) != NULL) {
        (yyval.tfield)->set_xsd_attrs((yyvsp[(9) - (10)].tstruct));
      }
    }
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 858 "thrifty.yy"
    {
      if ((yyvsp[(1) - (2)].iconst) <= 0) {
        pwarning(1, "Nonpositive value (%d) not allowed as a field key.\n", (yyvsp[(1) - (2)].iconst));
        (yyvsp[(1) - (2)].iconst) = y_field_val--;
      }
      (yyval.iconst) = (yyvsp[(1) - (2)].iconst);
    }
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 866 "thrifty.yy"
    {
      (yyval.iconst) = y_field_val--;
    }
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 872 "thrifty.yy"
    {
      if (g_arglist) {
        if (g_parse_mode == PROGRAM) {
          pwarning(1, "required keyword is ignored in argument lists.\n");
        }
        (yyval.ereq) = t_field::T_OPT_IN_REQ_OUT;
      } else {
        (yyval.ereq) = t_field::T_REQUIRED;
      }
    }
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 883 "thrifty.yy"
    {
      if (g_arglist) {
        if (g_parse_mode == PROGRAM) {
          pwarning(1, "optional keyword is ignored in argument lists.\n");
        }
        (yyval.ereq) = t_field::T_OPT_IN_REQ_OUT;
      } else {
        (yyval.ereq) = t_field::T_OPTIONAL;
      }
    }
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 894 "thrifty.yy"
    {
      (yyval.ereq) = t_field::T_OPT_IN_REQ_OUT;
    }
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 900 "thrifty.yy"
    {
      if (g_parse_mode == PROGRAM) {
        (yyval.tconstv) = (yyvsp[(2) - (2)].tconstv);
      } else {
        (yyval.tconstv) = NULL;
      }
    }
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 908 "thrifty.yy"
    {
      (yyval.tconstv) = NULL;
    }
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 914 "thrifty.yy"
    {
      pdebug("DefinitionType -> BaseType");
      (yyval.ttype) = (yyvsp[(1) - (1)].ttype);
    }
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 919 "thrifty.yy"
    {
      pdebug("DefinitionType -> ContainerType");
      (yyval.ttype) = (yyvsp[(1) - (1)].ttype);
    }
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 926 "thrifty.yy"
    {
      pdebug("FunctionType -> FieldType");
      (yyval.ttype) = (yyvsp[(1) - (1)].ttype);
    }
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 931 "thrifty.yy"
    {
      pdebug("FunctionType -> tok_void");
      (yyval.ttype) = g_type_void;
    }
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 938 "thrifty.yy"
    {
      pdebug("FieldType -> tok_identifier");
      if (g_parse_mode == INCLUDES) {
        // Ignore identifiers in include mode
        (yyval.ttype) = NULL;
      } else {
        // Lookup the identifier in the current scope
        (yyval.ttype) = g_scope->get_type((yyvsp[(1) - (1)].id));
        if ((yyval.ttype) == NULL) {
          yyerror("Type \"%s\" has not been defined.", (yyvsp[(1) - (1)].id));
          exit(1);
        }
      }
    }
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 953 "thrifty.yy"
    {
      pdebug("FieldType -> BaseType");
      (yyval.ttype) = (yyvsp[(1) - (1)].ttype);
    }
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 958 "thrifty.yy"
    {
      pdebug("FieldType -> ContainerType");
      (yyval.ttype) = (yyvsp[(1) - (1)].ttype);
    }
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 965 "thrifty.yy"
    {
      pdebug("BaseType -> tok_string");
      (yyval.ttype) = g_type_string;
    }
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 970 "thrifty.yy"
    {
      pdebug("BaseType -> tok_binary");
      (yyval.ttype) = g_type_binary;
    }
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 975 "thrifty.yy"
    {
      pdebug("BaseType -> tok_slist");
      (yyval.ttype) = g_type_slist;
    }
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 980 "thrifty.yy"
    {
      pdebug("BaseType -> tok_bool");
      (yyval.ttype) = g_type_bool;
    }
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 985 "thrifty.yy"
    {
      pdebug("BaseType -> tok_byte");
      (yyval.ttype) = g_type_byte;
    }
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 990 "thrifty.yy"
    {
      pdebug("BaseType -> tok_i16");
      (yyval.ttype) = g_type_i16;
    }
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 995 "thrifty.yy"
    {
      pdebug("BaseType -> tok_i32");
      (yyval.ttype) = g_type_i32;
    }
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 1000 "thrifty.yy"
    {
      pdebug("BaseType -> tok_i64");
      (yyval.ttype) = g_type_i64;
    }
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 1005 "thrifty.yy"
    {
      pdebug("BaseType -> tok_double");
      (yyval.ttype) = g_type_double;
    }
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 1011 "thrifty.yy"
    {
      pdebug("ContainerType -> SimpleContainerType TypeAnnotations");
      (yyval.ttype) = (yyvsp[(1) - (2)].ttype);
      if ((yyvsp[(2) - (2)].ttype) != NULL) {
        (yyval.ttype)->annotations_ = (yyvsp[(2) - (2)].ttype)->annotations_;
        delete (yyvsp[(2) - (2)].ttype);
      }
    }
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 1022 "thrifty.yy"
    {
      pdebug("SimpleContainerType -> MapType");
      (yyval.ttype) = (yyvsp[(1) - (1)].ttype);
    }
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 1027 "thrifty.yy"
    {
      pdebug("SimpleContainerType -> SetType");
      (yyval.ttype) = (yyvsp[(1) - (1)].ttype);
    }
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 1032 "thrifty.yy"
    {
      pdebug("SimpleContainerType -> ListType");
      (yyval.ttype) = (yyvsp[(1) - (1)].ttype);
    }
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 1039 "thrifty.yy"
    {
      pdebug("MapType -> tok_map <FieldType, FieldType>");
      (yyval.ttype) = new t_map((yyvsp[(4) - (7)].ttype), (yyvsp[(6) - (7)].ttype));
      if ((yyvsp[(2) - (7)].id) != NULL) {
        ((t_container*)(yyval.ttype))->set_cpp_name(std::string((yyvsp[(2) - (7)].id)));
      }
    }
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 1049 "thrifty.yy"
    {
      pdebug("SetType -> tok_set<FieldType>");
      (yyval.ttype) = new t_set((yyvsp[(4) - (5)].ttype));
      if ((yyvsp[(2) - (5)].id) != NULL) {
        ((t_container*)(yyval.ttype))->set_cpp_name(std::string((yyvsp[(2) - (5)].id)));
      }
    }
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 1059 "thrifty.yy"
    {
      pdebug("ListType -> tok_list<FieldType>");
      (yyval.ttype) = new t_list((yyvsp[(3) - (5)].ttype));
      if ((yyvsp[(5) - (5)].id) != NULL) {
        ((t_container*)(yyval.ttype))->set_cpp_name(std::string((yyvsp[(5) - (5)].id)));
      }
    }
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 1069 "thrifty.yy"
    {
      (yyval.id) = (yyvsp[(2) - (2)].id);
    }
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 1073 "thrifty.yy"
    {
      (yyval.id) = NULL;
    }
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 1079 "thrifty.yy"
    {
      pdebug("TypeAnnotations -> ( TypeAnnotationList )");
      (yyval.ttype) = (yyvsp[(2) - (3)].ttype);
    }
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 1084 "thrifty.yy"
    {
      (yyval.ttype) = NULL;
    }
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 1090 "thrifty.yy"
    {
      pdebug("TypeAnnotationList -> TypeAnnotationList , TypeAnnotation");
      (yyval.ttype) = (yyvsp[(1) - (2)].ttype);
      (yyval.ttype)->annotations_[(yyvsp[(2) - (2)].tannot)->key] = (yyvsp[(2) - (2)].tannot)->val;
      delete (yyvsp[(2) - (2)].tannot);
    }
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 1097 "thrifty.yy"
    {
      /* Just use a dummy structure to hold the annotations. */
      (yyval.ttype) = new t_struct(g_program);
    }
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 1104 "thrifty.yy"
    {
      pdebug("TypeAnnotation -> tok_identifier = tok_literal");
      (yyval.tannot) = new t_annotation;
      (yyval.tannot)->key = (yyvsp[(1) - (4)].id);
      (yyval.tannot)->val = (yyvsp[(3) - (4)].id);
    }
    break;



/* Line 1455 of yacc.c  */
#line 3052 "thrifty.cc"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 1111 "thrifty.yy"


