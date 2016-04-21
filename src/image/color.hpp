/**
 * File: color.hpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#pragma once

#include <cstdint>
#include "../utils/macros.h"

namespace image {

  struct RGBColor {

    RGBColor() : r(0), g(0), b(0) {};

    RGBColor(unsigned red, unsigned green, unsigned blue)
      : r(red), g(green), b(blue) {};

    RGBColor(const RGBColor &c) {
      r = c.r;
      g = c.g;
      b = c.b;
    };

    unsigned r;
    unsigned g;
    unsigned b;
  };

}
