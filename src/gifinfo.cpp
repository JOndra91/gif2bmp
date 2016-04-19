/**
 * File: gifinfo.cpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#include <fstream>
#include <cstdio>
#include <cstdint>
#include <cassert>

#include "fileReader.hpp"
#include "header.hpp"
#include "logicalScreenDescriptor.hpp"
#include "colorTable.hpp"

using namespace std;
using namespace gif;

const char *boolStr[2] = {
  "No", "Yes"
};

int main(int argc, char **argv) {

  assert(sizeof(char) == sizeof(uint8_t));

  if(argc < 2) {
    printf("Missing argument: file path\n");
    return 1;
  }

  for(int i = 1; i < argc; ++i) {
    GlobalColorTable *globalColorTable = NULL;

    ifstream gif(argv[i]);

    FileReader f(&gif);

    Header h((IReader*)&f);

    if(h.isValid()) {
      printf("File '%s' is GIF image.\n", argv[i]);
    }
    else {
      printf("File '%s' is not GIF image.\n", argv[i]);
      continue;
    }

    if(h.getVersion() == Version::v_87a) {
      printf("  Version: 87a\n");
    }
    else {
      printf("  Version: 89a\n");
    }

    LogicalScreenDescriptor lsd((IReader*)&f);

    printf("  Size: %u × %u\n", lsd.getWidth(), lsd.getHeight() );
    printf("  Global color table: %s\n", boolStr[lsd.hasColorTable()]);
    printf("  Global color table size: %u\n", lsd.getColorTableSize());
    printf("  Global color table ordered: %s\n", boolStr[lsd.isColorTableOrdered()]);
    printf("  Color resolution: %u\n", lsd.getColorResolution());

    if(lsd.hasColorTable()) {
      globalColorTable = new GlobalColorTable((IReader*)&f, &lsd);

      Color bg = globalColorTable->getBackground();
      printf("  Background color: rgb(%u, %u, %u)\n",
        bg.r, bg.g, bg.b);

      printf("  Global color table:\n");

      unsigned size = globalColorTable->getSize();

      for(unsigned i = 0; i < size; ++i) {
        Color c = globalColorTable->getColor(i);
        printf("    Color[%u]: rgb(%u, %u, %u)\n",
          lsd.getBackgroundColorIndex(), c.r, c.g, c.b);
      }
    }

    delete globalColorTable;

    gif.close();

  }

  return 0;
}
