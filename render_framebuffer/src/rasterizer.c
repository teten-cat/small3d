#include "rasterizer.h"
#include "brush.h"
#include "depth_buffer.h"
#include "math/vec2i.h"
#include "utility/cmp.h"
#include "math/vector.h"


#include <math.h>
#include <limits.h>
#include <linux/fb.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>


static char *fbp;
static struct fb_var_screeninfo vinfo;
static int line_length;

int window_corner_x;  // 30
int window_corner_y;  // 30

Color white;
uint32_t loaded_white;

int cheking = 0;


void Init_Rasterizer(char *fbp_init, const struct fb_var_screeninfo *v, int line_length_bytes, vec2int window_corner) {
  fbp = fbp_init;
  line_length = line_length_bytes;
  vinfo = *v;
  white.r = white.g = white.b = 255;
  loaded_white = Load_Pixel(white, &vinfo);

  window_corner_x = window_corner.x;
  window_corner_y = window_corner.y;
}


void Draw_Line(int x0, int y0, int x1, int y1, uint32_t color_loaded) {
  // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
  int dx = abs(x1 - x0);
  int sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0);
  int sy = y0 < y1 ? 1 : -1;
  int error = dx + dy;

  // output are x coordinates that will be used to draw bands when filling polygons

  while(1) {
    Color_Pixel(fbp, line_length, x0, y0, color_loaded);

    int e2 = 2*error;
    if(e2 >= dy) {
      if(x0 == x1) break;
      error = error + dy;
      x0 = x0+sx;
    }
    if(e2 <= dx) {
      if(y0 == y1)  break;
      error = error + dx;
      y0 = y0 + sy;
    }
  }
}

void Draw_Line_Space(int x0, int y0, float z0, int x1, int y1, float z1, uint32_t color_loaded) {
  // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
  int dx = abs(x1 - x0);
  int sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0);
  int sy = y0 < y1 ? 1 : -1;
  int error = dx + dy;

  // output are x coordinates that will be used to draw bands when filling polygons

  int longer_axis_length = max2i(dx, -dy);
  int depth_index = 0;

  while(1) {
    // z3 is between z0 and z1. 
    //  z0   (1-t)  z3   (t)  z1        // maybe the t and 1-t was inverted
    //  |------------|---------|
    // z3 is calculated by the depth interpolation formula.
    // the bottom and top of fraction is multiplied by z0*z1 to get rid of fractions in them

    float t = (longer_axis_length == 0) ? 0.0f : (float)depth_index / longer_axis_length;
    
    float frac_base = z1 * (1 - t) + z0 * t;
    float z3 = (frac_base != 0) ? (z0 * z1) / frac_base : z0;

    if(Attempt_Buffer(x0, y0, z3))
      Color_Pixel(fbp, line_length, x0, y0, color_loaded);

    int e2 = 2*error;
    if(e2 >= dy) {
      if(x0 == x1) break;
      error = error + dy;
      x0 = x0+sx;
    }
    if(e2 <= dx) {
      if(y0 == y1)  break;
      error = error + dx;
      y0 = y0 + sy;
    }

    depth_index++;

  }
}



void Draw_Line_FILL(int x0, int y0, int x1, int y1, uint32_t color_loaded, int* output_x, int isLeft, int index_origin) {
  // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
  int dx = abs(x1 - x0);
  int sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0);
  int sy = y0 < y1 ? 1 : -1;
  int error = dx + dy;

  // output are x coordinates that will be used to draw bands when filling polygons

  int index;

  while(1) {
    index = y0 - index_origin; 
    if(isLeft) {
      output_x[index] = min2i(x0, output_x[index]);
    } else {
      output_x[index] = max2i(x0, output_x[index]);
    }
    Color_Pixel(fbp, line_length, x0, y0, color_loaded);

    int e2 = 2*error;
    if(e2 >= dy) {
      if(x0 == x1) break;
      error = error + dy;
      x0 = x0+sx;
    }
    if(e2 <= dx) {
      if(y0 == y1)  break;
      error = error + dx;
      y0 = y0 + sy;
    }
  }
}

