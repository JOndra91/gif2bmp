/**
 * File: fStreamReader.cpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#include "fStreamReader.hpp"

#include <cstring>

using namespace std;

FStreamReader::FStreamReader(ifstream *file) : IStreamReader(), m_file(file) {}

size_t FStreamReader::read(uint8_t *buffer, size_t size) {
  m_file->read(reinterpret_cast<char *>(buffer), size);
  return m_file->gcount();
}

void FStreamReader::ignore(size_t size) {
  m_file->ignore(size);
}
