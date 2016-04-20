/**
 * File: gif2ppm.cpp
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
#include "gif/canvas.hpp"
#include "gif/imageData.hpp"

using namespace std;
using namespace gif;

int main(int argc, char **argv) {

  assert(sizeof(char) == sizeof(uint8_t));

  if(argc < 2) {
    printf("Missing argument: file path\n");
    return 1;
  }

  for(int i = 1; i < argc; ++i) {
    ifstream gif(argv[i]);
    string ppmfile(argv[i]);
    ppmfile.replace(ppmfile.end()-4, ppmfile.end(), ".ppm");

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

    Canvas image(lsd.getWidth(), lsd.getHeight());
    GlobalColorTable *globalColorTable = NULL;

    if(lsd.hasColorTable()) {
      globalColorTable = new GlobalColorTable(r, &lsd);
      image.fillColor(globalColorTable->getBackground());
    }

    ExtensionDetector extensionDetector(r);

    do {
      if(extensionDetector.hasExtension()) {
        extensionDetector.skipExtension();
      }
      else {
        ImageDescriptor imageDescriptor(r);

        ColorTable *localColorTable = NULL;
        ColorTable *colorTable = globalColorTable;
        if(imageDescriptor.hasColorTable()) {
          localColorTable = new ColorTable(r, imageDescriptor.getColorTableSize());
          colorTable = localColorTable;
        }

        Canvas subImage(imageDescriptor.getWidth(), imageDescriptor.getHeight());

        if(globalColorTable) {
          subImage.fillColor(globalColorTable->getBackground());
        }

        ImageData imageData(r);

        int index;

        if(imageDescriptor.isInterlaced()) {
          for(unsigned y = 0; y < imageDescriptor.getHeight(); y +=8) {
            for(unsigned x = 0; x < imageDescriptor.getWidth(); ++x) {
              index = imageData.next();
              assert(index != -1);

              subImage.setColor(colorTable->getColor(index), x, y);
            }
          }

          for(unsigned y = 4; y < imageDescriptor.getHeight(); y +=8) {
            for(unsigned x = 0; x < imageDescriptor.getWidth(); ++x) {
              index = imageData.next();
              assert(index != -1);

              subImage.setColor(colorTable->getColor(index), x, y);
            }
          }

          for(unsigned y = 2; y < imageDescriptor.getHeight(); y +=4) {
            for(unsigned x = 0; x < imageDescriptor.getWidth(); ++x) {
              index = imageData.next();
              assert(index != -1);

              subImage.setColor(colorTable->getColor(index), x, y);
            }
          }

          for(unsigned y = 1; y < imageDescriptor.getHeight(); y +=2) {
            for(unsigned x = 0; x < imageDescriptor.getWidth(); ++x) {
              index = imageData.next();
              assert(index != -1);

              subImage.setColor(colorTable->getColor(index), x, y);
            }
          }

          assert(imageData.next() == -1);
        }
        else {
          for(unsigned y = 0; y < imageDescriptor.getHeight(); ++y) {
            for(unsigned x = 0; x < imageDescriptor.getWidth(); ++x) {
              index = imageData.next();
              assert(index != -1);

              subImage.setColor(colorTable->getColor(index), x, y);
            }
          }

          assert(imageData.next() == -1);
        }

        image.drawAt(&subImage, imageDescriptor.getTop(), imageDescriptor.getLeft());

        delete localColorTable;
      }

      r->allocate(1);
    } while(r->peekByte() != (unsigned)Block::Trailer);

    delete globalColorTable;

    ofstream ppm(ppmfile.c_str());

    ppm << "P3" << endl;
    ppm << image.getWidth() << " " << image.getHeight() << endl;
    ppm << "255" << endl;

    for(unsigned y = 0; y < image.getHeight(); ++y) {
      Color *ptr = image.getColor(0, y);

      for(unsigned x = 0; x < image.getWidth(); ++x) {
        Color c = ptr[x];
        ppm << c.r << " " << c.g << " " << c.b << "  ";
      }

      ppm << endl;
    }

    printf("  Done\n");

    gif.close();

  }

  return 0;
}
