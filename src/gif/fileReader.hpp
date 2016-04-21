/**
 * File: fileReader.hpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/21
 */

#pragma once

#include "iReader.hpp"
#include <cstdio>

class FileReader : IReader {

  std::FILE *m_file;

public:

  FileReader(std::FILE *file);

  virtual void _allocate(size_t) override;
  virtual void _consume(size_t) override;

};
