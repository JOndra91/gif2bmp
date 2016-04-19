/**
 * File: fileReader.cpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#include "fileReader.hpp"

#include <cstring>

using namespace std;

#define INITIAL_BUFFER_SIZE 262144 // 256 kB

FileReader::FileReader(ifstream *file) {
  m_bufferSize = INITIAL_BUFFER_SIZE;
  m_buffer = new uint8_t[m_bufferSize];
  m_bufferHead = m_buffer;
  m_allocated = 0;

  m_file = file;
}

void FileReader::_allocate(size_t size) {
  size_t required;

  if(size < available()) {
    size = available();
  }

  required = size - m_allocated;

  if(required > available()) {
    if(required > m_bufferSize) {
      m_bufferSize = required + required/2;

      uint8_t *newBuffer = new uint8_t[m_bufferSize];

      memcpy(newBuffer, m_buffer, sizeof(uint8_t) * m_allocated);

      delete m_buffer;

      m_buffer = m_bufferHead = newBuffer;
    }
    else {
      memmove(m_buffer, m_bufferHead, m_allocated);
      m_bufferHead = m_buffer + m_allocated;
    }
  }

  // WARN: char/uint8_t type conflict on istream::read()
  m_file->read(reinterpret_cast<char *>(m_bufferHead + m_allocated), required);

  m_allocated += m_file->gcount();
}

void FileReader::_consume(size_t size) {

  m_bufferHead = m_buffer;
  size = size - m_allocated;
  m_allocated = 0;

  m_file->ignore(size);
}
