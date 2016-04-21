/**
 * File: rgbQuad.hpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/21
 */

#pragma once

#include <cstdint>
#include "../utils/macros.h"

namespace bmp {

  struct RGBQuad {
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t reserved;
  } PACKED;

}
