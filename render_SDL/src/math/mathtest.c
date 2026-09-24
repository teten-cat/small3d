#include "affine.h"
#include "mat2.h"
#include "mat3.h"
#include "mat4.h"
#include "vec3.h"
#include "vec4.h"
#include "vector.h"
#include "matrix.h"
#include <math.h>
#include <string.h>

mat4 Test_mat4() {
  mat4 m;
  for(int i = 0; i < 16; i++) {
    m.arr[i] = i;
  }

  return m;
}

mat3 Test_mat3() {
  mat3 m;
  for(int i = 0; i < 9; i++) {
    m.arr[i] = i;
  }

  return m;
}

mat2 Test_mat2() {
  mat2 m;
  for(int i = 0; i < 4; i++) {
    m.arr[i] = i;
  }
  return m;
}

int main() {

  // mat2 m = Test_mat2();
  // Print_mat2(m);
  // Print_mat2(Adj_mat2(m));
  // printf("inverse is \n");
  // Print_mat2(Inverse_mat2(m));


  /*
  for(int i = 0; i < 70000000; i++) {
    mat4 m = Test_mat4();
    mat4 mm = Mul_mat4_Unrolled(m, m);
  }
  */
  /*
  vec3 original_coordinates = {.x = 2, .y = 5, .z = 8};
  printf("original coordinates\n");
  Print_vec3(original_coordinates);
  float angle = rad_from_deg(45);
  printf("rotate by %f\n", angle);
  mat4 rotate_matrix = Rotate_Z(angle);
  printf("rotate matrix\n");
  Print_mat4(rotate_matrix);
  
  vec4 new_coordinates = Mul_mat4_vec4(rotate_matrix, vec4_from_vec3(original_coordinates));
  printf("result");
  Print_vec4(new_coordinates);
  */
  /*
  original Coordiantes
  {2, 5, 8, 1}
  Result 
  {4, 8, 12, 1}
  */

/*  
  mat3 m = Identity_mat3();
  float values[9] = {5, 4, -2, 3, 4, 3, 2, 5, 1};
  memcpy(m.arr, values, sizeof(m.arr));
  Print_mat3(m);
  printf("det %f\n", Det_mat3(m));
  printf("inverse\n");
  Print_mat3(Inverse_mat3(m));
*/
  // printf("minor is \n");
  // for(int i = 0; i < 3; i++) {
  //   for(int j = 0; j < 3; j++) {
  //     printf("Minor row: %d, col: %d is \n", i, j);
  //     Print_mat2(Minor_mat3(m, i, j));
  //   }
  // }
  

/*
  // #? does col[0][0] work?
  printf("m.col[0].val[0] : %g\n", m.col[0].val[0]);
  // #? does col[0][1] mean col[0].y or col[1].x
  printf("m.var[0][1] : %g\n", m.var[0][1]);
  printf("col[0].y : %g\n", m.col[0].y);
  printf("col[1].x : %g\n", m.col[1].x);
*/

/*

./mathtest 
  0   4   8  12 
  1   5   9  13 
  2   6  10  14 
  3   7  11  15 
m.col[0].val[0] : 0
m.var[0][1] : 1
col[0].y : 1
col[1].x : 4

*/

  /*
  // only do declaring with one side of the declaration. struct or array.
  vec3 a = {.x = 10, .y = 20, .z = 5};
  vec3 b = {.val[0] = 15, .val[1] = 10, .val[2] = 20};

  a.val[0] = 11;
  a.val[1] = 22;
  a.val[2] = 33;

  Print_vec3(a);
  Print_vec3(b);
  */
/*
  Print_vec3(Zero_vec3());
  Print_vec3(One_vec3());

  printf("algebra\n");
  printf("ADD, ");
  Print_vec3(Add_vec3(a, b));
  printf("SUB, ");
  Print_vec3(Sub_vec3(a, b));
  printf("MUL, ");
  Print_vec3(Mul_vec3(a, b));
  printf("DIV, ");
  Print_vec3(Div_vec3(a, b));

  printf("negs of a\n");
  Print_vec3(NegX_vec3(a));
  Print_vec3(NegY_vec3(a));
  Print_vec3(NegZ_vec3(a));

  printf("Dot %f\n", Dot_vec3(a, b));
  printf("Cross ");
  Print_vec3(Cross_vec3(a, b));

  vec3 u1 = {.x = 5, .y = 0, .z = 0};
  u1 = Normal_vec3(u1);


  double theta = 0;
  for(int i = 0; i < 12; i++) {
    vec3 tmp = {.x = cos(theta), .y = sin(theta)};
    printf("Angle: %g\n", theta * 180 / M_PI);
    printf("Dot %.3f\n", Dot_vec3(u1, tmp));
    printf("Cross ");
    Print_vec3(Cross_vec3(u1, tmp));
    theta += M_PI / 12;
  }
*/
  return 0;
}