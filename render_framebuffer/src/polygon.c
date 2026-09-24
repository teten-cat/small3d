#include <stdlib.h>
#include <linux/fb.h>
#include <stdint.h>
#include "math/vector.h"
#include "polygon.h"
#include "rasterizer.h"


struct fb_var_screeninfo vinfo;
int line_length;
char *fbp;
float *depth_buffer;
vec2int origin_vec2i;
vec3 origin_vec3;

uint32_t *default_colors;
static int color_index = 0;
int color_array_length = 0;


void Init_Polygon_Drawer(char *fbp_init, float* depth_init, const struct fb_var_screeninfo *v, int line_length_bytes, vec2int coordinates_origin, Color *colors, int colors_num) {
  vinfo = *v;
  line_length = line_length_bytes;
  fbp = fbp_init;
  depth_buffer = depth_init;
  origin_vec2i = coordinates_origin;
  origin_vec3 = (vec3){coordinates_origin.x, coordinates_origin.y, 0};

  color_array_length = colors_num;  // just not sure if sizeof works since a color is only 3 bytes

  default_colors = (uint32_t*)malloc(color_array_length * sizeof(uint32_t));
  for(int i = 0; i < color_array_length; i++) {
    default_colors[i] = Load_Pixel(colors[i], &vinfo);
  }
}

// resets the index of the color array so wireframes and faces would have same color in the next frame
void Reset_Polygon_Colors() {
  color_index = 0;
}

void Draw_Polygon(vec2int p1, vec2int p2, vec2int p3,  Color color, DrawMode mode) {

  uint32_t loaded_color = Load_Pixel(color, &vinfo);


  vec2int point1 = p1;
  vec2int point2 = p2;
  vec2int point3 = p3;

  if(mode == WIREFRAME) {
    Draw_Line(point1.x, point1.y, point2.x, point2.y, loaded_color);
    Draw_Line(point2.x, point2.y, point3.x, point3.y, loaded_color);
    Draw_Line(point3.x, point3.y, point1.x, point1.y, loaded_color);
  }

  if(mode == FILL)
    Fill_Polygon(point1, point2, point3, loaded_color);

  if(mode == COLOR_FRAME) {
    Draw_Line(point1.x, point1.y, point2.x, point2.y, default_colors[(color_index++) % color_array_length]);  // The last arguments indexes,
    Draw_Line(point2.x, point2.y, point3.x, point3.y, default_colors[(color_index++) % color_array_length]);  // i dont like this but its a 
    Draw_Line(point3.x, point3.y, point1.x, point1.y, default_colors[(color_index++) % color_array_length]);  // way to reset the index when its big
  }

  if(mode == COLOR_FILL) {
    Fill_Polygon(point1, point2, point3, default_colors[color_index++]);
    if(color_index >= color_array_length)
      color_index = 0;
  }
}

void Draw_Space_Polygon(vec3 p1, vec3 p2, vec3 p3, Color color, DrawMode mode) {
  /*
    new version with the goal to implement a depth buffer  
    z value will be handled with depth interpolation

    code structure will probable turn bad since i have to make maths with space stuff
    with the rasterizer which was previously only working on screenspace stuff
  */


  uint32_t loaded_color = Load_Pixel(color, &vinfo);

  vec3 point1 = p1;
  vec3 point2 = p2;
  vec3 point3 = p3;


  if(mode == WIREFRAME) {
    // depth doesnt matter
    Draw_Line_Space((int)point1.x, (int)point1.y, point1.z, (int)point2.x, (int)point2.y, point2.z, loaded_color);
    Draw_Line_Space((int)point2.x, (int)point2.y, point2.z, (int)point3.x, (int)point3.y, point3.z, loaded_color);
    Draw_Line_Space((int)point3.x, (int)point3.y, point3.z, (int)point1.x, (int)point1.y, point1.z, loaded_color);
  }

  if(mode == FILL)
    Fill_Polygon_Space(point1, point2, point3, loaded_color);

  if(mode == COLOR_FRAME) {
    Draw_Line((int)point1.x, (int)point1.y, (int)point2.x, (int)point2.y, default_colors[(color_index++) % color_array_length]);  // The last arguments indexes,
    Draw_Line((int)point2.x, (int)point2.y, (int)point3.x, (int)point3.y, default_colors[(color_index++) % color_array_length]);  // i dont like this but its a 
    Draw_Line((int)point3.x, (int)point3.y, (int)point1.x, (int)point1.y, default_colors[(color_index++) % color_array_length]);  // way to reset the index when its big
  }

  if(mode == COLOR_FILL) {
    Fill_Polygon_Space(point1, point2, point3, default_colors[color_index++]);
    if(color_index >= color_array_length)
      color_index = 0;
  }
}


void Clean_Polygon_Drawer() {
  free(default_colors);
}