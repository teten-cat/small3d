#ifndef RAST_H
#define RAST_H

#include <linux/fb.h>
#include <stdint.h>
#include "brush.h"

#include "math/vector.h"


void Init_Rasterizer(uint32_t *pixel_buffer, int screen_width, vec2int window_corner);

void draw_lineV(int x0, int y0, int x1, int y1, uint32_t color);
void draw_lineH(int x0, int y0, int x1, int y1, uint32_t color);

void Draw_Line(int x0, int y0, int x1, int y1, uint32_t color_loaded);
void Draw_Line_Space(int x0, int y0, float z0, int x1, int y1, float z1, uint32_t color_loaded);
  
void Draw_Line_FILL(int x0, int y0, int x1, int y1, uint32_t color_loaded, int* output_x, int isLeft, int index_origin);
void Draw_Line_FILL_Space(int x0, int y0, float z0, int x1, int y1, float z1, uint32_t color_loaded, int* output_x, int isLeft, int index_origin);


int Point_In_Polygon(vec2int point, vec2int p1, vec2int p2, vec2int p3);

void Fill_Polygon(vec2int p1, vec2int p2, vec2int p3, uint32_t color);
void Fill_Polygon_Space(vec3 p1, vec3 p2, vec3 p3, uint32_t color);


#endif