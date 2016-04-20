/**
 * File: canvas.cpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/20
 */

#include "canvas.hpp"

using namespace gif;

Canvas::Canvas(unsigned width, unsigned height)
  : m_width(width), m_height(height) {

  m_bitmap = new Color[m_width * m_height];

}

Canvas::~Canvas() {
  delete m_bitmap;
}

void Canvas::drawAt(Canvas *src, unsigned top, unsigned left) {

  for(unsigned y = 0; y < m_height; ++y) {
    Color *srcC = src->getColor(0, y);
    Color *dstC = getColor(left, y + top);

    for(unsigned x = 0; x < m_width; ++x) {
      dstC[x] = srcC[x];
    }
  }
}

void Canvas::fillColor(Color c) {
  for(unsigned y = 0; y < m_height; ++y) {
    Color *ptr = getColor(0, y);

    for(unsigned x = 0; x < m_width; ++x) {
      ptr[x] = c;
    }
  }
}
