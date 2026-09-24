#include "triangulate.h"
#include "doublelinkedlist.h"

int Is_Point_In_Triangle(vec3 p1, vec3 p2, vec3 p3);
int Is_CCWAngle_Convex(vec3 p1, vec3 p2, vec3 p3);

int Is_Point_Ear(vec3 p1, vec3 p2, vec3 p3);

// 
int* Triangulate(Vertex** v_list, int* indices) {
  // v_list : pointer to the array that holds ALL pointer information. to get positions from indexes
  // indices : indices of vertices that are in this face

  Node* head = NULL;
  
}
