#ifndef MESH_H
#define MESH_H

#include "math/vector.h"
#include <stdlib.h>
// cause maybe i want to add stuff later like texture coordinates.
// just to leave potential for later
typedef struct {
  vec3 position;  
} Vertex;

typedef struct {
  Vertex vertex[3];
} Face;

typedef struct {
  Vertex* vertices;
  int vertices_length;
  int* indices;
  int indices_length; // CAUTION: THIS IS THE NUMBER OF FACES!!! VERY CONFUSING I KNOW
} Mesh;

void Print_Mesh(Mesh mesh);
void Print_WHOLE_Vert(Mesh* mesh);
void Print_WHOLE_Indc(Mesh* mesh);
void Normal_Mesh(Mesh* mesh);
void Clear_Mesh(Mesh* m);

#endif