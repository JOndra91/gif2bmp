/**
 * File: header.cpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#include "header.hpp"
#include <cstdint>
#include <string>

using namespace gif;
using namespace std;

Header::Header(IReader &reader) {
  char buffer[3];
  string version;

  reader.allocate(6);
  reader.copy(3, buffer);

  m_isValid = string(buffer, 3) == "GIF";

  if(!m_isValid) {
    return;
  }

  reader.copy(3, buffer);

  version = string(buffer, 3);

  if(version == "87a") {
    m_version = Version::v_87a;
  }
  else if(version == "89a") {
    m_version = Version::v_89a;
  }
  else {
    m_isValid = false;
  }

};
