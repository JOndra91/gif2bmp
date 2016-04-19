/**
 * File: logicalScreenDescriptor.cpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#include "logicalScreenDescriptor.hpp"

using namespace gif;

LogicalScreenDescriptor::LogicalScreenDescriptor(IReader &reader) {
  LogicalScreenDescriptorPackedField packedField;

  reader.allocate(6);

  m_width = reader.readWord();
  m_height = reader.readWord();

  packedField.packedValue = reader.readByte();

  m_backgroundIndex = reader.readByte();
  m_pixelAspectRatio = reader.readByte();

  if(m_pixelAspectRatio) {
    m_pixelAspectRatio = (m_pixelAspectRatio + 15) / 64;
  }

  m_colorTableFlag = packedField.colorTableFlag;
  m_sortFlag = packedField.sortFlag;
  m_colorResolution = packedField.colorResolution;
  m_colorTableSize = 1 << (packedField.colorTableSize + 1);

};
