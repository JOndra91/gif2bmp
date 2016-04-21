/**
 * File: color.hpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#pragma once

#include <cstdint>
#include "../utils/macros.h"

namespace image {

  struct RGB8ColorPacked;
  struct BGR8ColorPacked;
  struct RGBColor;

  struct RGBColor {
    unsigned r;
    unsigned g;
    unsigned b;

    RGBColor() : r(0), g(0), b(0) {};

    RGBColor(unsigned red, unsigned green, unsigned blue)
      : r(red), g(green), b(blue) {};

    RGBColor(const RGBColor &c) {
      r = c.r;
      g = c.g;
      b = c.b;
    };

    RGB8ColorPacked packRGB8() const;
    BGR8ColorPacked packBGR8() const;

  };

  struct RGB8ColorPacked {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    RGB8ColorPacked() : r(0), g(0), b(0) {};

    RGB8ColorPacked(unsigned red, unsigned green, unsigned blue)
      : r(red), g(green), b(blue) {};

    RGB8ColorPacked(const RGB8ColorPacked &c) {
      r = c.r;
      g = c.g;
      b = c.b;
    };

    RGBColor unpackRGB() const;

  } PACKED;

  struct BGR8ColorPacked {
    uint8_t b;
    uint8_t g;
    uint8_t r;

    BGR8ColorPacked() : b(0), g(0), r(0) {};

    BGR8ColorPacked(unsigned red, unsigned green, unsigned blue)
      : b(blue), g(green), r(red) {};

    BGR8ColorPacked(const RGB8ColorPacked &c) {
      r = c.r;
      g = c.g;
      b = c.b;
    };

  } PACKED;

  inline RGB8ColorPacked RGBColor::packRGB8() const {
    return RGB8ColorPacked(r, g, b);
  }

  inline RGBColor RGB8ColorPacked::unpackRGB() const {
    return RGBColor(r, g, b);
  }

  inline BGR8ColorPacked RGBColor::packBGR8() const {
    return BGR8ColorPacked(r, g, b);
  }

}
