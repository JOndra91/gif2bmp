/**
 * File: extension.hpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#pragma once

#include "iReader.hpp"

namespace gif {

  enum class ExtensionLabel {
    PlainText = 0x01,
    GraphicControl = 0xF9,
    Comment = 0xFE,
    Application = 0xFF,
    None = 0xFFFF
  };

  class ExtensionDetector {

    IReader *m_reader;

  public:
    ExtensionDetector(IReader *reader);

    bool hasExtension();
    ExtensionLabel getExtensionType();
    void skipExtension();

  };

}
