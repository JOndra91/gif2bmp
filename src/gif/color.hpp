/**
 * File: color.hpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#pragma once

#include <cstdint>
#include "../utils/macros.h"
#include "../image/color.hpp"

using namespace image;

namespace gif {

  struct ColorPacked {
    uint8_t r;
    uint8_t g;
    uint8_t b;
  } PACKED;

}
