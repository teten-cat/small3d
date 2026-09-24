#ifndef POLY_H
#define POLY_H

#include "math/vector.h"
#include "brush.h"
#include <linux/fb.h>

typedef enum {
  WIREFRAME,
  FILL,
  COLOR_FRAME,
  COLOR_FILL
} DrawMode;

void Init_Polygon_Drawer(char *fbp_init, float* depth_init, const struct fb_var_screeninfo *v, int line_length_bytes, vec2int coordinates_origin, Color *colors, int colors_num);

void Reset_Polygon_Colors();

void Draw_Polygon(vec2int p1, vec2int p2, vec2int p3, Color color, DrawMode mode);

void Draw_Space_Polygon(vec3 p1, vec3 p2, vec3 p3, Color color, DrawMode mode);

void Clean_Polygon_Drawer();

  #endif