#ifndef DISPLAY_OBJ_H
#define DISPLAY_OBJ_H

#include "math/vector.h"
#include "math/matrix.h"
#include "mesh.h"
#include "brush.h"
#include "polygon.h"

typedef struct {
  Mesh* mesh;
  vec3 position;
  vec3 rotation;
  vec3 scale;
  Color color;
  DrawMode mode;
} DisplayObject;

void Init_Object_Renderer(int screen_width, int screen_height);
void Set_ProjectionMatrix(mat4 m);
void Set_ViewMatrix(mat4 m);
void Render(DisplayObject obj);

#endif