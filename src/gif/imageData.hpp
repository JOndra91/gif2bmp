/**
 * File: imageData.hpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/20
 */

#pragma once

#include "iReader.hpp"
#include <cstddef>
#include <cassert>
#include <deque>

namespace gif {

  struct LZWEntry {
    unsigned value;
    LZWEntry *prev;
  };

  class ImageData {

    IReader *m_reader;
    std::deque<int> m_buffer;
    static const unsigned m_bufferLimit = 256;

    int m_subBlockSize = 0;
    unsigned m_lzwMinimumCodeSize = 0;
    unsigned m_lzwCodeSize;
    unsigned m_lzwClearCode;
    unsigned m_lzwTableSize;
    unsigned m_lzwTableLimit;
    unsigned m_lzwExitCode;
    unsigned m_lzwBitValue = 0;
    unsigned m_lzwBitCount = 0;
    LZWEntry *m_prevEntry = NULL;

    LZWEntry m_lzwTable[4096]; // 2^12

    int decompress();
    void clearTable();

    inline void loadBlock() {
      if(m_subBlockSize ==  0) {

        m_reader->allocate(2);

        if(m_lzwMinimumCodeSize == 0) {
          m_lzwMinimumCodeSize = m_reader->readByte();
          clearTable();
        }

        m_subBlockSize = m_reader->readByte();
        if(m_subBlockSize > 0) {
          m_reader->allocate(m_subBlockSize);
        }
        else {
          m_subBlockSize = -1;
        }
      }
    }

    inline unsigned nextCode() {
      unsigned byte;

      while(m_lzwBitCount < m_lzwCodeSize) {
        loadBlock();
        assert(m_subBlockSize > 0);

        byte = m_reader->readByte();
        m_subBlockSize--;

        m_lzwBitValue |= byte << m_lzwBitCount;
        m_lzwBitCount += 8;
      }

      byte = m_lzwBitValue & ((1 << m_lzwCodeSize) - 1);
      m_lzwBitValue >>= m_lzwCodeSize;
      m_lzwBitCount -= m_lzwCodeSize;

      return byte;
    }

    inline LZWEntry* addTableEntry(unsigned value) {
      m_lzwTable[m_lzwTableSize] = { .value=value, .prev=m_prevEntry };
      m_lzwTableSize++;

      if(m_lzwTableSize == m_lzwTableLimit) {
        m_lzwCodeSize++;
        m_lzwTableLimit <<= 1;
      }

      return m_lzwTable + m_lzwTableSize - 1;
    }

  public:

    ImageData(IReader *reader);

    /**
     * Skips whole image data block.
     */
    void skip();

    /**
     * Returns following index to color table or -1 if no index is available.
     */
    inline int next() {
      if(m_buffer.empty()) {
        if(decompress() == 0) {
          return -1;
        }
      }

      int value = m_buffer.front();
      m_buffer.pop_front();

      return value;
    };

  };


}
