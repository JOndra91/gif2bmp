/**
 * File: imageData.hpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/20
 */

#pragma once

#include "iReader.hpp"

namespace gif {

  class ImageData {

    IReader *m_reader;

  public:

    ImageData(IReader *reader);

    void skip();

  };


}
