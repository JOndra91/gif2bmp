/**
 * File: logicalScreenDescriptor.hpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#pragma once

#include "iReader.hpp"
#include <cstdint>

namespace gif {

  union LogicalScreenDescriptorPackedField {
    struct {
      unsigned colorTableSize : 3;
      bool sortFlag : 1;
      unsigned colorResolution : 3;
      bool colorTableFlag : 1;
    };
    uint8_t packedValue;
  };

  class LogicalScreenDescriptor {

    unsigned m_width;
    unsigned m_height;
    unsigned m_backgroundIndex;
    unsigned m_pixelAspectRatio;

    bool m_colorTableFlag;
    bool m_sortFlag;
    unsigned m_colorResolution;
    unsigned m_colorTableSize;

  public:

    LogicalScreenDescriptor(IReader *reader);

    inline unsigned getWidth() {
      return m_width;
    }

    inline unsigned getHeight() {
      return m_height;
    }

    inline unsigned getBackgroundColorIndex() {
      return m_backgroundIndex;
    }

    inline unsigned getPixelAspectRatio() {
      return m_pixelAspectRatio;
    }

    inline bool hasColorTable() {
      return m_colorTableFlag;
    }

    inline unsigned getColorTableSize() {
      return m_colorTableSize;
    }

    inline unsigned getColorResolution() {
      return m_colorResolution;
    }

    inline bool isColorTableOrdered() {
      return m_sortFlag;
    }

  };

}
