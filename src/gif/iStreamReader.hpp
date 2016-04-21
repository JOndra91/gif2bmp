/**
 * File: iStreamReader.hpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#pragma once

#include "iReader.hpp"
#include <fstream>

class IStreamReader : IReader {

  virtual void _allocate(size_t) override;
  virtual void _consume(size_t) override;

protected:

  IStreamReader();

  virtual size_t read(uint8_t* buffer, size_t size) = 0;
  virtual void ignore(size_t size) = 0;

};
