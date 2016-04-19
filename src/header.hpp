/**
 * File: header.hpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#pragma once

#include "iReader.hpp"

namespace gif {

  enum class Version {
    v_87a,
    v_89a,
  };

  class Header {

    // IReader &m_reader;
    Version m_version;
    bool m_isValid;

  public:
    Header(IReader &reader);

    inline bool isValid() {
      return m_isValid;
    }

    inline Version getVersion() {
      return m_version;
    }
  };

}
