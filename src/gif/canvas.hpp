/**
 * File: canvas.hpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/20
 */

#pragma once

#include "color.hpp"

namespace gif {

  class Canvas {

    unsigned m_width;
    unsigned m_height;
    Color *m_bitmap;

  public:

    Canvas(unsigned width, unsigned height);
    ~Canvas();

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

    void drawAt(Canvas *src, unsigned top, unsigned left);
    void fillColor(Color c);

  };

}
