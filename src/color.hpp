/**
 * File: color.hpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#pragma once

#include <cstdint>
#include "macros.h"

namespace gif {

  struct ColorPacked {
    uint8_t r;
    uint8_t g;
    uint8_t b;
  } PACKED;

  struct Color {

    Color() {
      r = g = b = 0;
    };

    Color(const Color &c) {
      r = c.r;
      g = c.g;
      b = c.b;
    };

    Color(const ColorPacked &c) {
      r = c.r;
      g = c.g;
      b = c.b;
    }

    unsigned r;
    unsigned g;
    unsigned b;
  };

}
