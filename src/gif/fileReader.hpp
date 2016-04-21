/**
 * File: fileReader.hpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/21
 */

#pragma once

#include "iStreamReader.hpp"
#include <cstdio>

class FileReader : IStreamReader {

  std::FILE *m_file;

  virtual size_t read(uint8_t* buffer, size_t size) override;
  virtual void ignore(size_t size) override;

public:

  FileReader(std::FILE *file);

};
