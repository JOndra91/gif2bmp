/**
 * File: iReader.hpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <endian.h>

class IReader {

protected:

  uint8_t *m_buffer;
  uint8_t *m_bufferHead;
  size_t m_bufferSize;
  size_t m_allocated;

  /**
   * Returns amount of unallocated bytes in buffer from point of
   * buffer head to end of buffer.
   */
  inline size_t available() {
    return m_bufferSize - (m_bufferHead - m_buffer) - m_allocated;
  };

  inline void consumeBuffer(size_t size) {
    m_bufferHead += size;
    m_allocated -= size;
  }

  /**
   * Ensures that enough bytes is allocated from point of reading head.
   * Returns amount of allocated bytes.
   */
  virtual void _allocate(size_t) = 0;

  /**
   * Consumes data from point of reading head.
   * May consume more data than is allocated.
   * Returns amount of allocated bytes.
   */
  virtual void _consume(size_t) = 0;

public:

  virtual ~IReader() {
      delete[] m_buffer;
  };

  /**
   * Ensures that enough bytes is allocated from point of reading head.
   * Returns amount of allocated bytes.
   */
  inline size_t allocate(size_t size) {
    if(size > m_allocated) {
      _allocate(size);
    }

    return m_allocated;
  };

  /**
   * Consumes data from point of reading head.
   * May consume more data than is allocated.
   * Returns amount of allocated bytes.
   */
  inline size_t consume(size_t size) {
    if(size > m_allocated) {
      _consume(size);
    }
    else {
      consumeBuffer(size);
    }

    return m_allocated;
  };

  /**
   * Copies data between buffers with type casting.
   * Returns amount of copied bytes, which is never more than is allocated
   * at the moment of method call.
   */
  template <typename T>
  size_t copy(size_t size, T *destBuffer) {

    if(size > allocated()) {
      size = allocated();
    }

    for(size_t i = 0; i < size; ++i) {
      destBuffer[i] = (T)m_bufferHead[i];
    }

    consumeBuffer(size);

    return allocated();
  };

  /**
   * Returns single byte from buffer. You have to ensure that enough bytes
   * is available using <code>allocate()</code>.
   * This operation does not consume the byte.
   */
  inline uint8_t peekByte(size_t offset = 0) {
    return m_bufferHead[offset];
  }

  /**
   * Reads word (2 bytes) from buffer and converts it to host byte order.
   * You have to ensure that enough bytes is available using
   * <code>allocate()</code>.
   * Given offset is in bytes.
   * This operation does not consume the word.
   */
  inline uint16_t peekWord(size_t offset = 0) {
    uint16_t word = *((uint16_t*)(m_bufferHead + offset));
    return le16toh(word);
  };

  /**
   * Returns single byte from buffer. You have to ensure that enough bytes
   * is available using <code>allocate()</code>.
   * This operation also consumes the byte.
   */
  inline uint8_t readByte() {
    uint8_t byte = *m_bufferHead;
    consumeBuffer(1);
    return byte;
  };

  /**
   * Reads word (2 bytes) from buffer and converts it to host byte order.
   * You have to ensure that enough bytes is available using
   * <code>allocate()</code>.
   * This operation also consumes the word.
   */
  inline uint16_t readWord() {
    uint16_t word = *((uint16_t*)m_bufferHead);
    consumeBuffer(2);
    return le16toh(word);
  };

  /**
   * Returns pointer to reading head of internal buffer.
   */
  inline const uint8_t* buffer() {
    return m_bufferHead;
  };

  /**
   * Return amount of currently allocated bytes.
   */
  inline size_t allocated() {
    return m_allocated;
  }

};
