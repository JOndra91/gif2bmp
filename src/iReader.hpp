/**
 * File: iReader.hpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <arpa/inet.h>

class IReader {

protected:

  uint8_t *m_buffer;
  uint8_t *m_bufferHead;
  size_t m_bufferSize;

public:

  virtual ~IReader() {
      delete m_buffer;
  };

  /**
   * Ensures that enough bytes is allocated from point of reading head.
   * Returns amount of available bytes.
   */
  virtual size_t allocate(size_t) = 0;

  /**
   * Consumes data from point of reading head.
   * May consume more data than is allocated.
   * Returns amount of bytes availabe.
   */
  virtual size_t consume(size_t) = 0;

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

    m_bufferHead += size;

    return allocated();
  };

  /**
   * Returns single byte from buffer. You have to ensure that enough bytes
   * is available using <code>allocate()</code>.
   * This operation does not consume the byte.
   */
  inline uint8_t peekByte() {
    return *m_bufferHead;
  }

  /**
   * Reads word (2 bytes) from buffer and converts it to host byte order.
   * You have to ensure that enough bytes is available using
   * <code>allocate()</code>.
   * This operation does not consume the word.
   */
  inline uint16_t peekWord() {
    uint16_t word = *((uint16_t*)m_bufferHead);
    return ntohs(word);
  };

  /**
   * Returns single byte from buffer. You have to ensure that enough bytes
   * is available using <code>allocate()</code>.
   * This operation also consumes the byte.
   */
  inline uint8_t readByte() {
    uint8_t byte = *m_bufferHead;
    m_bufferHead++;
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
    m_bufferHead += 2;
    return ntohs(word);
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
    return m_bufferSize - (m_bufferHead - m_buffer);
  }

};
