/**
 * File: imageDescriptor.cpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/20
 */

#include "imageDescriptor.hpp"
#include "blocks.hpp"

#include <cassert>

using namespace gif;

ImageDescriptor::ImageDescriptor(IReader *reader) {
  ImageDescriptorPackedField packedField;
  reader->allocate(10);

  // TODO: Proper handling
  assert(reader->readByte() == (unsigned)Block::ImageSeparator);

  m_left = reader->readWord();
  m_top = reader->readWord();
  m_width = reader->readWord();
  m_height = reader->readWord();

  packedField.packedValue = reader->readByte();

  m_colorTableFlag = packedField.colorTableFlag;
  m_interlaceFlag = packedField.interlaceFlag;
  m_sortFlag = packedField.sortFlag;
  m_colorTableSize = packedField.colorTableSize;
}
