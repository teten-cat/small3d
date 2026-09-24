#include "../triangulate.h"
#include "../double_linkedlist.h"
#include "../math/vec3.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define ALLOWED_ERROR 1e-4f

vec3 Get_Normal_Of_Polygon(Vertex* v_list, int* indices, int indices_length) {
  vec3 N = Zero_vec3();
  for(int i = 0; i < indices_length; i++) {
      vec3 v1 = v_list[indices[i] - 1].position;
      vec3 v2 = v_list[indices[(i + 1) % indices_length] - 1].position;
      N.x += (v1.y - v2.y) * (v1.z + v2.z);
      N.y += (v1.z - v2.z) * (v1.x + v2.x);
      N.z += (v1.x - v2.x) * (v1.y + v2.y);
  }

  return N;
}

vec2* Project(Vertex* v_list, int* indices, int indices_length) {
  vec2* projected = (vec2*)malloc(sizeof(vec2) * indices_length);

  vec3 Face_Normal = Get_Normal_Of_Polygon(v_list, indices, indices_length);

  float length_x = Face_Normal.x * Face_Normal.x;
  float length_y = Face_Normal.y * Face_Normal.y;
  float length_z = Face_Normal.z * Face_Normal.z;

  float max_length = (length_x >= length_y && length_x >= length_z) ? length_x : ((length_y >= length_z) ? length_y : length_z);

  for(int i = 0; i < indices_length; i++) {
    if(max_length == length_x) {
      projected[i] = (vec2){.x = v_list[i].position.y, .y = v_list[i].position.z};
    } else if(max_length == length_y) {
      projected[i] = (vec2){.x = v_list[i].position.z, .y = v_list[i].position.x};
    } else if(max_length == length_z) {
      projected[i] = (vec2){.x = v_list[i].position.x, .y = v_list[i].position.y};
    }
  }

  float proj_sign = (max_length == length_x) ? Face_Normal.x : ((max_length == length_y) ? Face_Normal.y : Face_Normal.z);
  if (proj_sign < 0.0f) {
    for (int i = 0; i < indices_length; i++) {
      projected[i].y = -projected[i].y; 
    }
  }

  return projected;
}


int Is_Point_In_Triangle(vec2 point, vec2 p1, vec2 p2, vec2 p3) {
  vec2 lineAB = Sub_vec2(p2, p1);
  vec2 lineBC = Sub_vec2(p3, p2);
  vec2 lineCA = Sub_vec2(p1, p3);

  vec2 lineAP = Sub_vec2(point, p1);

  int cp1 = Cross_vec2(lineAB, lineAP);

  vec2 lineBP = Sub_vec2(point, p2);

  int cp2 = Cross_vec2(lineBC, lineBP);

  vec2 lineCP = Sub_vec2(point, p3);

  int cp3 = Cross_vec2(lineCA, lineCP);

  int any_neg = (cp1 < 0) || (cp2 < 0) || (cp3 < 0);
  int any_pos = (cp1 > 0) || (cp2 > 0) || (cp3 > 0);

  return !(any_neg && any_pos);
}


// int Is_Point_In_Triangle(vec3 point, vec3 p1, vec3 p2, vec3 p3) {
//   vec3 lineAB = Sub_vec3(p2, p1);
//   vec3 lineBC = Sub_vec3(p3, p2);
//   vec3 lineCA = Sub_vec3(p1, p3);

//   vec3 perpendicular = Cross_vec3(lineAB, lineBC);

//   vec3 lineAP = Sub_vec3(point, p1);
//   vec3 cp1 = Cross_vec3(lineAB, lineAP);

//   // if AP isnt orthogonal to the perpendicular vector -> not on triangle
//   if(fabs(Dot_vec3(lineAP, perpendicular)) > ALLOWED_ERROR) {
//     return 0;
//   }

//   vec3 lineBP = Sub_vec3(point, p2);
//   vec3 cp2 = Cross_vec3(lineBC, lineBP);

//   vec3 lineCP = Sub_vec3(point, p3);
//   vec3 cp3 = Cross_vec3(lineCA, lineCP);

//   int d1 = Dot_vec3(cp1, perpendicular);
//   int d2 = Dot_vec3(cp2, perpendicular);
//   int d3 = Dot_vec3(cp3, perpendicular);

//   int any_neg = (d1 < ALLOWED_ERROR) || (d2 < ALLOWED_ERROR) || (d3 < ALLOWED_ERROR);
//   int any_pos = (d1 > -ALLOWED_ERROR) || (d2 > -ALLOWED_ERROR) || (d3 > -ALLOWED_ERROR);

//   return !(any_neg && any_pos);
// }

// returns 1 if the angle at p2 is convex CounterClockwise

float angleCCW(vec2 a, vec2 b) {
  float dot = Dot_vec2(a, b);
  float det = Cross_vec2(a, b);
  float angle = atan2(det, dot);
  if(angle < 0) {
    angle = 2*M_PI + angle;
  }
  return angle;
}



