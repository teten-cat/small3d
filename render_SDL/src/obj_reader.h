#ifndef OBJ_READER_H
#define OBJ_READER_H

#include "mesh.h"

void Print_Mesh(Mesh mesh);
Mesh OBJ_Parse(char *filename);

#endif