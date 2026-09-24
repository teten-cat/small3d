#include "brush.h"


Brush_Setting brush;  // this should change the "brush in the header file

void Init_Brush(Window *w) {


  brush.origin_x = w->origin.x; // 30
  brush.origin_y = w->origin.y; // 30
  brush.cap_x_windowSpace = w->width; // 640
  brush.cap_y_windowSpace = w->height;// 480
  brush.cap_x_screenSpace = brush.origin_x + brush.cap_x_windowSpace;
  brush.cap_y_screenSpace = brush.origin_y + brush.cap_y_windowSpace;


  // printf("brush origin x %d\n", brush.origin_x);
  // printf("brush origin y %d\n", brush.origin_y);
  // printf("brush WINDOW space cap x %d\n", brush.cap_x_windowSpace);
  // printf("brush WINDOW space cap y %d\n", brush.cap_y_windowSpace);
  // printf("brush SCREEN space cap x %d\n", brush.cap_x_screenSpace);
  // printf("brush SCREEN space cap y %d\n", brush.cap_y_screenSpace);
}
