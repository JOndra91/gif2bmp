/**
 * File: gif2bmp.c
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/21
 */

#include "gif2bmp.h"

#include <cstdio>
#include <cstdint>
#include <vector>
#include <tuple>
#include <memory>
#include <endian.h>

#include "gif.hpp"
#include "image.hpp"
#include "bmp.hpp"

#define gif2bmp_assert(assertion) do{if(!(assertion)) return GIF2BMP_ERROR;} while(0)

using namespace std;
using namespace gif;
using namespace image;

int gif2bmp(tGIF2BMP *gif2bmp, FILE *inputFile, FILE *outputFile) {

  fseek(inputFile, 0, SEEK_SET);

  FileReader fileReader(inputFile);
  IReader *r = (IReader*)&fileReader;

  Header h(r);

  gif2bmp_assert(h.isValid());

  LogicalScreenDescriptor lsd(r);

  Canvas<RGBColor> image(lsd.getWidth(), lsd.getHeight());
  unique_ptr<GlobalColorTable> globalColorTable;

  if(lsd.hasColorTable()) {
    globalColorTable = unique_ptr<GlobalColorTable>(new GlobalColorTable(r, &lsd));
    image.fillColor(globalColorTable->getBackground());
  }

  ExtensionDetector extensionDetector(r);

  bool skipImage = false;
  do {
    if(extensionDetector.hasExtension()) {
      extensionDetector.skipExtension();
    }
    else if(skipImage) {
      ImageDescriptor imageDescriptor(r);
      if(imageDescriptor.hasColorTable()) {
        ColorTable(r, imageDescriptor.getColorTableSize());
      }

      ImageData(r).skip();
    }
    else {
      ImageDescriptor imageDescriptor(r);

      gif2bmp_assert(imageDescriptor.isValid());

      ColorTable *colorTable = globalColorTable.get();
      unique_ptr<ColorTable> localColorTable;
      if(imageDescriptor.hasColorTable()) {
        localColorTable = unique_ptr<ColorTable>(new ColorTable(r, imageDescriptor.getColorTableSize()));
        colorTable = localColorTable.get();
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
            gif2bmp_assert(index != -1);

            subImage.setColor(colorTable->getColor(index), x, y);
          }
        }
      }

      gif2bmp_assert(imageData.next() == -1);

      image.drawAt(&subImage, imageDescriptor.getTop(), imageDescriptor.getLeft());

      skipImage = true; // Skip next frames
    }

    r->allocate(1);
  } while(r->peekByte() != (unsigned)Block::Trailer);

  gif2bmp->gifSize = ftell(inputFile);

  bmp::FileHeader bmpFHeader;
  bmp::InfoHeader bmpIHeader;

  unsigned rowPadding = (4 - (image.getWidth() * sizeof(BGR8ColorPacked)) % 4) % 4;
  unsigned imageDataSize = ((image.getWidth() * sizeof(BGR8ColorPacked) +
    rowPadding) * image.getHeight());
  unsigned headerSize = sizeof(bmp::FileHeader) + sizeof(bmp::InfoHeader);

  bmpFHeader = {
    .type = htole16(0x4D42),
    .size = htole32(imageDataSize + headerSize),
    .reserved1 = 0,
    .reserved2 = 0,
    .offBits = htole32(headerSize),
  };

  bmpIHeader = {
    .size = htole32(40),
    .width = htole32((int32_t)image.getWidth()),
    .height = htole32((int32_t)image.getHeight()),
    .planes = htole16(1),
    .bitCount = htole16(24),
    .compression = 0,
    .sizeImage = htole32(imageDataSize),
    .xPelsPerMeter = htole32(2835),
    .yPelsPerMeter = htole32(2835),
    .clrUsed = 0,
    .clrImportant = 0,
  };

  fwrite(&bmpFHeader, sizeof(bmp::FileHeader), 1, outputFile);
  fwrite(&bmpIHeader, sizeof(bmp::InfoHeader), 1, outputFile);

  unsigned rowBufferSize = image.getWidth() * sizeof(BGR8ColorPacked) + rowPadding;
  BGR8ColorPacked *rowBuffer = new BGR8ColorPacked[rowBufferSize]();
  for(int y = image.getHeight() - 1; y >= 0 ; --y) {
    RGBColor *ptr = image.getColor(0, y);

    for(unsigned x = 0; x < image.getWidth(); ++x) {
      rowBuffer[x] = ptr[x].packRGB8();
    }

    fwrite(rowBuffer, sizeof(uint8_t), rowBufferSize, outputFile);
  }
  delete[] rowBuffer;

  gif2bmp->bmpSize = imageDataSize + headerSize;

  return GIF2BMP_OK;

}
