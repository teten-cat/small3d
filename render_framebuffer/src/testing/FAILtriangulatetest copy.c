#include "../triangulate.h"
#include "../double_linkedlist.h"
#include "../math/vec3.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define ALLOWED_ERROR 1e-4f

int Is_Point_In_Triangle(vec3 point, vec3 p1, vec3 p2, vec3 p3) {
  vec3 lineAB = Sub_vec3(p2, p1);
  vec3 lineBC = Sub_vec3(p3, p2);
  vec3 lineCA = Sub_vec3(p1, p3);

  vec3 perpendicular = Cross_vec3(lineAB, lineBC);

  vec3 lineAP = Sub_vec3(point, p1);
  vec3 cp1 = Cross_vec3(lineAB, lineAP);

  // if AP isnt orthogonal to the perpendicular vector -> not on triangle
  if(fabs(Dot_vec3(lineAP, perpendicular)) > ALLOWED_ERROR) {
    return 0;
  }

  vec3 lineBP = Sub_vec3(point, p2);
  vec3 cp2 = Cross_vec3(lineBC, lineBP);

  vec3 lineCP = Sub_vec3(point, p3);
  vec3 cp3 = Cross_vec3(lineCA, lineCP);

  int d1 = Dot_vec3(cp1, perpendicular);
  int d2 = Dot_vec3(cp2, perpendicular);
  int d3 = Dot_vec3(cp3, perpendicular);

  int any_neg = (d1 < ALLOWED_ERROR) || (d2 < ALLOWED_ERROR) || (d3 < ALLOWED_ERROR);
  int any_pos = (d1 > -ALLOWED_ERROR) || (d2 > -ALLOWED_ERROR) || (d3 > -ALLOWED_ERROR);

  return !(any_neg && any_pos);
}

// returns 1 if the angle at p2 is convex CounterClockwise
int Is_CCWAngle_Convex(vec3 p1, vec3 p2, vec3 p3) {
  vec3 lineAB = Sub_vec3(p2, p1);
  vec3 lineBC = Sub_vec3(p3, p2);

  vec3 normal = Cross_vec3(lineAB, lineBC);

  // make it flat on one axis plane and woek likes it 2D. basic shape is the same so same results
  // get longest side of bounding box (square to work in absolute values)
  float length_x = normal.x * normal.x;
  float length_y = normal.y * normal.y;
  float length_z = normal.z * normal.z;

  // cross product in 2D
  float cp;
  float max_length = (length_x >= length_y && length_x >= length_z) ? length_x : ((length_y >= length_z) ? length_y : length_z);

  if(max_length == length_x) {
    cp = lineAB.y * lineBC.z - lineAB.z * lineBC.y;
  } else if(max_length == length_y) {
    cp = lineAB.z * lineBC.x - lineAB.x * lineBC.z;
  } else if(max_length == length_z) {
    cp = lineAB.x * lineBC.y - lineAB.y * lineBC.z;
  }
  
  return (cp >= 0);
};



int Is_Point_Ear(Vertex* v_list, Node** head, int indices_length,  vec3 p1, vec3 p2, vec3 p3) {
  // is ear if "NOT CONVEX" and "NO OTHER VERT IS IN TRIANGLE"
  if (Is_CCWAngle_Convex(p1, p2, p3) == 0)  return 0;

  Node* tmp = *head;

  printf("trying ears\n");
  Print_vec3(p1);
  Print_vec3(p2);
  Print_vec3(p3);

  // the linked list is cyclic so loop until its back should work
  do {
    vec3 subject = v_list[tmp->val-1].position;
    if(Equal_vec3(subject, p1) ||
       Equal_vec3(subject, p2) ||
       Equal_vec3(subject, p3)) {
        tmp = tmp->next;
        continue;
    }
    if(Is_Point_In_Triangle(subject, p1, p2, p3)) {
      printf("IN TRIANGLE\n");
      return 0;
    }

    tmp = tmp->next;
  } while(tmp != *head);

  return 1;
}


