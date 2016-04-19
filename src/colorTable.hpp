/**
 * File: colorTable.hpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#pragma once

#include "iReader.hpp"
#include "color.hpp"
#include "logicalScreenDescriptor.hpp"

namespace gif {

  class ColorTable {

    Color *m_table;
    unsigned m_tableSize;
    unsigned m_backgroundIndex;

  public:

    ColorTable(IReader &reader, LogicalScreenDescriptor &descriptor);
    ~ColorTable();

    inline Color getColor(unsigned index) {
      return m_table[index];
    }

    inline Color getBackground() {
      return m_table[m_backgroundIndex];
    }

  };

}