void Draw_Line_FILL_Space(int x0, int y0, float z0, int x1, int y1, float z1, uint32_t color_loaded, int* output_x, int isLeft, int index_origin) {
  // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
  int dx = abs(x1 - x0);
  int sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0);
  int sy = y0 < y1 ? 1 : -1;
  int error = dx + dy;

  // output are x coordinates that will be used to draw bands when filling polygons

  int index;

  int longer_axis_length = max2i(dx, -dy);
  int depth_index = 0;

  while(1) {
    index = y0 - index_origin; 
    if(isLeft) {
      output_x[index] = min2i(x0, output_x[index]);
    } else {
      output_x[index] = max2i(x0, output_x[index]);
    }

    // z3 is between z0 and z1. 
    //  z0   (1-t)  z3   (t)  z1        // maybe the t and 1-t was inverted
    //  |------------|---------|
    // z3 is calculated by the depth interpolation formula.
    // the bottom and top of fraction is multiplied by z0*z1 to get rid of fractions in them

    float t = (longer_axis_length == 0) ? 0 : (float)depth_index / longer_axis_length;

    float frac_base = z1 * (1 - t) + z0 * t;
    float z3 = (frac_base != 0) ? (z0 * z1) / frac_base : z0;


    if(Attempt_Buffer(x0, y0, z3))
      Color_Pixel(fbp, line_length, x0, y0, color_loaded);

    int e2 = 2*error;
    if(e2 >= dy) {
      if(x0 == x1) break;
      error = error + dy;
      x0 = x0+sx;
    }
    if(e2 <= dx) {
      if(y0 == y1)  break;
      error = error + dx;
      y0 = y0 + sy;
    }

    depth_index++;
  }
}


// this used to be used but not anymore after improving the triangle filling algorithm.
// However, same logic is used in the triangulation but with floats instead. would be made in another file

int Point_In_Polygon(vec2int point, vec2int p1, vec2int p2, vec2int p3) {

  vec2int lineAB = {p2.x - p1.x, p2.y - p1.y};
  vec2int lineBC = {p3.x - p2.x, p3.y - p2.y};
  vec2int lineCA = {p1.x - p3.x, p1.y - p3.y};

  // check if point is in triangle
  vec2int lineAP = {point.x - p1.x, point.y - p1.y};

  int cp1 = Cross_vec2i(lineAB, lineAP);
  
  vec2int lineBP = {point.x - p2.x, point.y - p2.y};

  int cp2 = Cross_vec2i(lineBC, lineBP);

  vec2int lineCP = {point.x - p3.x, point.y - p3.y};

  int cp3 = Cross_vec2i(lineCA, lineCP);

  int any_neg = (cp1 < 0) || (cp2 < 0) || (cp3 < 0);
  int any_pos = (cp1 > 0) || (cp2 > 0) || (cp3 > 0);

  return !(any_neg && any_pos);

}


/*
  Filling a polygon works by
  Draw the lines and keep track of the left most, right most of the lines
  fill each scanline from the kept coordinates
*/

void Fill_Polygon(vec2int p1, vec2int p2, vec2int p3, uint32_t color) {
  int maxX = max3i(p1.x, p2.x, p3.x);
  int minX = min3i(p1.x, p2.x, p3.x);

  int maxY = max3i(p1.y, p2.y, p3.y);
  int minY = min3i(p1.y, p2.y, p3.y);

  int box_width = maxX - minX;
  int box_height = maxY - minY;

  // edge cases. since i dont know why it got double length somtimes. it was buggy
  if(box_height == 0 && box_width == 0) {
    Color_Pixel(fbp, line_length, maxX, maxY, color);
    return;
  }
  if(box_height == 0) {
    Color_Band(fbp, line_length, minX, maxX, maxY, color);
    return;
  } else if(box_width == 0) {
    Draw_Line(minX, minY, minX, minY, color);
    return;
  }


  

  vec2int apex = (p1.y == minY) ? p1 : ((p2.y <= p3.y) ? p2 : p3);  
  vec2int bottom = (p1.y == maxY) ? p1 : ((p2.y >= p3.y) ? p2 : p3);
  vec2int middle; // stupid way to find middle
  middle.x = (p1.x + p2.x + p3.x) - apex.x - bottom.x;
  middle.y = (p1.y + p2.y + p3.y) - apex.y - bottom.y;



  int band_left[box_height];
  int band_right[box_height];

  for(int i = 0; i < box_height; i++) {
    band_left[i] = INT_MAX;
    band_right[i] = INT_MIN;
  }

  if(bottom.x <= apex.x) {
    // longest line goes down left. ex.
    /*   .
        / \
       /   .
      /   
    */
    // long left side
    Draw_Line_FILL(apex.x, apex.y, bottom.x, bottom.y, color, band_left, 1, minY);  
    // right 2 shorter sides
    Draw_Line_FILL(apex.x, apex.y, middle.x, middle.y, color, band_right, 0, minY);  
    Draw_Line_FILL(middle.x, middle.y, bottom.x, bottom.y, color, band_right, 0, minY);  
  } else {

    // longest line goes down right. ex.
    /*   .
        / \
           \
            \
    */
    // long right side
    Draw_Line_FILL(apex.x, apex.y, bottom.x, bottom.y, color, band_right, 0, minY);
    // left 2 shorter sides
    Draw_Line_FILL(apex.x, apex.y, middle.x, middle.y, color, band_left, 1, minY);  
    Draw_Line_FILL(middle.x, middle.y, bottom.x, bottom.y, color, band_left, 1, minY);
  }

  // fill the middle
  for(int i = 0; i < box_height; i++) {
    Color_Band(fbp, line_length, band_left[i], band_right[i], apex.y+i, color);
  }

}

