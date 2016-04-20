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

protected:
    Color *m_table;
    unsigned m_tableSize;

  public:

    ColorTable(IReader *reader, size_t size);
    ~ColorTable();

    inline Color getColor(unsigned index) {
      return m_table[index];
    }

    inline size_t getSize() {
      return m_tableSize;
    }
  };

  class GlobalColorTable : public ColorTable {
    unsigned m_backgroundIndex;

  public:

    GlobalColorTable(IReader *reader, LogicalScreenDescriptor *descriptor);

    inline Color getBackground() {
      return m_table[m_backgroundIndex];
    }
  };

}