int Is_CCWAngle_Convex(vec2 p1, vec2 p2, vec2 p3) {
  vec2 a = Sub_vec2(p1, p2);
  vec2 b = Sub_vec2(p3, p2);
  float internal_angle = angleCCW(b, a);

  return internal_angle <= M_PI;
};



// int Is_Point_Ear(Vertex* v_list, Node** head, int indices_length,  vec3 p1, vec3 p2, vec3 p3) {
//   // is ear if "NOT CONVEX" and "NO OTHER VERT IS IN TRIANGLE"
//   if (Is_CCWAngle_Convex(p1, p2, p3) == 0)  return 0;

//   Node* tmp = *head;

//   printf("trying ears\n");
//   Print_vec3(p1);
//   Print_vec3(p2);
//   Print_vec3(p3);

//   // the linked list is cyclic so loop until its back should work
//   do {
//     vec3 subject = v_list[tmp->val-1].position;
//     if(Equal_vec3(subject, p1) ||
//        Equal_vec3(subject, p2) ||
//        Equal_vec3(subject, p3)) {
//         tmp = tmp->next;
//         continue;
//     }
//     if(Is_Point_In_Triangle(subject, p1, p2, p3)) {
//       printf("IN TRIANGLE\n");
//       return 0;
//     }

//     tmp = tmp->next;
//   } while(tmp != *head);

//   return 1;
// }


int* Triangulate(Vertex* v_list, int* indices, int indices_length) {
  // v_list : pointer to the array that holds ALL pointer information. to get positions from indexes
  // indices : indices of vertices that are in this face

  int* output = NULL;
  int output_length = 0;

  int available_vertexes = indices_length;

  Node* head = NULL;
  for(int i = 0; i < available_vertexes; i++) {
    // indices are 1 basd but vert array is zero based
    Insert_Tail_Node(&head, indices[i]-1);
  };
  Print_List(head);

  vec2* projected = Project(v_list, indices, indices_length);
  printf("projection conplete\n");

  Node** node = &head;
  while(available_vertexes > 2) {
    printf("available indices %d ", available_vertexes);
    Print_List(head);

    int i = (*node)->prev->val;
    int j = (*node)->val;
    int k = (*node)->next->val;

    vec2 vert_prev = projected[i];
    vec2 vert_curr = projected[j];
    vec2 vert_next = projected[k];

    int is_convex = Is_CCWAngle_Convex(vert_prev, vert_curr, vert_next);
    int is_ear = 1; // as True

    if(is_convex) {
      // printf("is convex\n");
      Node* test_node = (*node)->next->next;
      while(test_node != (*node)->prev && is_ear) {
        printf("check ear");
        vec2 vert_test = projected[test_node->val];
        is_ear = !(Is_Point_In_Triangle(vert_prev, vert_curr, vert_next, vert_test));
        test_node = test_node->next;
      }
    } else {
      is_ear = 0;
    }

    Node* next_node = (*node)->next;

    if(is_ear) {
      printf("ear found\n");
      output = (int*)malloc((output_length+3) * sizeof(int));
      output[output_length] = i;
      output[output_length + 1] = j;
      output[output_length + 2] = k;
      Delete_Node_Value(&head, (*node)->val);
      available_vertexes--;
    }

    (*node) = next_node;
  }

  return output;
}

int main() {
  /*
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

  int* indices = Triangulate(vertexes, this_face, vert_count);
  
  for(int i = 1; i <= 300; i++) {
    printf("%d ", indices[i]);
    if(i % 3 == 0)
      printf("\n");
  }

  Node* head = NULL;
  for(int i = 0; i < 12; i++) {
    Insert_Tail_Node(&head, this_face[i]);
  };
  */

  // Node** vtex = &head;
  // for(int i = 0; i < 12; i++) {
  //   if(Is_CCWAngle_Convex(vertexes[(*vtex)->prev->val].position, vertexes[(*vtex)->val].position, vertexes[(*vtex)->next->val].position)) {
  //     printf("CONVEX AT \n");
  //     Print_vec3(vertexes[(*vtex)->prev->val].position);
  //     Print_vec3(vertexes[(*vtex)->val].position);
  //     Print_vec3(vertexes[(*vtex)->next->val].position);
  //   }
  
  //   (*vtex) = (*vtex)->next;
  // }




  Vertex* square_vertex;
  square_vertex = (Vertex*)malloc(sizeof(Vertex) * 4);
  square_vertex[0] = (Vertex) {.position.x = -1.000000, .position.y =  0.000000, .position.z =  1.000000};
  square_vertex[1] = (Vertex) {.position.x = 1.000000, .position.y =  0.000000, .position.z =  1.000000};
  square_vertex[2] = (Vertex) {.position.x = 0.310711, .position.y =  0.000000, .position.z =  0.411410};
  square_vertex[3] = (Vertex) {.position.x = 1.000000, .position.y =  0.000000, .position.z =  -1.000000};

  int square_face[] = {1, 2, 4, 3};   // not convex at 1 based index 4 3 1
  int* square_indices = Triangulate(square_vertex, square_face, 4);


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