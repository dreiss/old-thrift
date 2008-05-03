// NOTICE!!!
// DO NOT FORGET to run regen.sh if you change this file
// (or if you change the compiler).


/*

Design Document for TRobustOfflineStream

TRobustOfflineStream is a Thrift transport that encodes data in a format
suitable for storage in a file (not synchronous communication).

TRobustOfflineStream achieves following design goals:
- Be self-describing and extensible.  A file containing a TRobustOfflineStream
  must contain enough metadata for an application to read it with no other
  context.  It should be possible to add new features without breaking
  backwards and forwards compatibility.  It should be possible to completely
  change the format without confusing old or programs.
- Be robust against disk corruption.  All data and metadata must (optionally)
  be checksummed.  It must be possible to recover and continue reading
  uncorrupted data after corruption is encountered.
- Be (optionally) human-readable.  TRobustOfflineStream will also be used for
  plan-text, line-oriented, human-readable data.  Allowing a plain-text,
  line-oriented, human-readable header format will be advantageous for this
  use case.
- Support asynchronous file I/O.  This feature will not be implemented in the
  first version of TRobustOfflineStream, but the implementation must support
  the eventual inclusion of this feature.
- Be performant.  No significant sacrifice of speed should be made in order to
  achieve any of the other design goals.

TRobustOfflineStream will not do any I/O itself, but will instead focus on
preparing the data format and depend on an underlying transport (TFDTransport,
for example) to write the data to a file.

TRobustOfflineStream will have two distinct formats: binary and plain text.

Binary-format streams shall begin with a format version number, encoded as a
32-bit big-endian integer.  The version number must not exceed 2^24-1, so the
first byte of a TRobustOfflineStream will always be 0. The version number
shall be repeated once to guard against corruption.  If the two copies of the
version number do not match, the stream must be considered corrupt, and
recovery should proceed as described below (TODO).

Plain-text streams shall begin with the string ASCII "TROS: " (that is a space
after the colon), followed by the decimal form of the version number
(ASCII-encoded), followed by a linefeed (ASCII 0x0a) character.  The full
version line shall be repeated.

This document describes version 1 of the format.  Version 1 streams are
composed of series of chunks.  Variable-length chunks are supported, but their
use is discoraged because they make recovering from corrupt chunk headers
difficult.  Each chunk begins with the redundant version identifiers described
above.

Following the version numbers, a binary-format stream shall contain the
following fields, in order and with no padding:
- The (32-bit) CRC-32 of the header length + header data.
- The 32-bit big endian header length.
- A variable-length header, which is a TBinaryProtocol-serialized Thrift
  structure (whose exact structure is defined in
  robust_offline_stream.thrift).

A plain-text stream should follow the versions with:
- The string "Header-Checksum: "
- The eight-character (leading-zero-padded) hexadecimal encoding of the
  unsigned CRC-32 of the header (which does *not* include the CRC-32).
- A linefeed (0x0a).
- A header consisting of zero or more entries, where each entry consists of
  - An entry name, which is an ASCII string consisting of alphanumeric
    characters, dashes ("-"), underscores, and periods (full-stops).
  - A colon followed by a space.
  - An entry value, which is a printable ASCII string not including any
    linefeeds.
  - A linefeed.
- A linefeed.

Header entry names may be repeated.  The handling of repeated names is
dependent on the particular name.  Unless otherwise specified, all entries
with a given name other than the last are ignored.

The actual data will be stored in sub-chunks, which may optionally be
compressed.  (The chunk header will define the compression format used.)  The
chunk header will specify the following fields for each sub-chunk:
 - (optional) Offset within the chunk.  If ommitted, it should be assumed to
   immediately follow the previous sub-chunk.
 - (required) Length of the (optionally) compressed sub-chunk.  This is the
   physical number of bytes in the stream taken up by the sub-cunk.
 - (optional) Uncompressed length of the sub-chunk.  Used as an optimization
   hint.
 - (optional) CRC-32 of the (optionally compressed) sub-chunk.
 - (optional) CRC-32 of the uncompressed sub-chunk.

If no compression format is specified, the sub-chunks should be assumed to be
in "raw" format.

*/


