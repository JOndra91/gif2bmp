/**
 * File: colorTable.cpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#include "colorTable.hpp"

using namespace gif;


ColorTable::ColorTable(IReader *reader, size_t size) {
  m_tableSize = size;

  m_table = new RGBColor[m_tableSize];
  reader->allocate(sizeof(RGB8ColorPacked) * m_tableSize);

  const RGB8ColorPacked *cp
    = reinterpret_cast<const RGB8ColorPacked*>(reader->buffer());

  RGBColor *end = m_table + m_tableSize;
  for(RGBColor *c = m_table; c != end; c++, cp++) {
    *c = cp->unpackRGB();
  }

  reader->consume(sizeof(RGB8ColorPacked) * m_tableSize);
}

ColorTable::~ColorTable() {
  delete m_table;
}

GlobalColorTable::GlobalColorTable(IReader *reader, LogicalScreenDescriptor *descriptor)
  : ColorTable (reader, descriptor->getColorTableSize()) {
    m_backgroundIndex = descriptor->getBackgroundColorIndex();
}
