/**
 * File: imageDescriptor.hpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/20
 */

#pragma once

#include <cstdint>
#include "iReader.hpp"

namespace gif {

  union ImageDescriptorPackedField {
    struct {
      unsigned colorTableSize : 3;
      int reserved : 2;
      bool sortFlag : 1;
      bool interlaceFlag : 1;
      bool colorTableFlag : 1;
    };
    uint8_t packedValue;
  };

  class ImageDescriptor {

    unsigned m_left;
    unsigned m_top;
    unsigned m_width;
    unsigned m_height;

    bool m_colorTableFlag;
    bool m_interlaceFlag;
    bool m_sortFlag;
    unsigned m_colorTableSize;

  public:

    ImageDescriptor(IReader *reader);

    inline unsigned getLeft() {
      return m_left;
    };

    inline unsigned getTop() {
      return m_top;
    };

    inline unsigned getWidth() {
      return m_width;
    };

    inline unsigned getHeight() {
      return m_height;
    };

    inline bool hasColorTable() {
      return m_colorTableFlag;
    };

    inline bool isInterlaced() {
      return m_interlaceFlag;
    };

    inline bool isColorTableOrdered() {
      return m_sortFlag;
    };

    inline unsigned getColorTableSize() {
      return m_colorTableSize;
    };

  };

}
