#include <SDL2/SDL_events.h>
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

#include <SDL2/SDL.h>

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

#define TARGET_FPS 31
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


#define ONE_DEG_IN_RAD (M_PI / 180)

int render_loop = 1;


int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window* window = SDL_CreateWindow("Raw Pixel Buffer Test",
                                      100,
                                      100,
                                      SCREEN_WIDTH, SCREEN_HEIGHT,
                                      SDL_WINDOW_SHOWN);
  if (!window) {
      SDL_Quit();
      return 1;
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
  SDL_Texture* texture = SDL_CreateTexture(renderer, 
                                              SDL_PIXELFORMAT_RGBA8888, 
                                              SDL_TEXTUREACCESS_STREAMING, 
                                              SCREEN_WIDTH, SCREEN_HEIGHT);

  uint32_t* pixel_buffer = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint32_t));
  
  if(pixel_buffer == NULL) {
    perror("Failed to allocate second buffer. malloc");
    return 1;
  }
  
  int quit = 0;
  SDL_Event e;
  
  
  vec2int screen_origin = {30, 30};
  int screensize = SCREEN_HEIGHT * SCREEN_WIDTH;
  
  // position of (0,0) in the polygon drawer
  vec2int coordinates_origin = {screen_origin.x, screen_origin.y};
  
  
  Color Blue1 = {56, 103, 234};
  uint32_t Default_Color = Load_Pixel(Blue1);
  
  Window w = {
    .pixel_buffer = pixel_buffer,
    .origin = screen_origin,
    .width = SCREEN_WIDTH,
    .height = SCREEN_HEIGHT,
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
  Init_Rasterizer(pixel_buffer, SCREEN_WIDTH, w.origin);
  Init_Polygon_Drawer(pixel_buffer, depth_buffer, SCREEN_WIDTH, coordinates_origin, colors, 7);
  Init_Object_Renderer(w.width, w.height);
  Init_Depth_Buffer(w.width, w.height);
  
  
  Color StartColor = {255, 0, 0};
  Color EndColor = {0, 0, 255};
  
  Color white;
  white.r = white.g = white.b = 255;
  
  uint32_t loaded_white;
  loaded_white = Load_Pixel(white);
  
  Color black;
  black.r = black.g = black.b = 0;
  uint32_t loaded_black;
  loaded_black = Load_Pixel(black);
  
  
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
  Clear_Buffer(pixel_buffer, SCREEN_HEIGHT * SCREEN_WIDTH);

  int targetFPS = TARGET_FPS;

  int frame_interval = 1000 / targetFPS;
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
  // Mug.scale = Fill_vec3(2.5);
  // Mug.position.z = -5;
  // Mug.position.y = -1;



  main_object->scale = Fill_vec3(2.5);
  main_object->position.z = -5;
  main_object->position.y = -1;
  // main_object->rotation.z = ONE_DEG_IN_RAD * 90;


  while(!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = 1;
      } else if(e.type == SDL_KEYDOWN) {
        if(e.key.keysym.sym == SDLK_q) {
          quit = 1;
        }
      }
    }

    // these two are replaced by SDL
    Clear_Window(&w);
    // Clear_Depth();
    // memset(pixel_buffer, Default_Color, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint32_t));

    
    Reset_Polygon_Colors();
    
  // ----------------------------------------------------------
  // OBJECT POSITION STUFF
  
  main_object->rotation.y += 5 * ONE_DEG_IN_RAD;
  Render(*main_object);
 


  // ----------------------------------------------------------
  
    // sends pixel buffer to GPU
    SDL_UpdateTexture(texture, NULL, pixel_buffer, SCREEN_WIDTH * sizeof(uint32_t));
    
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture, NULL, NULL);

    SDL_RenderPresent(renderer);
    
    // interval
    SDL_Delay(frame_interval); 
  }

  printf("clearing up\n");
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  Clear_Mesh(&Mug_mesh);

  Clean_Polygon_Drawer();
  if(isImportingOBJ == 1) fclose(imported_obj);

  return 0;
}