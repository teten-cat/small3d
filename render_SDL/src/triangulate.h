#ifndef TRIANGULATE_H
#define TRIANGULATE_H

#include "mesh.h"
#include "double_linkedlist.h"

// these two might should've been in the math library but wanted to keep it all in here since its only used here(for now but thats enough)
// int Is_Point_In_Triangle(vec3 point, vec3 p1, vec3 p2, vec3 p3);
// int Is_CCWAngle_Convex(vec3 p1, vec3 p2, vec3 p3);
// int Is_Point_Ear(Vertex** v_list, int* indices, vec3 p1, vec3 p2, vec3 p3);
// int Is_Point_Ear(Vertex** v_list, int* indices, int indices_length,  vec3 p1, vec3 p2, vec3 p3);
// int Is_Point_Ear(vec3 p1, vec3 p2, vec3 p3);
int Is_Point_Ear(Vertex* v_list, Node** head, int indices_length, vec3 p1, vec3 p2, vec3 p3);

// int* Triangulate(Vertex** v_list, int* indices);
int* Triangulate(Vertex* v_list, int* indices, int indices_length);



int Is_Point_In_Triangle(vec2 point, vec2 p1, vec2 p2, vec2 p3);
int Is_CCWAngle_Convex(vec2 p1, vec2 p2, vec2 p3);


#endif