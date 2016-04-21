/**
 * File: main.c
 * Author: Ondřej Janošík <xjanos12@stud.fit.vutbr.cz>
 * Created: 2016/04/21
 */

#include <getopt.h>
#include <stdio.h>
#include <unistd.h>

#include "gif2bmp.h"

void printHelp(FILE *output) {

  fprintf(output,
    "Utility for conversion from GIF image format to BMP image format.\n"
    "\n"
    "Usage:\n"
    "\n"
    "  gif2img [-i INPUT] [-o OUTPUT] [-l LOG]\n"
    "  gif2img -h\n"
    "\n"
    "Options:\n"
    "\n"
    "  -i INPUT\n"
    "    Read GIF image from INPUT file (default: stdin).\n"
    "\n"
    "  -o OUTPUT\n"
    "    Write BMP image to OUTPUT file (default: stdout).\n"
    "\n"
    "  -l LOG\n"
    "    Write log to LOG file.\n"
    "\n"
    "  -h\n"
    "    Prints this help.\n"
  );

}

int main(int argc, char** argv) {

  FILE *inputFile = stdin;
  FILE *outputFile = stdout;
  FILE *logFile = NULL;

  int c;
  while ((c = getopt (argc, argv, "i:o:l:h")) != -1) {
    switch (c) {
      case 'i':
        inputFile = fopen(optarg, "r");
        break;
      case 'o':
        outputFile = fopen(optarg, "w");
        break;
      case 'l':
        logFile = fopen(optarg, "w");
        break;
      case 'h':
        printHelp(stdout);
        return 0;
      default:
        printHelp(stderr);
        return 1;
    }
  }

  if(inputFile == NULL || outputFile == NULL) {
    fprintf(stderr, "Error: %s\n", "Could not open INPUT or OUTPUT file.");
    return 2;
  }

  tGIF2BMP stats;
  gif2bmp(&stats, inputFile, outputFile);

  if(logFile) {
    fprintf(logFile,
      "login = %s\n"
      "uncodedSize = %ld\n"
      "codedSize = %ld\n", "xjanos12", stats.bmpSize, stats.gifSize);
  }

  return 0;
}
