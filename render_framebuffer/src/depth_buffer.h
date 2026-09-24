#ifndef DEPTH_BUFFER_H
#define DEPTH_BUFFER_H

#include <linux/fb.h>


void Init_Depth_Buffer(int row_length, int height);

void Clear_Depth();
float Get_Depth(int x, int y);
int Attempt_Buffer(int x, int y, float value);
void Set_Buffer(int x, int y, float value);
void Show_Depth(char* buffer, int line_length, struct fb_var_screeninfo* vinfo);
void Print_WHOLE_DepthBuffer();
#endif