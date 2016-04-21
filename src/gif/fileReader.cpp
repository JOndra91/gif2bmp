/**
 * File: fileReader.cpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/21
 */

#include "fileReader.hpp"

#include <cstring>

using namespace std;

FileReader::FileReader(FILE *file) : IStreamReader(), m_file(file) {}

size_t FileReader::read(uint8_t *buffer, size_t size) {
  return fread(buffer, sizeof(uint8_t), size, m_file);
}

void FileReader::ignore(size_t size) {
  fseek(m_file, size, SEEK_CUR);
}