int* Triangulate(Vertex* v_list, int* indices, int indices_length) {
  // v_list : pointer to the array that holds ALL pointer information. to get positions from indexes
  // indices : indices of vertices that are in this face

  int* output = NULL;
  int output_length = 0;

  int available_count = indices_length;
  
  Node* head = NULL;
  for(int i = 0; i < indices_length; i++) {
    Insert_Tail_Node(&head, indices[i]);
  };
  Print_List(head);

  printf("prev of head has value %d\n", head->prev->val);

  Node** vertex = &head;

  Print_List(*vertex);

  printf("indices_length is %d\n", indices_length);
  for(int i = 0; i < indices_length; i++) {
    int index = (*vertex)->val;
    printf("%2d: index %d : value ", i, index);
    Print_vec3(v_list[index-1].position);
    *vertex = (*vertex)->next;
  }

  while(available_count > 2) {
    // printf("trying on index %d\n",(*vertex)->prev->val-1);
    // printf("trying on index %d\n",(*vertex)->val-1);
    // printf("trying on index %d\n",(*vertex)->next->val-1);

    int prev = (*vertex)->prev->val-1;
    int curr = (*vertex)->val-1;
    int next = (*vertex)->next->val-1;
    Print_List(*vertex);
    // for(int i = 0; i < available_count; i++) {
    //   int index = (*vertex)->val;
    //   printf("%2d: index %d : value ", i, index);
    //   Print_vec3(v_list[index-1].position);
    //   *vertex = (*vertex)->next;
    // }
    if(
      Is_Point_Ear(
        v_list, vertex, available_count,
         v_list[prev].position,
         v_list[curr].position,
         v_list[next].position
      )
    ) {

      output = (int*)realloc(output, (output_length + 3) * sizeof(int));
      output[output_length] = indices[prev];
      output[output_length+1] = indices[curr];
      output[output_length+2] = indices[next];
      output_length += 3;

      Delete_Node_Value(&head, (*vertex)->val);
      Print_List(head);
      available_count--;
    } else {
      (*vertex) = (*vertex)->next;
    }
  }
  return output;
}

