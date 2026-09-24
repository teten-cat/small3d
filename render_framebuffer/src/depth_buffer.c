#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "depth_buffer.h"
#include "brush.h"
typedef float* depth_buffer;

depth_buffer z_buffer;

int buffer_width = 0;
int buffer_height = 0;
int length;
int x_offset;
int y_offset;

void Init_Depth_Buffer(int row_length, int height) {

  if(z_buffer != NULL) {  // just in case
    free(z_buffer);
    z_buffer = NULL;
  }

  buffer_width = row_length;
  buffer_height = height;
  length = buffer_height * buffer_width;

  // printf("depth buffer:\n");
  // printf("width: %d\n", buffer_width);
  // printf("height: %d\n", buffer_height);
  // printf("legnth: %d\n", length);

  
  z_buffer = (float *)malloc(sizeof(float) * length);
}

void Clear_Depth() {
  for (int i = 0; i < length; i++) {
    // i have no idea why INFINITY works but -INFINITY didnt. maybe the Z axis is going the other way??? idk
      z_buffer[i] = INFINITY; 
  }
}

float Get_Depth(int x, int y) {
  if(x < 0) x = 0;
  if(x >= buffer_width) x = buffer_width-1;
  if(y < 0) y = 0;
  if(y >= buffer_height) y = buffer_height-1;

  return z_buffer[buffer_width * y + x];
}

int Attempt_Buffer(int x, int y, float value) {
  
  if(y < 0 || y >= buffer_height || x < 0 || x >= buffer_width)
    return 0;

  // return 1 when was successful.
  if(z_buffer[buffer_width * y + x] > value) {
    Set_Buffer(x, y, value);
    return 1;
  }
  else
    return 0;
}

void Set_Buffer(int x, int y, float value) {
  z_buffer[buffer_width * y + x] = value;
}

void Show_Depth(char* buffer, int line_length, struct fb_var_screeninfo* vinfo) {
  float minZ = 1e9f;
  float maxZ = -1e9f;
  for(int i = 0; i < buffer_height; i++) {
    for(int j = 0; j < buffer_width; j++) {
      float z = z_buffer[buffer_width * i + j];
      if(z < minZ) minZ = z;
      if(z > maxZ) maxZ = z;
    }
  }

  float range = maxZ - minZ;
  if(range == 0.0f) range = 1.0f;

  for(int i = 0; i < buffer_height; i++) {
    for(int j = 0; j < buffer_width; j++) {
      float z = z_buffer[buffer_width * i + j];
      
      // Normalize depth linearly from 0.0 to 1.0
      float normalized = (z - minZ) / range;
      
      int value = (int)round(255.0f * normalized);

      Color color = {.r = value, .g = value, .b = value};
      Color_Pixel(buffer, line_length, j, i, Load_Pixel(color, vinfo));
    }
  }
}

void Print_WHOLE_DepthBuffer() {
  for(int i = 0; i < buffer_height; i++) {
    printf("%d: ", i);
    for(int j = 0; j < buffer_width; j++) {
      float z = z_buffer[buffer_width * i + j];
      printf("%7f, ", z);
    }
    printf("\n");
  }
}