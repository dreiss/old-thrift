// Copyright (c) 2006- Facebook
// Distributed under the Thrift Software License
//
// See accompanying file LICENSE or visit the Thrift site at:
// http://developers.facebook.com/thrift/

#ifndef _THRIFT_TRANSPORT_TMEMBUFFER_H_
#define _THRIFT_TRANSPORT_TMEMBUFFER_H_ 1

#include <cstdlib>
#include <string>
#include <algorithm>
#include <transport/TTransport.h>
#include <transport/TVirtualTransport.h>

namespace facebook { namespace thrift { namespace transport {

/**
 * A memory buffer is a tranpsort that simply reads from and writes to an
 * in memory buffer. Anytime you call write on it, the data is simply placed
 * into a buffer, and anytime you call read, data is read from that buffer.
 *
 * The buffers are allocated using C constructs malloc,realloc, and the size
 * doubles as necessary.
 *
 * @author Mark Slee <mcslee@facebook.com>
 * @author David Reiss <dreiss@facebook.com>
 */
template <typename CollectStats_ = boost::false_type>
class TMemoryBufferT : public TVirtualTransport<TMemoryBufferT<CollectStats_>,
                                                CollectStats_> {
 private:

  // Common initialization done by all constructors.
  void initCommon(uint8_t* buf, uint32_t size, bool owner, uint32_t wPos) {
    if (buf == NULL && size != 0) {
      assert(owner);
      buf = (uint8_t*)std::malloc(size);
      if (buf == NULL) {
        throw TTransportException("Out of memory");
      }
    }

    buffer_ = buf;
    bufferSize_ = size;
    owner_ = owner;
    wPos_ = wPos;
    rPos_ = 0;
  }

  // make sure there's at least 'len' bytes available for writing
  void ensureCanWrite(uint32_t len);

 public:
  static const uint32_t defaultSize = 1024;

  /**
   * This enum specifies how a TMemoryBuffer should treat
   * memory passed to it via constructors or resetBuffer.
   *
   * OBSERVE:
   *   TMemoryBuffer will simply store a pointer to the memory.
   *   It is the callers responsibility to ensure that the pointer
   *   remains valid for the lifetime of the TMemoryBuffer,
   *   and that it is properly cleaned up.
   *   Note that no data can be written to observed buffers.
   *
   * COPY:
   *   TMemoryBuffer will make an internal copy of the buffer.
   *   The caller has no responsibilities.
   *
   * TAKE_OWNERSHIP:
   *   TMemoryBuffer will become the "owner" of the buffer,
   *   and will be responsible for freeing it.
   *   The membory must have been allocated with malloc.
   */
  enum MemoryPolicy {
    OBSERVE = 1,
    COPY = 2,
    TAKE_OWNERSHIP = 3,
  };

  /**
   * Construct a TMemoryBuffer with a default-sized buffer,
   * owned by the TMemoryBuffer object.
   */
  TMemoryBufferT() {
    initCommon(NULL, defaultSize, true, 0);
  }

  /**
   * Construct a TMemoryBuffer with a buffer of a specified size,
   * owned by the TMemoryBuffer object.
   *
   * @param sz  The initial size of the buffer.
   */
  TMemoryBufferT(uint32_t sz) {
    initCommon(NULL, sz, true, 0);
  }

  /**
   * Construct a TMemoryBuffer with buf as its initial contents.
   *
   * @param buf    The initial contents of the buffer.
   *               Note that, while buf is a non-const pointer,
   *               TMemoryBuffer will not write to it if policy == OBSERVE,
   *               so it is safe to const_cast<uint8_t*>(whatever).
   * @param sz     The size of @c buf.
   * @param policy See @link MemoryPolicy @endlink .
   */
  TMemoryBufferT(uint8_t* buf, uint32_t sz, MemoryPolicy policy = OBSERVE) {
    if (buf == NULL && sz != 0) {
      throw TTransportException(TTransportException::BAD_ARGS,
                                "TMemoryBuffer given null buffer with non-zero size.");
    }

    switch (policy) {
      case OBSERVE:
      case TAKE_OWNERSHIP:
        initCommon(buf, sz, policy == TAKE_OWNERSHIP, sz);
        break;
      case COPY:
        initCommon(NULL, sz, true, 0);
        this->write(buf, sz);
        break;
      default:
        throw TTransportException(TTransportException::BAD_ARGS,
                                  "Invalid MemoryPolicy for TMemoryBuffer");
    }
  }

  ~TMemoryBufferT() {
    if (owner_) {
      std::free(buffer_);
      buffer_ = NULL;
    }
  }

  bool isOpen() {
    return true;
  }

  bool peek() {
    return (rPos_ < wPos_);
  }

  void open() {}

  void close() {}

  // TODO(dreiss): Make bufPtr const.
  void getBuffer(uint8_t** bufPtr, uint32_t* sz) {
    *bufPtr = buffer_;
    *sz = wPos_;
  }

  std::string getBufferAsString() {
    if (buffer_ == NULL) {
      return "";
    }
    return std::string((char*)buffer_, (std::string::size_type)wPos_);
  }

  void appendBufferToString(std::string& str) {
    if (buffer_ == NULL) {
      return;
    }
    str.append((char*)buffer_, wPos_);
  }

  void resetBuffer() {
    wPos_ = 0;
    rPos_ = 0;
    // It isn't safe to write into a buffer we don't own.
    if (!owner_) {
      bufferSize_ = 0;
    }
  }

  /// See constructor documentation.
  void resetBuffer(uint8_t* buf, uint32_t sz, MemoryPolicy policy = OBSERVE) {
    // Use a variant of the copy-and-swap trick for assignment operators.
    // This is sub-optimal in terms of performance for two reasons:
    //   1/ The constructing and swapping of the (small) values
    //      in the temporary object takes some time, and is not necessary.
    //   2/ If policy == COPY, we allocate the new buffer before
    //      freeing the old one, precluding the possibility of
    //      reusing that memory.
    // I doubt that either of these problems could be optimized away,
    // but the second is probably no a common case, and the first is minor.
    // I don't expect resetBuffer to be a common operation, so I'm willing to
    // bite the performance bullet to make the method this simple.

    // Construct the new buffer.
    TMemoryBufferT<CollectStats_> new_buffer(buf, sz, policy);
    // Move it into ourself.
    this->swap(new_buffer);
    // Our old self gets destroyed.
  }

  uint32_t read(uint8_t* buf, uint32_t len);

  std::string readAsString(uint32_t len) {
    std::string str;
    (void)readAppendToString(str, len);
    return str;
  }

  uint32_t readAppendToString(std::string& str, uint32_t len);

  void readEnd() {
    if (rPos_ == wPos_) {
      resetBuffer();
    }
  }

  void write(const uint8_t* buf, uint32_t len);

  uint32_t available() const {
    return wPos_ - rPos_;
  }

  const uint8_t* borrow(uint8_t* buf, uint32_t* len);

  void consume(uint32_t len);

  void swap(TMemoryBufferT<CollectStats_>& that) {
    using std::swap;
    swap(buffer_,     that.buffer_);
    swap(bufferSize_, that.bufferSize_);
    swap(wPos_,       that.wPos_);
    swap(owner_,      that.owner_);
  }

  // Returns a pointer to where the client can write data to append to
  // the TMemoryBuffer, and ensures the buffer is big enough to accomodate a
  // write of the provided length.  The returned pointer is very convenient for
  // passing to read(), recv(), or similar. You must call wroteBytes() as soon
  // as data is written or the buffer will not be aware that data has changed.
  uint8_t* getWritePtr(uint32_t len) {
    ensureCanWrite(len);
    return buffer_ + wPos_;
  }

  // Informs the buffer that the client has written 'len' bytes into storage
  // that had been provided by getWritePtr().
  void wroteBytes(uint32_t len);

 private:
  // Data buffer
  uint8_t* buffer_;

  // Allocated buffer size
  uint32_t bufferSize_;

  // Where the write is at
  uint32_t wPos_;

  // Where the reader is at
  uint32_t rPos_;

  // Is this object the owner of the buffer?
  bool owner_;

  // Don't forget to update constrctors, initCommon, and swap if
  // you add new members.
};

template <typename CollectStats_>
uint32_t TMemoryBufferT<CollectStats_>::read(uint8_t* buf, uint32_t len) {
  // Check avaible data for reading
  uint32_t avail = wPos_ - rPos_;
  if (avail == 0) {
    return 0;
  }

  // Decide how much to give
  uint32_t give = len;
  if (avail < len) {
    give = avail;
  }

  // Copy into buffer and increment rPos_
  memcpy(buf, buffer_ + rPos_, give);
  rPos_ += give;

  return give;
}

template <typename CollectStats_>
uint32_t TMemoryBufferT<CollectStats_>::readAppendToString(std::string& str,
                                                           uint32_t len) {
  // Don't get some stupid assertion failure.
  if (buffer_ == NULL) {
    return 0;
  }

  // Check avaible data for reading
  uint32_t avail = wPos_ - rPos_;
  if (avail == 0) {
    return 0;
  }

  // Device how much to give
  uint32_t give = len;
  if (avail < len) {
    give = avail;
  }

  // Reserve memory, copy into string, and increment rPos_
  str.reserve(str.length()+give);
  str.append((char*)buffer_ + rPos_, give);
  rPos_ += give;

  return give;
}

template <typename CollectStats_>
void TMemoryBufferT<CollectStats_>::ensureCanWrite(uint32_t len) {
  // Check available space
  uint32_t avail = bufferSize_ - wPos_;
  if (len <= avail) {
    return;
  }

  if (!owner_) {
    throw TTransportException("Insufficient space in external MemoryBuffer");
  }

  // Grow the buffer as necessary
  while (len > avail) {
    bufferSize_ *= 2;
    avail = bufferSize_ - wPos_;
  }
  buffer_ = (uint8_t*)std::realloc(buffer_, bufferSize_);
  if (buffer_ == NULL) {
    throw TTransportException("Out of memory.");
  }
}

template <typename CollectStats_>
void TMemoryBufferT<CollectStats_>::write(const uint8_t* buf, uint32_t len) {
  ensureCanWrite(len);

  // Copy into the buffer and increment wPos_
  memcpy(buffer_ + wPos_, buf, len);
  wPos_ += len;
}

template <typename CollectStats_>
void TMemoryBufferT<CollectStats_>::wroteBytes(uint32_t len) {
  uint32_t avail = bufferSize_ - wPos_;
  if (len > avail) {
    throw TTransportException("Client wrote more bytes than size of buffer.");
  }
  wPos_ += len;
}

template <typename CollectStats_>
const uint8_t* TMemoryBufferT<CollectStats_>::borrow(uint8_t* buf,
                                                     uint32_t* len) {
  if (wPos_-rPos_ >= *len) {
    *len = wPos_-rPos_;
    return buffer_ + rPos_;
  }
  return NULL;
}

template <typename CollectStats_>
void TMemoryBufferT<CollectStats_>::consume(uint32_t len) {
  if (wPos_-rPos_ >= len) {
    rPos_ += len;
  } else {
    throw TTransportException(TTransportException::BAD_ARGS,
                              "consume did not follow a borrow.");
  }
}

typedef TMemoryBufferT<> TMemoryBuffer;

}}} // facebook::thrift::transport

#endif // #ifndef _THRIFT_TRANSPORT_TMEMBUFFER_H_
