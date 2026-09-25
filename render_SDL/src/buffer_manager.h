#ifndef BUFFER_H
#define BUFFER_H

#include <string.h>
#include "brush.h"
#include "window.h"



static inline void Send_Buffer(char *dest, char *src, int screensize) {
  // sends secondary buffer to the actuall screen buffer
  // screensize is the size of the buffer in bytes
  memcpy(dest, src, screensize);
}


static inline void Clear_Window(Window *w) {

  const uint32_t color = w->Background;
  const int screensize = (size_t)w->width * w->height;
  uint32_t *pixels = w->pixel_buffer;
  
  // thought this might be faster since it cuts the checks Colorband does
  for(int i = 0; i < screensize; i++) {
    pixels[i] = color;
  }

  // int right_wall = w->width+1;
  // for(int i = 0; i < w->height+1; i++) {
  //   Color_Band(w->pixel_buffer,w->width, 0, right_wall, i, w->Background);
  // }
}

static inline void Clear_Buffer(uint32_t *buffer, int screensize) {
  memset(buffer, 0, screensize * sizeof(uint32_t));
}

#endif