void Fill_Polygon_Space(vec3 p1, vec3 p2, vec3 p3, uint32_t color) {
  int maxX = max3i((int)p1.x, (int)p2.x, (int)p3.x);
  int minX = min3i((int)p1.x, (int)p2.x, (int)p3.x);
  int maxY = max3i((int)p1.y, (int)p2.y, (int)p3.y);
  int minY = min3i((int)p1.y, (int)p2.y, (int)p3.y);


  int box_width = maxX - minX;
  int box_height = maxY - minY;

  // edge cases. since i dont know why it got double length when boxheight was 0 but not all the time
  if(box_height == 0 && box_width == 0) {
    if(Attempt_Buffer(maxX, maxY, p1.z))
      Color_Pixel(fbp, line_length, maxX, maxY, color);
    return;
  }

  float minX_z = (minX == p1.x) ? p1.z : ((minX == p2.x) ? p2.z : p3.z);
  float maxX_z = (maxX == p1.x) ? p1.z : ((maxX == p2.x) ? p2.z : p3.z);
  float minY_z = (minY == p1.y) ? p1.z : ((minY == p2.y) ? p2.z : p3.z);
  float maxY_z = (maxY == p1.y) ? p1.z : ((maxY == p2.y) ? p2.z : p3.z);

  if(box_height == 0) {
    Color_Band_Space(fbp, line_length, minX, maxX, maxY, minX_z, maxX_z, color);
    return;
  } else if(box_width == 0) {
    Draw_Line_Space(minX, minY,minY_z, minX, maxY, maxY_z, color);
    return;
  }

  vec3 apex = (p1.y == (float)minY) ? p1 : ((p2.y <= p3.y) ? p2 : p3);  
  vec3 bottom = (p1.y == (float)maxY) ? p1 : ((p2.y >= p3.y) ? p2 : p3);
  vec3 middle; // stupid way to find middle but only called once so ig doesnt matter so much

  middle.x = (p1.x + p2.x + p3.x) - apex.x - bottom.x;
  middle.y = (p1.y + p2.y + p3.y) - apex.y - bottom.y;
  middle.z = (p1.z + p2.z + p3.z) - apex.z - bottom.z;

  int band_left[box_height];
  int band_right[box_height];

  for(int i = 0; i < box_height; i++) {
    band_left[i] = INT_MAX;
    band_right[i] = INT_MIN;
  }

  if(bottom.x <= apex.x) {
    // longest line goes down left. ex.
    /*   .
        / \
       /   .
      /   
    */
    // long left side

    Draw_Line_FILL_Space((int)apex.x, (int)apex.y, apex.z, (int)bottom.x, (int)bottom.y, bottom.z, color, band_left, 1, minY);  
    // right 2 shorter sides
    Draw_Line_FILL_Space((int)apex.x, (int)apex.y, apex.z, (int)middle.x, (int)middle.y, middle.z, color, band_right, 0, minY);  
    Draw_Line_FILL_Space((int)middle.x, (int)middle.y, middle.z, (int)bottom.x, (int)bottom.y, bottom.z, color, band_right, 0, minY);  
  } else {

    // longest line goes down right. ex.
    /*   .
        / \
       .   \
            \
            -.
    */
    // long right side
    Draw_Line_FILL_Space((int)apex.x, (int)apex.y, apex.z, (int)bottom.x, (int)bottom.y, bottom.z, color, band_right, 0, minY);  
    // left 2 shorter sides
    Draw_Line_FILL_Space((int)apex.x, (int)apex.y, apex.z, (int)middle.x, (int)middle.y, middle.z, color, band_left, 1, minY);  
    Draw_Line_FILL_Space((int)middle.x, (int)middle.y, middle.z, (int)bottom.x, (int)bottom.y, bottom.z, color, band_left, 1, minY);  
  }

  // fill the middle but check depth first
  for(int i = 0; i < box_height; i++) {
    if(apex.y+i < 0)  continue;

    float left_z = Get_Depth(band_left[i], apex.y+i);
    float right_z = Get_Depth(band_right[i], apex.y+i);
    if (isnan(left_z) || isnan(right_z))
      continue;
    Color_Band_Space(fbp, line_length, band_left[i], band_right[i], apex.y+i,left_z, right_z, color);
  }

  cheking = 1;
}