namespace cpp    facebook.thrift.transport.robust_offline_stream
namespace java   com.facebook.thrift.transport.robust_offline_stream
namespace python thrift.transport.robust_offline_stream


/*
 * enums in plain-text headers should be represented as strings, not numbers.
 * Each enum value should specify the string used in plain text.
 */

enum CompressionType {
  /**
   * "raw": No compression.
   *
   * The data written to the TRobustOfflineStream object appears byte-for-byte
   * in the stream.  Raw format streams ignore the uncompressed length and
   * uncompressed checksum of the sub-chunks.  It is strongly advised to use
   * checksums when writing raw sub-chunks.
   */
  COMPRESSION_RAW = 0,

  /**
   * "zlib": zlib compression.
   *
   * The compressed data is a zlib stream compressed with the "deflate"
   * algorithm.  This format is specified by RFCs 1950 and 1951, and is
   * produced by zlib's "compress" or "deflate" functions.  Note that this is
   * *not* a raw "deflate" stream nor a gzip file.
   */
  COMPRESSION_ZLIB = 1,
}

enum RecordType {
  /**
   * (Absent in plain text.) Unspecified record type.
   */
  RECORD_UNKNOWN = 0,

  /**
   * "struct": Thrift structures, serialized back-to-back.
   */
  RECORD_STRUCT = 1,

  /**
   * "call": Thrift method calls, produced by send_method();
   */
  RECORD_CALL = 2,

  /**
   * "lines": Line-oriented text data.
   */
  RECORD_LINES = 3,
}

enum ProtocolType {
  /** (Absent in plain text.) */
  PROTOCOL_UNKNOWN     = 0;
  /** "binary" */
  PROTOCOL_BINARY      = 1;
  /** "dense" */
  PROTOCOL_DENSE       = 2;
  /** "json" */
  PROTOCOL_JSON        = 3;
  /** "simple_json" */
  PROTOCOL_SIMPLE_JSON = 4;
  /** "csv" */
  PROTOCOL_CSV         = 5;
}

/**
 * The structure used to represent metadata about a sub-chunk.
 * In plain text, this structure is included as the value of a "Sub-Chunk"
 * header entry.  Each of these fields should be included, represented
 * according to the comment for ChunkHeader.  Fields should be in order and
 * separated by a single space.  Absent fields should be included as a single
 * dash ("-").
 */
struct SubChunkHeader {
  1: optional i32 offset;
  2: required i32 length;
  3: optional i32 checksum;
  4: optional i32 uncompressed_length;
  5: optional i32 uncompressed_checksum;
}

/**
 * This is the top-level structure encoded as the chunk header.
 * Unless otherwise specified, field will be represented in plain text by
 * uppercasing each word in the field name and replacing underscores with
 * hyphens, producing the field name.  Integers should be ASCII-encoded
 * decimal, except for checksums which should be ASCII-encoded hexadecimal
 * unsigned.
 */
struct ChunkHeader {
  /**
   * Number of bytes per chunk.
   * Recommended to be a power of 2.
   */
  1: required i32 chunk_size;

  /**
   * Type of compression used for sub-chunks.
   * Assumed to be RAW if absent.
   */
  3: optional CompressionType compression_type = COMPRESSION_RAW;

  /**
   * Type of records encoded in the sub-chunks.
   * This information is made accessible to applications,
   * but is otherwise uninterpreted by the transport.
   */
  4: optional RecordType record_type = RECORD_UNKNOWN;

  /**
   * Protocol used for serializing records.
   * This information is made accessible to applications,
   * but is otherwise uninterpreted by the transport.
   */
  5: optional ProtocolType protocol_type = PROTOCOL_UNKNOWN;

  /**
   * The metadata for the individual sub-chunks,
   * in the order they should be read.
   *
   * In the plain-text format, each of these is written as a separate
   * "Sub-Chunk" header entry, in order.
   */
  2: required list<SubChunkHeader> sub_chunk_headers;
}
