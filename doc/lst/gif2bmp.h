/**
 * File: gif2bmp.h
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/21
 */

#ifndef GIF2BMP_H
#define GIF2BMP_H

#ifdef __cplusplus
#include <cstdint>
#include <cstdio>
#else
#include <stdint.h>
#include <stdio.h>
#endif

#ifdef __cplusplus
extern "C" {
using std::int64_t;
using std::FILE;
#endif

typedef struct {
    int64_t bmpSize;
    int64_t gifSize;
} tGIF2BMP;

enum {
  GIF2BMP_OK = 0,
  GIF2BMP_ERROR = -1,
};

int gif2bmp(tGIF2BMP *gif2bmp, FILE *inputFile, FILE *outputFile);

#ifdef __cplusplus
}
#endif

#endif // GIF2BMP_H
