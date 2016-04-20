/**
 * File: gifinfo.cpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/19
 */

#include <fstream>
#include <cstdio>
#include <cstdint>
#include <cassert>

#include "gif/fileReader.hpp"
#include "gif/header.hpp"
#include "gif/logicalScreenDescriptor.hpp"
#include "gif/colorTable.hpp"
#include "gif/extension.hpp"
#include "gif/imageDescriptor.hpp"
#include "gif/blocks.hpp"

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
    ifstream gif(argv[i]);

    FileReader f(&gif);
    IReader *r = (IReader*)&f;

    Header h(r);

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

    LogicalScreenDescriptor lsd(r);

    printf("  Size: %u × %u\n", lsd.getWidth(), lsd.getHeight() );
    printf("  Global color table: %s\n", boolStr[lsd.hasColorTable()]);
    printf("  Global color table size: %u\n", lsd.getColorTableSize());
    printf("  Global color table ordered: %s\n", boolStr[lsd.isColorTableOrdered()]);
    printf("  Color resolution: %u\n", lsd.getColorResolution());

    if(lsd.hasColorTable()) {
      GlobalColorTable globalColorTable(r, &lsd);

      Color bg = globalColorTable.getBackground();
      printf("  Background color: rgb(%u, %u, %u)\n",
        bg.r, bg.g, bg.b);

      printf("  Global color table:\n");

      unsigned size = globalColorTable.getSize();

      for(unsigned i = 0; i < size; ++i) {
        Color c = globalColorTable.getColor(i);
        printf("    Color[%u]: rgb(%u, %u, %u)\n", i, c.r, c.g, c.b);
      }
    }

    ExtensionDetector extensionDetector(r);

    do {
      ExtensionLabel label = ExtensionLabel::None;
      if(extensionDetector.hasExtension()) {
        label = extensionDetector.getExtensionLabel();
        string extension;

        switch (label) {
          case ExtensionLabel::PlainText:
            extension = "PlainText";
            break;

          case ExtensionLabel::GraphicControl:
            extension = "GraphicControl";
            break;

          case ExtensionLabel::Comment:
            extension = "Comment";
            break;

          case ExtensionLabel::Application:
            extension = "Application";
            break;

          default:
            extension = "None";
        }

        printf("  Extension: %s\n", extension.c_str());

        extensionDetector.skipExtension();
        printf("    skipped\n");
      }
      else {
        ImageDescriptor imageDescriptor(r);

        printf("  Image descriptor:\n");
        printf("    Position: %u × %u\n", imageDescriptor.getTop(), imageDescriptor.getLeft());
        printf("    Size: %u × %u\n", imageDescriptor.getWidth(), imageDescriptor.getHeight());
        printf("    Interlaced: %s\n", boolStr[imageDescriptor.isInterlaced()]);
        printf("    Local color table: %s\n", boolStr[imageDescriptor.hasColorTable()]);
        printf("    Local color table size: %u\n", imageDescriptor.getColorTableSize());
        printf("    Local color table ordered: %s\n", boolStr[imageDescriptor.isColorTableOrdered()]);

        if(imageDescriptor.hasColorTable()) {
          ColorTable localColorTable(r, imageDescriptor.getColorTableSize());

          printf("    Local color table:\n");

          unsigned size = localColorTable.getSize();

          for(unsigned i = 0; i < size; ++i) {
            Color c = localColorTable.getColor(i);
            printf("      Color[%u]: rgb(%u, %u, %u)\n", i, c.r, c.g, c.b);
          }
        }

        { // Skip image data
          r->consume(1);
          r->allocate(512);

          unsigned blockSize;
          while((blockSize = r->readByte())) {
            r->consume(blockSize);
            if(r->allocated() == 0) {
              r->allocate(512);
            }
          }
        }
      }

      r->allocate(1);
    } while(r->peekByte() != (unsigned)Block::Trailer);

    printf("  Trailer\n");

    gif.close();

  }

  return 0;
}
