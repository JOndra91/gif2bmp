/**
 * File: imageData.cpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/20
 */

#include "imageData.hpp"

using namespace gif;

ImageData::ImageData(IReader *reader) : m_reader(reader) {
  m_bufferHead = m_bufferTail = m_buffer;
};

void ImageData::skip() {
  m_reader->allocate(512);
  m_reader->consume(1);

  unsigned blockSize;
  while((blockSize = m_reader->readByte())) {
    m_reader->consume(blockSize);
    if(m_reader->allocated() == 0) {
      m_reader->allocate(512);
    }
  }
}

int ImageData::decompress() {

  loadBlock();
  assert(m_subBlockSize > 0);

  m_bufferHead = m_bufferTail = m_buffer;

  int *limit = m_buffer + m_bufferLimit;
  while(m_bufferTail < limit) {
    unsigned code = nextCode();

    if(code == m_lzwClearCode) {
      clearTable();
      continue;
    }
    else if(code == m_lzwExitCode) {
      assert(m_subBlockSize == 0);
      assert(m_reader->readByte() == 0);
      *(m_bufferTail++) = -1;
      break;
    }

    if(m_prevEntry == NULL) {
      assert(code < m_lzwTableSize);
      m_prevEntry = m_lzwTable + code;
      *(m_bufferTail++) = m_prevEntry->value;
      continue;
    }

    LZWEntry *e;
    if(code < m_lzwTableSize) {
      e = m_lzwTable + code;

      do {
        *m_bufferTail = e->value;
        m_bufferTail++;
      } while ((e = e->prev) != NULL);

      e = m_lzwTable + code;

      addTableEntry(m_lzwTable[code].value);

      m_prevEntry = e;

    }
    else {
      e = m_prevEntry;

      do {
        *m_bufferTail = e->value;
        m_bufferTail++;
      } while ((e = e->prev) != NULL);

      addTableEntry(m_prevEntry->value);
    }

  };

  return m_bufferTail - m_bufferHead;
}

void ImageData::clearTable() {

  m_lzwCodeSize = m_lzwMinimumCodeSize + 1;
  m_lzwClearCode = 1 << m_lzwMinimumCodeSize;
  m_lzwExitCode = m_lzwClearCode + 1;
  m_lzwTableLimit = 1 << m_lzwCodeSize;
  m_lzwTableSize = m_lzwExitCode + 1;

  for(unsigned i = 0; i < m_lzwClearCode; ++i) {
    m_lzwTable[i] = { .value=i, .prev=NULL };
  }
}
