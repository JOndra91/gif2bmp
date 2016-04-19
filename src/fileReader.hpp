/**
 * File: fileReader.hpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#pragma once

#include "iReader.hpp"
#include <fstream>

class FileReader : IReader {

  std::ifstream m_file;

public:

  FileReader(std::ifstream file);
  ~FileReader() override;

  virtual size_t allocate(size_t) override;
  virtual size_t consume(size_t) override;

};
