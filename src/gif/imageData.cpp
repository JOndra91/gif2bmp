/**
 * File: imageData.cpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/20
 */

#include "imageData.hpp"

using namespace gif;

ImageData::ImageData(IReader *reader) : m_reader(reader) {
  assert(m_buffer.empty());
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

  while(m_buffer.size() < m_bufferLimit) {
    unsigned code = nextCode();

    if(code == m_lzwClearCode) {
      clearTable();
      continue;
    }
    else if(code == m_lzwExitCode) {
      assert(m_subBlockSize == 0);
      assert(m_reader->readByte() == 0);
      m_buffer.push_back(-1);
      break;
    }

    if(m_prevEntry == NULL) {
      assert(code < m_lzwTableSize);
      m_prevEntry = m_lzwTable + code;
      m_buffer.push_back(m_prevEntry->value);
      continue;
    }

    LZWEntry *e, *f;
    std::deque<int> v;
    if(code < m_lzwTableSize) {
      e = m_lzwTable + code;

      do {
        f = e;
        v.push_back(e->value);
      } while ((e = e->prev) != NULL);

      addTableEntry(f->value);

      m_prevEntry = m_lzwTable + code;

    }
    else {
      e = m_prevEntry;

      do {
        f = e;
        v.push_back(e->value);
      } while ((e = e->prev) != NULL);

      v.push_front(f->value);

      m_prevEntry = addTableEntry(f->value);
    }

    for(auto it = v.rbegin(); it != v.rend(); ++it) {
      m_buffer.push_back(*it);
    }

  };

  return m_buffer.size();
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
