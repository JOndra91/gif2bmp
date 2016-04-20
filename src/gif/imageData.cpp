/**
 * File: imageData.cpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/20
 */

#include "imageData.hpp"

using namespace gif;


ImageData::ImageData(IReader *reader) : m_reader(reader) {};

void ImageData::skip() {
  m_reader->allocate(512);
  m_reader->consume(1);

  unsigned blockSize;
  while((blockSize = m_reader->readByte())) {
    m_reader->consume(blockSize);
    if(m_reader->allocated() == 0) {
      m_reader->allocate(512);
    }
  }
}
