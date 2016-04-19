/**
 * File: fileReader.cpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#include "fileReader.hpp"

using namespace std;

#define INITIAL_BUFFER_SIZE 262144 // 256 kB

FileReader::FileReader(ifstream file) {
  m_bufferSize = INITIAL_BUFFER_SIZE;
  m_buffer = new uint8_t[m_bufferSize];
  m_bufferHead = m_buffer;
}

FileReader::~FileReader() {
  m_file.close();
}

size_t FileReader::allocate(size_t size) {
  return 0;
}
