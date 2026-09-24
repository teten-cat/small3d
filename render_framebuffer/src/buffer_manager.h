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
  int right_wall = w->width+1;
  for(int i = 0; i < w->height+1; i++) {
    Color_Band(w->fbp, w->line_length, 0, right_wall, i, w->Background);
  }
}

static inline void Clear_Buffer(char *buffer, int screensize) {
  memset(buffer, 0, screensize);
}

#endif
