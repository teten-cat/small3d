#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <math.h>
#include <stdlib.h>

#include "math/linearmath.h"
#include "brush.h"
#include "math/vec3.h"
#include "rasterizer.h"
#include "polygon.h"
#include "mesh.h"
#include "window.h"
#include "buffer_manager.h"
#include "display_objects.h"
#include "depth_buffer.h"
#include "obj_reader.h"

#define TARGET_FPS 10

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

  // printf("%d %d %d\n", red.offset, red.length, red.msb_right);
  // printf("%d %d %d\n", green.offset, green.length, green.msb_right);
  // printf("%d %d %d\n", blue.offset, blue.length, blue.msb_right);
  // printf("%d %d %d\n", transp.offset, transp.length, transp.msb_right);

  
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
  
  Color white;
  white.r = white.g = white.b = 255;
  
  uint32_t loaded_white;
  loaded_white = Load_Pixel(white, &vinfo);  
  
  Color black;
  black.r = black.g = black.b = 0;
  uint32_t loaded_black;
  loaded_black = Load_Pixel(black, &vinfo);
  
  
  char imported_file_name[200];
  int isImportingOBJ = 0;
  FILE *imported_obj;
  Mesh *Import_mesh = malloc(sizeof(Mesh));
  
  int isColorMode = 0;

if(argc >= 2) {
    if(strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
      printf("default mug: %s\n", argv[0]);
      printf("imported .obj file: %s --obj FILENAME\n", argv[0]);
      printf("add --color on the end to use colored wireframe\n");
      exit(0);
    }

    if(strcmp(argv[1], "--obj") == 0) {
      strncpy(imported_file_name, argv[2], sizeof(char)*199);
      imported_file_name[199] = '\0';
      printf("FILENAME is: %s\n", imported_file_name);

      imported_obj = fopen(imported_file_name, "r");
      if(imported_obj != NULL) {
        isImportingOBJ = 1;


        *Import_mesh = OBJ_Parse(imported_file_name);
        // Print_Mesh(*Import_mesh);
        // Print_WHOLE_Vert(Import_mesh);

        Normal_Mesh(Import_mesh);

        // Print_Mesh(*Import_mesh);

      } else {
        perror("Failed to find import target file.");
        return 1;
      }
    }

    printf("this point reached\n");
    if(strcmp(argv[1], "--color") == 0 || strcmp(argv[1], "--colour") == 0) { // im very inclusive
      isColorMode = 1;
    }
    if(argc >= 4) {
      if(strcmp(argv[3], "--color") == 0 || strcmp(argv[3], "--colour") == 0)
        isColorMode = 1;
    }
    printf("this point ALSO reached\n");
  }

  printf("isImportingOBJ is %d\n", isImportingOBJ);

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

  // both works here. top is the hand typed positions bottom is imported version
  // Mesh UnitCube_mesh = {
  //   .vertices = UnitCube_vertices,
  //   .vertices_length = sizeof(UnitCube_vertices) / sizeof(UnitCube_vertices[0]),
  //   .indices = Cube_indices,
  //   .indices_length = sizeof(Cube_indices) /(sizeof(Cube_indices[0]) * 3)
  // };
  Mesh UnitCube_mesh = OBJ_Parse("import/CubeTessalated.obj");
  DisplayObject Cube = {
    .mesh = &UnitCube_mesh,
    .position = Zero_vec3(),
    .rotation = Zero_vec3(),
    .scale = One_vec3(),
    .color = white,
    .mode = WIREFRAME
  };

  Mesh Mug_mesh = OBJ_Parse("import/ATessellatedMug.obj");

  Normal_Mesh(&Mug_mesh); // make it's bounding box into a 1x1x1 cube
  DisplayObject Mug = {
    .mesh = &Mug_mesh,
    .position = Zero_vec3(),
    .rotation = Zero_vec3(),
    .scale = One_vec3(),
    .color = white,
    .mode = WIREFRAME
  };

  DisplayObject* imported_diplayobj = malloc(sizeof(DisplayObject));
  if(isImportingOBJ) {
    imported_diplayobj->mesh = Import_mesh;
    imported_diplayobj->position = Zero_vec3();
    imported_diplayobj->rotation = Zero_vec3();
    imported_diplayobj->scale = One_vec3();
    imported_diplayobj->color = white;
    imported_diplayobj->mode = WIREFRAME;
  }

  DisplayObject* main_object;
  if(isImportingOBJ) {
    main_object = imported_diplayobj;
  } else {
    main_object = &Mug;
  }

  if(isColorMode) {
    main_object->mode = COLOR_FRAME;
  }
  
  /*
  RENDER LOOP
  CLEAR BUFFER
  RENDER ON BUFFER
  
  SWAP BUFFER
  POOL RENDER
  */

  // render loop
  float delta_time = 0;
  
  float angle = 5;

  // clear the whole screen first
  Clear_Buffer(fbp, screensize);

  int targetFPS = TARGET_FPS;

  int frame_interval = 1000000 / targetFPS;
  printf("target fps: %d\n", targetFPS);
  // printf("frame interval: %d\n", frame_interval);
  
  mat4 proj = Perspective(rad_from_deg(80), 1.333, 0.1, 10000);

  Set_ProjectionMatrix(proj);
  
  DisplayObject Cube_red = Cube;
  Cube_red.color = StartColor;


  Cube.position.x = -1;
  Cube.position.y = -1;
  Cube_red.position.x = 1;
  Cube_red.position.y = 1;


  Cube.position.z = -3;
  Cube_red.position.z = -3;


  // adjustments to make it visible (0, 0, 0) is the camera position so it shoves the object in the camera's face
  Mug.scale = Fill_vec3(2.5);
  Mug.position.z = -5;
  Mug.position.y = -1;



  main_object->scale = Fill_vec3(2.5);
  main_object->position.z = -5;
  main_object->position.y = -1;


  while(render_loop) {
    Clear_Window(&w);
    Clear_Depth();
    
    Reset_Polygon_Colors();
    
  // ----------------------------------------------------------

  main_object->rotation.y += 5 * ONE_DEG_IN_RAD;
  Render(*main_object);

  // ----------------------------------------------------------
  
    Send_Buffer(fbp, buffer2, screensize);
    usleep(frame_interval);

  }

  printf("clearing up\n");
  Clear_Mesh(&Mug_mesh);

  Clean_Polygon_Drawer();
  munmap(fbp, screensize);
  free(buffer2);

  if(isImportingOBJ == 1) fclose(imported_obj);

  close(fd);
  return 0;
}