int main() {
  Vertex* vertexes;
  vertexes = (Vertex*)malloc(sizeof(Vertex) * 12);
  vertexes[0] = (Vertex) {.position.x = 0.514589  ,.position.y = -1.070596 ,.position.z = 0.054911};
  vertexes[1] = (Vertex) {.position.x = -0.514589 ,.position.y = 0.794453  ,.position.z = 0.054911};
  vertexes[2] = (Vertex) {.position.x = -0.429538 ,.position.y = -0.948289 ,.position.z = 0.045836};
  vertexes[3] = (Vertex) {.position.x = -0.429538 ,.position.y = 0.571475  ,.position.z = 0.045836};
  vertexes[4] = (Vertex) {.position.x = 0.202415  ,.position.y = -0.948289 ,.position.z = -0.021600};
  vertexes[5] = (Vertex) {.position.x = 0.202415  ,.position.y = 0.571475  ,.position.z = -0.021600};
  vertexes[6] = (Vertex) {.position.x = 0.322381  ,.position.y = -0.866920 ,.position.z = -0.034401};
  vertexes[7] = (Vertex) {.position.x = 0.202451  ,.position.y = -0.948324 ,.position.z = -0.021604};
  vertexes[8] = (Vertex) {.position.x = 0.322381  ,.position.y = 0.590778  ,.position.z = -0.034401};
  vertexes[9] = (Vertex) {.position.x = 0.119848  ,.position.y = 0.794453  ,.position.z = -0.012789};
  vertexes[10] = (Vertex) {.position.x = 0.295247  ,.position.y = 0.692615  ,.position.z = -0.031506};
  vertexes[11] = (Vertex) {.position.x = 0.221114  ,.position.y = 0.767166  ,.position.z = -0.023595};

  int this_face[] = {5, 8, 7, 9, 11, 12, 10, 2, 1, 3, 4, 6};
  int vert_count = sizeof(this_face) / sizeof(this_face[0]);

  Node* head = NULL;
  for(int i = 0; i < 12; i++) {
    Insert_Tail_Node(&head, this_face[i]);
  };

  Node** vtex = &head;
  for(int i = 0; i < 12; i++) {
    if(Is_CCWAngle_Convex(vertexes[(*vtex)->prev->val].position, vertexes[(*vtex)->val].position, vertexes[(*vtex)->next->val].position)) {
      printf("CONVEX AT \n");
      Print_vec3(vertexes[(*vtex)->prev->val].position);
      Print_vec3(vertexes[(*vtex)->val].position);
      Print_vec3(vertexes[(*vtex)->next->val].position);
    }
  
    (*vtex) = (*vtex)->next;
  }

  // int* indices = Triangulate(vertexes, this_face, vert_count);

  // for(int i = 1; i <= 300; i++) {
  //   printf("%d ", indices[i]);
  //   if(i % 3 == 0)
  //     printf("\n");
  // }

  Vertex* square_vertex;
  square_vertex = (Vertex*)malloc(sizeof(Vertex) * 4);
  square_vertex[0] = (Vertex) {.position.x = -1.000000, .position.y =  0.000000, .position.z =  1.000000};
  square_vertex[1] = (Vertex) {.position.x = 1.000000, .position.y =  0.000000, .position.z =  1.000000};
  square_vertex[2] = (Vertex) {.position.x = 0.310711, .position.y =  0.000000, .position.z =  0.411410};
  square_vertex[3] = (Vertex) {.position.x = 1.000000, .position.y =  0.000000, .position.z =  -1.000000};

  int square_face[] = {1, 2, 4, 3};   // not convex at 1 based index 4 3 1

  // this will break
  // Print_vec3(square_vertex[square_face[2]].position);
/*
  if(Is_CCWAngle_Convex(square_vertex[square_face[0]-1].position, square_vertex[square_face[1]-1].position, square_vertex[square_face[2]-1].position)) {
    printf("trinagl eis convec on p2\n");
  } else {
    printf("trinagl eis not convec on p2\n");
    Print_vec3(square_vertex[square_face[0]-1].position);
    Print_vec3(square_vertex[square_face[1]-1].position);
    Print_vec3(square_vertex[square_face[2]-1].position);
  }

  if(Is_CCWAngle_Convex(square_vertex[square_face[1]-1].position, square_vertex[square_face[2]-1].position, square_vertex[square_face[3]-1].position)) {
    printf("trinagl eis convec on p2\n");
  } else {
    printf("trinagl eis not convec on p2\n");
    Print_vec3(square_vertex[square_face[1]-1].position);
    Print_vec3(square_vertex[square_face[2]-1].position);
    Print_vec3(square_vertex[square_face[3]-1].position);
  }

  // NOT CONVEX HERE
  if(Is_CCWAngle_Convex(square_vertex[square_face[2]-1].position, square_vertex[square_face[3]-1].position, square_vertex[square_face[0]-1].position)) {
    printf("trinagl eis convec on p2\n");
  } else {
    printf("trinagl eis not convec on p2\n");
    Print_vec3(square_vertex[square_face[2]-1].position);
    Print_vec3(square_vertex[square_face[3]-1].position);
    Print_vec3(square_vertex[square_face[0]-1].position);
  }

  if(Is_CCWAngle_Convex(square_vertex[square_face[3]-1].position, square_vertex[square_face[0]-1].position, square_vertex[square_face[1]-1].position)) {
    printf("trinagl eis convec on p2\n");
  } else {
    printf("trinagl eis not convec on p2\n");
    Print_vec3(square_vertex[square_face[3]-1].position);
    Print_vec3(square_vertex[square_face[0]-1].position);
    Print_vec3(square_vertex[square_face[1]-1].position);
  }
*/
  return 0;
}