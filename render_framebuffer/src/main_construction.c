// just a backup of main before i clear a lot of "work in progress" mess from main.c

#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#include "math/linearmath.h"
#include "brush.h"
#include "rasterizer.h"
#include "polygon.h"
#include "mesh.h"
#include "window.h"
#include "buffer_manager.h"
#include "display_objects.h"
#include "depth_buffer.h"
#include "obj_reader.h"

#define ONE_DEG_IN_RAD (M_PI / 180)


int render_loop = 1;

void SIGINT_handler(int sig) {
  render_loop = 0;
}

int main(int argc, char *argv[]) {
  int fd;
  char *fbp;
  struct fb_fix_screeninfo finfo;
  struct fb_var_screeninfo vinfo;

  signal(SIGINT, SIGINT_handler);


  fd = open("/dev/fb0", O_RDWR);
  if(fd == -1) {
    perror("Failed to open framebuffer device");
    return 1;
  }

  if(ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
    perror("Failed to get variable screen info");
    close(fd);
    return 1;
  }

  if(ioctl(fd, FBIOGET_FSCREENINFO, &finfo) == -1) {
    perror("Failed to get fixed screen info");
    close(fd);
    return 1;
  }

  // printf("Resolution: %dx%d\n", vinfo.xres, vinfo.yres);
  // printf("Bits per pixel: %d\n",vinfo.bits_per_pixel);
  // printf("Line length: %d\n", finfo.line_length); // length of a line in bytes
  // Resolution: 1920x1080
  // Bits per pixel: 32
  // Line length: 7680

  int width = vinfo.xres;
  int height = vinfo.yres;
  int bpp = vinfo.bits_per_pixel;
  int line_length = finfo.line_length;
  int screensize = line_length * height;

  struct fb_bitfield red = vinfo.red;
  struct fb_bitfield green = vinfo.green;
  struct fb_bitfield blue = vinfo.blue;
  struct fb_bitfield transp = vinfo.transp;

  printf("%d %d %d\n", red.offset, red.length, red.msb_right);
  printf("%d %d %d\n", green.offset, green.length, green.msb_right);
  printf("%d %d %d\n", blue.offset, blue.length, blue.msb_right);
  printf("%d %d %d\n", transp.offset, transp.length, transp.msb_right);


  fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if(fbp == MAP_FAILED) {
    perror("Failed to mmap frambuffer device");
    close(fd);
    return 1;
  }


  char *buffer2 = (char *)malloc(screensize);
  if(buffer2 == NULL) {
    perror("Failed to allocate second buffer. malloc");
    close(fd);
    return 1;
  }
  


  vec2int screen_origin = {30, 30};
  int WIDTH = 640;
  int HEIGHT = 480;

  // position of (0,0) in the polygon drawer
  vec2int coordinates_origin = {screen_origin.x, screen_origin.y};


  Color Blue1 = {56, 103, 234};
  uint32_t Default_Color = Load_Pixel(Blue1, &vinfo);

  Window w = {
    .fbp = buffer2,
    .line_length = line_length,
    .origin = screen_origin,
    .width = WIDTH,
    .height = HEIGHT,
    .Background = Default_Color
  };

  vec2int screen_middle = {w.width/2,w.height/2};

  float depth_buffer[w.width * w.height];
  memset(depth_buffer, 0, sizeof(depth_buffer));  // thought about using the Clear_Buffer but thats based on char arrays and doing 4*size seemed kinda risky


  Color colors[7] = {
    {.r = 171, .g = 86, .b = 117},
    {.r = 238, .g = 106, .b = 124},
    {.r = 255, .g = 167, .b = 165},
    {.r = 255, .g = 224, .b = 126},
    {.r = 255, .g = 231, .b = 214},
    {.r = 114, .g = 220, .b = 187},
    {.r = 52, .g = 172, .b = 186}
  };

  Init_Brush(&w);
  Init_Rasterizer(buffer2, &vinfo, line_length, w.origin);
  Init_Polygon_Drawer(buffer2,depth_buffer, &vinfo, line_length, coordinates_origin, colors, 7);
  Init_Object_Renderer(w.width, w.height);
  Init_Depth_Buffer(w.width, w.height);


  Color StartColor = {255, 0, 0};
  Color EndColor = {0, 0, 255};

  uint32_t loadedStart = Load_Pixel(StartColor, &vinfo);
  uint32_t loadedEnd = Load_Pixel(EndColor, &vinfo);



  // coordinates for a diamond
  vec2int diamond[] = {
    {100, 50},
    {150, 100},
    {100, 150},
    {50,  100}
  };

  vec2int square[] = {
    {50,  50},
    {150, 50},
    {150, 150},
    {50,  150}
  };

  Color white;
  white.r = white.g = white.b = 255;

  uint32_t loaded_white;
  loaded_white = Load_Pixel(white, &vinfo);  

  Color black;
  black.r = black.g = black.b = 0;
  uint32_t loaded_black;
  loaded_black = Load_Pixel(black, &vinfo);

  vec2int test_triangles[7][3] = {
    {{20, 60}, {60, 90}, {180, 10}}, // x
    {{100, 100}, {130, 150}, {30, 200}},
    {{150, 150}, {180, 100}, {250, 200}},
    {{300, 300}, {350, 400}, {300, 400}},
    {{300, 300}, {350, 300}, {350, 400}}, // x
    {{500, 500}, {550, 500}, {500, 700}},
    {{550, 500}, {550, 700}, {500, 700}}
  };
  
  Vertex pyramid_vertices[5] = {
    {-50.0, 0,  50.0},
    { 50.0, 0,  50.0},
    { 50.0, 0, -50.0},
    {-50.0, 0, -50.0},
    {    0,75,     0}
  };

  int pyramid_indices[] = {
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4,
    0, 1, 2,
    0, 2, 3
  };


  Vertex RectTube_vertices[8] = {
    {-100, 100, 0},
    {-100,-100, 0},
    { 100,-100, 0},
    { 100, 100, 0},
    {-100, 100, -400},
    {-100,-100, -400},
    { 100,-100, -400},
    { 100, 100, -400}
  };

  Vertex UnitCube_vertices[8] = {
    {-0.5, 0.5, 0.5},
    {-0.5,-0.5, 0.5},
    { 0.5,-0.5, 0.5},
    { 0.5, 0.5, 0.5},
    {-0.5, 0.5, -0.5},
    {-0.5,-0.5, -0.5},
    { 0.5,-0.5, -0.5},
    { 0.5, 0.5, -0.5}
  };

  Vertex Fish_vertices[6] = {
    {-20, 0, 0},
    {0, -20, 0},
    {20, 0, 0},
    {40, -20, 0},
    {40, 20, 0},
    {0, 20, 0}
  };

  int Cube_indices[] = {
    0, 1, 2,
    0, 2, 3,
    3, 2, 6,
    3, 6, 7,
    0, 1, 4, 
    1, 5, 4,
    4, 5, 7, 
    5, 6, 7,
    0, 3, 4,
    3, 4, 7,
    1, 5, 6,
    1, 2, 6
  };

  int Fish_indices[] = {
    0, 1, 2,
    2, 3, 4,
    0, 2, 5
  };


  Mesh pyramid_mesh = {
    .vertices = pyramid_vertices,
    .vertices_length = sizeof(pyramid_vertices) / sizeof(pyramid_vertices[0]),
    .indices = pyramid_indices,
    .indices_length = sizeof(pyramid_indices) /(sizeof(pyramid_indices[0]) * 3)
  };
  DisplayObject pyramid = {
    .mesh = &pyramid_mesh,
    .position = {0,0,-100},
    .rotation = vec3_rad_from_deg((vec3){48, 0, 0}),
    .scale = Fill_vec3(0.2),
    .color = white,
    .mode = COLOR_FRAME
  };

  Mesh RectTube_mesh = {
    .vertices = RectTube_vertices,
    .vertices_length = sizeof(RectTube_vertices) / sizeof(RectTube_vertices[0]),
    .indices = Cube_indices,
    .indices_length = sizeof(Cube_indices) /(sizeof(Cube_indices[0]) * 3)
  };
  DisplayObject RectTube = {
    .mesh = &RectTube_mesh,
    .position = {5,0,-100},
    .rotation = vec3_rad_from_deg((vec3){0, 0, 0}),
    .scale = Fill_vec3(0.2),
    .color = white,
    .mode = COLOR_FRAME
  };

  // Mesh UnitCube_mesh = {
  //   .vertices = UnitCube_vertices,
  //   .vertices_length = sizeof(UnitCube_vertices) / sizeof(UnitCube_vertices[0]),
  //   .indices = Cube_indices,
  //   .indices_length = sizeof(Cube_indices) /(sizeof(Cube_indices[0]) * 3)
  // };
  Mesh UnitCube_mesh = OBJ_Parse("../import/cube.obj");
  // Print_Mesh(UnitCube_mesh);
  // printf("\n\n");
  // for(int i = 0; i < UnitCube_mesh.vertices_length; i++) {
  //   Print_vec3(UnitCube_mesh.vertices->position);
  // }
  DisplayObject Cube = {
    .mesh = &UnitCube_mesh,
    .position = Zero_vec3(),
    .rotation = Zero_vec3(),
    .scale = One_vec3(),
    .color = white,
    .mode = WIREFRAME
  };

  Mesh Mug_mesh = OBJ_Parse("../import/ATessellatedMug.obj");

  Normal_Mesh(&Mug_mesh);
  DisplayObject Mug = {
    .mesh = &Mug_mesh,
    .position = Zero_vec3(),
    .rotation = Zero_vec3(),
    .scale = One_vec3(),
    .color = white,
    .mode = WIREFRAME
  };
  
  printf("MUGMESH");
  Print_Mesh(Mug_mesh);
  // for(int i = 0; i < Mug_mesh.vertices_length; i++) {
  //   Print_vec3(Mug_mesh.vertices->position);
  // }

  Mesh Fish_mesh = {
    .vertices = Fish_vertices,
    .vertices_length = sizeof(Fish_vertices) / sizeof(Fish_vertices[0]),
    .indices = Fish_indices,
    .indices_length = sizeof(Fish_indices) /(sizeof(Fish_indices[0]) * 3)
  };
  DisplayObject Fish = {
    .mesh = &Fish_mesh,
    .position = Zero_vec3(),
    .rotation = Zero_vec3(),
    .scale = One_vec3(),
    .color = EndColor,
    .mode = COLOR_FILL
  };

  DisplayObject pyramid_small = pyramid;
  pyramid_small.position = (vec3){-150, 0, 0};
  pyramid_small.scale = Fill_vec3(0.7);
  DisplayObject pyramid_big = pyramid;
  pyramid_big.position = (vec3){150, 0, 0};
  pyramid_big.scale = Fill_vec3(1.5);


  
  /*
  RENDER LOOP
  CLEAR BUFFER
  RENDER ON BUFFER
  
  SWAP BUFFER
  POOL REDNER
  */

  
  // int frame = 0;
  
  // while(1) {
  //   memset(buffer2, 0, screensize);
    
  //   for(int i = 0; i < width; i++) {
  //     for(int j = 0; j < height; j++) {
  //       if(frame % 2 == 0) {
  //         Color_Pixel(buffer2, line_length, i, j, loadedStart);
  //       } else {
  //         Color_Pixel(buffer2, line_length, i, j, loadedEnd);
  //       }
  //     }
  //   }
  //   sleep(1);
  //   memcpy(fbp, buffer2, screensize);
  // }
  
  // render loop
  clock_t start, current, prev;
  start = clock();
  prev = start;
  float delta_time = 0;
  
  float fps;
  float max_fps = 0;
  float angle = 5;

  // clear the whole screen first
  Clear_Buffer(fbp, screensize);

  int targetFPS = 10;

  int frame_interval = 1000000 / targetFPS;
  printf("drawmode is %d\n", pyramid.mode);
  printf("target fps: %d\n", targetFPS);
  printf("frame interval: %d\n", frame_interval);

  
  mat4 proj = Perspective(rad_from_deg(80), 1.333, 0.1, 10000);

  printf("proj is \n");
  Print_mat4(proj);
  

  Set_ProjectionMatrix(proj);

  vec2int top_left_right = {50, 0};
  vec2int top_left_corner = {0, 0};
  vec2int top_left_bottom = {0, 50};

  
  
  vec2int middletest_up;
  vec2int middletest_down;
  vec2int middletest_left;
  vec2int middletest_right;
  middletest_left = middletest_down = middletest_right = middletest_up = screen_middle;
  int middletestlength = 30;
  middletest_up.y -= middletestlength;
  middletest_down.y += middletestlength;
  middletest_left.x -= middletestlength;
  middletest_right.x += middletestlength;
  
  float depthtest_left_z = 1;
  float depthtest_right_z = 0;
  vec3 depth_testl0 = {10, 150, depthtest_left_z};
  vec3 depth_testl1 = {120, 250, depthtest_left_z};
  vec3 depth_testl2 = {120, 150, depthtest_left_z};
  
  vec3 depth_testr0 = {100, 200, depthtest_right_z};
  vec3 depth_testr1 = {100, 300, depthtest_right_z};
  vec3 depth_testr2 = {150, 300, depthtest_right_z};
  
  
  DisplayObject Cube_red = Cube;
  Cube_red.color = StartColor;


  

  Fish.scale = Fill_vec3(0.05);
  Fish.scale.y *= 0.5;

  Fish.position.z = -5;

  float fish_speed = 0.05;

  // printf("fish position\n");
  // Print_vec3(Fish.position);
  // printf("fish scale\n");
  // Print_vec3(Fish.scale);
  // printf("fish rotation\n");
  // Print_vec3(Fish.rotation);

  Fish.position.x = 0;
  
  enum Fish_State {
    swimming, turning
  };

  enum Fish_State state = swimming;

  float angle_sum = 0;
  float angle_swim = 0;

  Cube.position.x = -1;
  Cube.position.y = -1;
  Cube_red.position.x = 1;
  Cube_red.position.y = 1;


  Cube.position.z = -3;
  Cube_red.position.z = -3;

  Mug.position.z = -3;
  Mug.position.y = -1;

  // Print_WHOLE_Indc(&Mug_mesh);
  // Print_WHOLE_Vert(&Mug_mesh);

  while(render_loop) {
    Clear_Window(&w);
    Clear_Depth();
    
    Reset_Polygon_Colors();
    
    // ----------------------------------------------------------
    
    // Render(Fish);
    // switch (state) {
    //   case swimming:
    //     Fish.position.x -= fish_speed;
    //     Fish.position.y = 20 * sinf(rad_from_deg(angle_swim));
    //     angle_swim += rad_from_deg(10);
    //     if(Fish.position.x >= 3.5 || Fish.position.x <= -3.5)
    //       state = turning;
    //     break;
    //   case turning:
    //     Fish.rotation.y = rad_from_deg(angle);
    //     angle += 20;
    //     angle_sum += 20;
    //     if(angle_sum >= 180) {
    //       angle_sum = 0;
    //       fish_speed *= -1; 
    //       state = swimming;
    //     }
    //     break;
    // }
    /*
    Fish.rotation.y = rad_from_deg(angle);
    Fish.position.y = 0.25*sinf(rad_from_deg(angle));
    */
  // float cube_pos = sinf(rad_from_deg(angle));
  // if(cube_pos >= 0)
  //   cube_pos *= -1;
  // cube_away.position.x = cube_pos;
  // angle += 5;

  // Cube.rotation.y += 5 * ONE_DEG_IN_RAD;
  // Cube_red.rotation.y += 5 * ONE_DEG_IN_RAD;
  
  // Render(Cube);
  // Render(Cube_red);

    Mug.rotation.y += 5 * ONE_DEG_IN_RAD;
    Render(Mug);

  // Render(cube_away);
  // Cube.rotation.y += 5 * ONE_DEG_IN_RAD;
  
/*
  
  Fill_Polygon(top_left_corner, top_left_bottom, top_left_right, loaded_white);

  Draw_Line(0, 0, 50, 0, loaded_white);
  Draw_Line(50,0, 50, 50, loaded_white);
  Draw_Line(50,50, 0, 50, loaded_white);
  Draw_Line(0, 50, 0, 0, loaded_white);

  Color_Band(buffer2, line_length, 0, 120, 50, loadedStart);


  for(int y = 0; y < 30; y++) {
    for(int x = 0; x < 30; x++) {
      float t = (float)y / (30 - 1);
  
      Color pos_color = {
        .r = (uint8_t)(StartColor.r + t * (EndColor.r - StartColor.r)),
        .g = (uint8_t)(StartColor.g + t * (EndColor.g - StartColor.g)),
        .b = (uint8_t)(StartColor.b + t * (EndColor.b - StartColor.b))
      };
      
      uint32_t loaded_color = Load_Pixel(pos_color, &vinfo);
      Color_Pixel(buffer2, line_length, x, y, loaded_color);
    }
  }

*/

  // Draw_Space_Polygon(depth_testr0, depth_testr1, depth_testr3, StartColor, FILL);

  // top red triangle
  Fill_Polygon_Space(depth_testr0, depth_testr1, depth_testr2, loadedStart);
  // bottom blue triangle
  Fill_Polygon_Space(depth_testl0, depth_testl1, depth_testl2, loadedEnd);

  // middle crosshair
  //Fill_Polygon(middletest_down, middletest_up, middletest_left, loadedEnd);
  //Fill_Polygon(middletest_down, middletest_up, middletest_right, loadedEnd);

  // RectTube.rotation.z += 5 * ONE_DEG_IN_RAD;
  // Render(RectTube);

  // FOR CENTER CHECK. right now the center is the middle of the window
  // Draw_Polygon((vec2int){-50, 50}, (vec2int){50, 50}, (vec2int){50, -50}, white, FILL);

  /*
  pyramid.rotation.x += 5 * ONE_DEG_IN_RAD;
  pyramid_small.rotation.y += 5 * ONE_DEG_IN_RAD;
  pyramid_big.rotation.z += 5 * ONE_DEG_IN_RAD;
  */
  // Render(pyramid);
  // Render(pyramid_small);
  // Render(pyramid_big);


  // ----------------------------------------------------------
  
    //Show_Depth(buffer2, line_length, &vinfo);

    //Print_WHOLE_DepthBuffer();

    Send_Buffer(fbp, buffer2, screensize);
    usleep(frame_interval);
    prev = current;

  }

  printf("clearing up\n");
  Clear_Mesh(&Mug_mesh);

  Clean_Polygon_Drawer();
  munmap(fbp, screensize);
  free(buffer2);

  close(fd);
  return 0;
}