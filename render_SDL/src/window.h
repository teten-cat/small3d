#ifndef WINDOW_H
#define WINDOW_H


#include <stdint.h>
#include "math/vector.h"


typedef struct {
  uint32_t* pixel_buffer;
  vec2int origin;
  int width;
  int height;
  uint32_t Background;
} Window;

#endif