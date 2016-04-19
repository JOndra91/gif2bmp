/**
 * File: colorTable.cpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#include "colorTable.hpp"

using namespace gif;

ColorTable::ColorTable(IReader &reader, LogicalScreenDescriptor &descriptor) {
  m_tableSize = descriptor.getColorTableSize();
  m_backgroundIndex = descriptor.getBackgroundColorIndex();

  m_table = new Color[m_tableSize];
  reader.allocate(sizeof(ColorPacked) * m_tableSize);

  const ColorPacked *cp = reinterpret_cast<const ColorPacked*>(reader.buffer());

  Color *end = m_table + m_tableSize;
  for(Color *c = m_table; c != end; c++, cp++) {
    *c = Color(*cp);
  }
}

ColorTable::~ColorTable() {
  delete m_table;
}