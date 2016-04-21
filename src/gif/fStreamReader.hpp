/**
 * File: fStreamReader.hpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#pragma once

#include "iReader.hpp"
#include <fstream>

class FStreamReader : IReader {

  std::ifstream *m_file;

public:

  FStreamReader(std::ifstream *file);

  virtual void _allocate(size_t) override;
  virtual void _consume(size_t) override;

};
