#ifndef BRUSH_H
#define BRUSH_H

#include <stdint.h>
#include <linux/fb.h>
#include <stdio.h>
#include "window.h"
#include "depth_buffer.h"

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} Color;

typedef struct {
  int origin_x; // 30 
  int origin_y; // 30
  int cap_x_windowSpace;  // 670 = 30 + 640
  int cap_y_windowSpace;  // 510 = 30 + 480
  int cap_x_screenSpace;  // 670 = 30 + 640
  int cap_y_screenSpace;  // 510 = 30 + 480
} Brush_Setting;

extern Brush_Setting brush;


void Init_Brush(Window *w);

static inline uint32_t Load_Pixel(Color p) {
  uint8_t a = 255;
  uint32_t pixel_color = (p.r << 24) | (p.g << 16) | (p.b << 8) | a;
  return pixel_color;
}


// a (0, 0) input is the top left of the window
static inline void Color_Pixel(uint32_t *pixel_buffer, int width, int x, int y, uint32_t pixel_data) {
  // make sure to draw only within the window
  if(x < 0 || y < 0 || x > brush.cap_x_windowSpace || y > brush.cap_y_windowSpace)  return;
  int index = (y * width) + x;
  pixel_buffer[index] = pixel_data;
}

// input coordinates in window space
static inline void Color_Band(uint32_t *pixel_buffer, int width, int x0, int x1, int y, uint32_t pixel_data) {
  if(x1 < x0) {
    int tmp = x1;
    x1 = x0;
    x0 = tmp;
  }

  if(x0 < 0)  x0 = 0;
  if(x1 > brush.cap_x_windowSpace)  x1 = brush.cap_x_windowSpace;
  if(y < 0) return;
  if(y > brush.cap_y_windowSpace) return;

  int dx = x1-x0;
  
  // output position in screen space
  int start_index = (y * width) + x0;

  for(int i = 0; i < dx; i++) {
    pixel_buffer[start_index+i] = pixel_data;
  }
}

static inline void Color_Band_Space(uint32_t *pixel_buffer, int width, int x0, int x1, int y, float z0, float z1, uint32_t pixel_data) {
  if(x1 < x0) {
    int tmpx = x1;
    int tmpz = z1;

    x1 = x0;
    z1 = z0;
    x0 = tmpx;
    z0 = tmpz;
  }

  if(x0 < 0)  x0 = 0;
  if(x1 > brush.cap_x_windowSpace)  x1 = brush.cap_x_windowSpace;
  if(y < 0) return;
  if(y > brush.cap_y_windowSpace) return;

  int dx = x1-x0;
  
  // output position in screen space
  int start_index = (y * width) + x0;

  for(int i = 0; i < dx; i++) {
    float t = (float)i / dx;
    float z3 = (z0 * z1)/(z1 * (1-t) + z0*t);
    
    
    if(Attempt_Buffer(x0+i, y, z3)) {
      pixel_buffer[start_index+i] = pixel_data;
    }
  }
}

#endif