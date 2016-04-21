/**
 * File: fStreamReader.hpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#pragma once

#include "iStreamReader.hpp"
#include <fstream>

class FStreamReader : IStreamReader {

  std::ifstream *m_file;

  virtual size_t read(uint8_t* buffer, size_t size) override;
  virtual void ignore(size_t size) override;

public:

  FStreamReader(std::ifstream *file);

};
