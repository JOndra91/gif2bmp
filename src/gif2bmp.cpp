/**
 * File: gif2bmp.cpp
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/21
 */

#include <fstream>
#include <cstdio>
#include <cstdint>
#include <cassert>
#include <vector>
#include <tuple>
#include <endian.h>

#include "gif.hpp"
#include "image.hpp"
#include "bmp.hpp"

using namespace std;
using namespace gif;
using namespace image;


int main(int argc, char **argv) {

  assert(sizeof(char) == sizeof(uint8_t));

  if(argc < 2) {
    printf("Missing argument: file path\n");
    return 1;
  }

  for(int i = 1; i < argc; ++i) {
    ifstream gif(argv[i]);
    string bmpfile(argv[i]);
    bmpfile.replace(bmpfile.end()-4, bmpfile.end(), ".bmp");

    FStreamReader f(&gif);
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

    Canvas<RGBColor> image(lsd.getWidth(), lsd.getHeight());
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

        Canvas<RGBColor> subImage(imageDescriptor.getWidth(), imageDescriptor.getHeight());

        if(globalColorTable) {
          subImage.fillColor(globalColorTable->getBackground());
        }

        ImageData imageData(r);

        int index;
        vector<pair<unsigned, unsigned>> rowOrder;
        if(imageDescriptor.isInterlaced()) {
          rowOrder = {
            make_pair(0, 8),
            make_pair(4, 8),
            make_pair(2, 4),
            make_pair(1, 2),
          };
        }
        else {
          rowOrder = {
            make_pair(0, 1),
          };
        }

        for(auto &order : rowOrder) {
          unsigned offset = order.first;
          unsigned step = order.second;

          for(unsigned y = offset; y < imageDescriptor.getHeight(); y += step) {
            for(unsigned x = 0; x < imageDescriptor.getWidth(); ++x) {
              index = imageData.next();
              assert(index != -1);

              subImage.setColor(colorTable->getColor(index), x, y);
            }
          }
        }

        assert(imageData.next() == -1);

        image.drawAt(&subImage, imageDescriptor.getTop(), imageDescriptor.getLeft());

        delete localColorTable;
      }

      r->allocate(1);
    } while(r->peekByte() != (unsigned)Block::Trailer);

    delete globalColorTable;

    ofstream bmp(bmpfile.c_str(), ios::binary);

    bmp::FileHeader bmpFHeader;
    bmp::InfoHeader bmpIHeader;

    bmpFHeader = {
      .type = htole16(0x4D42),
      .offBits = htole32(sizeof(bmp::FileHeader) + sizeof(bmp::InfoHeader)),
    };

    unsigned rowPadding = (4 - (image.getWidth() * sizeof(BGR8ColorPacked)) % 4) % 4;
    bmpIHeader = {
      .size = htole32(40),
      .width = htole32((int32_t)image.getWidth()),
      .height = htole32((int32_t)image.getHeight()),
      .planes = htole16(1),
      .bitCount = htole16(24),
      .compression = 0,
      .sizeImage = htole32((uint32_t)
          ((image.getWidth() * sizeof(BGR8ColorPacked) + rowPadding) *
          image.getHeight())),
      .xPelsPerMeter = htole32(2835),
      .yPelsPerMeter = htole32(2835),
      .clrUsed = 0,
      .clrImportant = 0,
    };

    bmp.write((char*)&bmpFHeader, sizeof(bmp::FileHeader));
    bmp.write((char*)&bmpIHeader, sizeof(bmp::InfoHeader));

    unsigned rowBufferSize = image.getWidth() * sizeof(BGR8ColorPacked) + rowPadding;
    BGR8ColorPacked *rowBuffer = new BGR8ColorPacked[rowBufferSize]();
    for(int y = image.getHeight() - 1; y >= 0 ; --y) {
      RGBColor *ptr = image.getColor(0, y);

      for(int x = 0; x < image.getWidth(); ++x) {
        rowBuffer[x] = ptr[x].packRGB8();
      }

      bmp.write((char*)rowBuffer, rowBufferSize);
    }

    bmp.close();

    printf("  Done\n");

    gif.close();

  }

  return 0;
}
