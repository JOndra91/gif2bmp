/**
 * File: extension.cpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#include "extension.hpp"
#include "blocks.hpp"

using namespace gif;

ExtensionDetector::ExtensionDetector(IReader *reader) : m_reader(reader) {};

bool ExtensionDetector::hasExtension() {
  m_reader->allocate(2);
  return m_reader->peekByte() == (unsigned)Block::ExtensionIntroducer;
}

ExtensionLabel ExtensionDetector::getExtensionType() {
  m_reader->allocate(2);
  unsigned label = m_reader->peekByte(1);

  switch (label) {

    case (unsigned)ExtensionLabel::PlainText:
      return ExtensionLabel::PlainText;

    case (unsigned)ExtensionLabel::GraphicControl:
      return ExtensionLabel::GraphicControl;

    case (unsigned)ExtensionLabel::Comment:
      return ExtensionLabel::Comment;

    case (unsigned)ExtensionLabel::Application:
      return ExtensionLabel::Application;

    default:
      return ExtensionLabel::None;
  }
}

void ExtensionDetector::skipExtension() {

  if(!hasExtension()) {
    return;
  }

  m_reader->consume(2);
  m_reader->allocate(512);

  unsigned blockSize;
  while((blockSize = m_reader->readByte())) {
    m_reader->consume(blockSize);
    if(m_reader->allocated() == 0) {
      m_reader->allocate(512);
    }
  }
}
