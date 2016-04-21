/**
 * File: canvas.hpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/20
 */

#pragma once

namespace image {

  template <class Color>
  class Canvas {

    unsigned m_width;
    unsigned m_height;
    Color *m_bitmap;

  public:

    Canvas(unsigned width, unsigned height)
      : m_width(width), m_height(height) {
      m_bitmap = new Color[m_width * m_height];
    }

    ~Canvas() {
      delete[] m_bitmap;
    }

    inline Color* getColor(unsigned x, unsigned y) {
      return m_bitmap + x + m_width * y;
    }

    inline void setColor(Color c, unsigned x, unsigned y) {
      *getColor(x, y) = c;
    }

    inline void setColor(Color c, unsigned index) {
      *getColor(index % m_width, index / m_width) = c;
    }

    inline unsigned getWidth() {
      return m_width;
    }

    inline unsigned getHeight() {
      return m_height;
    }

    void drawAt(Canvas *src, unsigned top, unsigned left) {

      for(unsigned y = 0; y < src->m_height; ++y) {
        Color *srcC = src->getColor(0, y);
        Color *dstC = getColor(left, y + top);

        for(unsigned x = 0; x < src->m_width; ++x) {
          dstC[x] = srcC[x];
        }
      }
    }

    void fillColor(Color c) {
      for(unsigned y = 0; y < m_height; ++y) {
        Color *ptr = getColor(0, y);

        for(unsigned x = 0; x < m_width; ++x) {
          ptr[x] = c;
        }
      }
    }
  };
}
