/**
 * File: blocks.hpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#pragma once

namespace gif {

  enum class Block {
    Terminator = 0x00,
    ExtensionIntroducer = 0x21,
    ImageSeparator = 0x2C,
    Trailer = 0x3B,
  };

}
