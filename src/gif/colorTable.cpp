/**
 * File: colorTable.cpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */


#include "color.hpp"
#include "colorTable.hpp"

using namespace gif;

ColorTable::ColorTable(IReader *reader, size_t size) {
  m_tableSize = size;

  m_table = new RGBColor[m_tableSize];
  reader->allocate(sizeof(ColorPacked) * m_tableSize);

  const ColorPacked *cp = reinterpret_cast<const ColorPacked*>(reader->buffer());

  RGBColor *end = m_table + m_tableSize;
  for(RGBColor *c = m_table; c != end; c++, cp++) {
    *c = RGBColor(cp->r, cp->g, cp->b);
  }

  reader->consume(sizeof(ColorPacked) * m_tableSize);
}

ColorTable::~ColorTable() {
  delete m_table;
}

GlobalColorTable::GlobalColorTable(IReader *reader, LogicalScreenDescriptor *descriptor)
  : ColorTable (reader, descriptor->getColorTableSize()) {
    m_backgroundIndex = descriptor->getBackgroundColorIndex();
}
