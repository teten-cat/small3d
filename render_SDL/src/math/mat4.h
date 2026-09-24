#ifndef MAT4_H
#define MAT4_H

#include "mat3.h"
#include "vec4.h"

/*
  4x4 matrix definition

vec4 col array
[ col[0].x  col[1].x  col[2].x  col[3].x ]
| col[0].y  col[1].y  col[2].y  col[3].y |
| col[0].z  col[1].z  col[2].z  col[3].z |
[ col[0].w  col[1].w  col[2].w  col[3].w ]

float array array

[ var[0][0] var[1][0] var[2][0] var[3][0]]
| var[0][1] var[1][1] var[2][1] var[3][1]|
| var[0][2] var[1][2] var[2][2] var[3][2]|
[ var[0][3] var[1][3] var[2][3] var[3][3]]


float arr array index
[ 0  4  8 12]
| 1  5  9 13|
| 2  6 10 14|
[ 3  7 11 15]

Thus
col[1].z = col[1].val[2] = var[1][2] = arr[6]

*/


typedef union {
  vec4 col[4];
  float var[4][4];  // value array - i know it sounds stupid. Im just running out of short words
  float arr[16];
} mat4;


static inline void Print_mat4(mat4 m) {
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      printf("%9g ", m.arr[4*j + i]);
    }
    printf("\n");
  }
}

static inline mat4 Fill_mat4(float val) {
  mat4 m;
  for(int i = 0; i < 16; i++) {
    m.arr[i] = val;
  }
  return m;
}

static inline mat4 Zero_mat4() {
  return Fill_mat4(0);
}

// returns the identity matrix. - m[i][j] = 0(i != j), 1(i == j)
static inline mat4 Identity_mat4() {
  mat4 m = Zero_mat4();
  m.var[0][0] = 1;
  m.var[1][1] = 1;
  m.var[2][2] = 1;
  m.var[3][3] = 1;
  return m;
}

// Matrix addition
static inline mat4 Add_mat4(mat4 a, mat4 b) {
  mat4 m;
  m.arr[0] = a.arr[0] + b.arr[0];
  m.arr[1] = a.arr[1] + b.arr[1];
  m.arr[2] = a.arr[2] + b.arr[2];
  m.arr[3] = a.arr[3] + b.arr[3];
  m.arr[4] = a.arr[4] + b.arr[4];
  m.arr[5] = a.arr[5] + b.arr[5];
  m.arr[6] = a.arr[6] + b.arr[6];
  m.arr[7] = a.arr[7] + b.arr[7];
  m.arr[8] = a.arr[8] + b.arr[8];
  m.arr[9] = a.arr[9] + b.arr[9];
  m.arr[10] = a.arr[10] + b.arr[10];
  m.arr[11] = a.arr[11] + b.arr[11];
  m.arr[12] = a.arr[12] + b.arr[12];
  m.arr[13] = a.arr[13] + b.arr[13];
  m.arr[14] = a.arr[14] + b.arr[14];
  m.arr[15] = a.arr[15] + b.arr[15];

  return m;
}

// Adds float to all values of matrix
static inline mat4 Plus_mat4(mat4 a, float b) {
  mat4 m = a;
  m.arr[0] += b;
  m.arr[1] += b;
  m.arr[2] += b;
  m.arr[3] += b;
  m.arr[4] += b;
  m.arr[5] += b;
  m.arr[6] += b;
  m.arr[7] += b;
  m.arr[8] += b;
  m.arr[9] += b;
  m.arr[10] += b;
  m.arr[11] += b;
  m.arr[12] += b;
  m.arr[13] += b;
  m.arr[14] += b;
  m.arr[15] += b;
  return m;
}

// Matrix subtraction
static inline mat4 Sub_mat4(mat4 a, mat4 b) {
  mat4 m;
  m.arr[0] = a.arr[0] - b.arr[0];
  m.arr[1] = a.arr[1] - b.arr[1];
  m.arr[2] = a.arr[2] - b.arr[2];
  m.arr[3] = a.arr[3] - b.arr[3];
  m.arr[4] = a.arr[4] - b.arr[4];
  m.arr[5] = a.arr[5] - b.arr[5];
  m.arr[6] = a.arr[6] - b.arr[6];
  m.arr[7] = a.arr[7] - b.arr[7];
  m.arr[8] = a.arr[8] - b.arr[8];
  m.arr[9] = a.arr[9] - b.arr[9];
  m.arr[10] = a.arr[10] - b.arr[10];
  m.arr[11] = a.arr[11] - b.arr[11];
  m.arr[12] = a.arr[12] - b.arr[12];
  m.arr[13] = a.arr[13] - b.arr[13];
  m.arr[14] = a.arr[14] - b.arr[14];
  m.arr[15] = a.arr[15] - b.arr[15];

  return m;
}

// Multiply all values in matrix a by float b
static inline mat4 Times_mat4(mat4 a, float b) {
  mat4 m = a;
  m.arr[0] *= b;
  m.arr[1] *= b;
  m.arr[2] *= b;
  m.arr[3] *= b;
  m.arr[4] *= b;
  m.arr[5] *= b;
  m.arr[6] *= b;
  m.arr[7] *= b;
  m.arr[8] *= b;
  m.arr[9] *= b;
  m.arr[10] *= b;
  m.arr[11] *= b;
  m.arr[12] *= b;
  m.arr[13] *= b;
  m.arr[14] *= b;
  m.arr[15] *= b;
  return m;
}
/*
Note: Unrapped version is about 3 times faster. tested with 1million and 70 million times
static inline mat4 Mul_mat4(mat4 a, mat4 b) {
  mat4 m;
  //rows
  for(int i = 0; i < 4; i++) {
    // cols
    for(int j = 0; j < 4; j++) {
      float sum = 0;
      for(int k = 0; k < 4; k++) {
        sum += (a.var[k][i] * b.var[j][k]);
      }
      m.var[j][i] = sum;
    }
  }
  return m;
}

 for this reason i made most simple functions not use for loops. looks cool too
*/

// Matrix multiplication
static inline mat4 Mul_mat4(mat4 a, mat4 b) {
  // copy pasted from cglm
  float   a00 = a.var[0][0], a01 = a.var[0][1], a02 = a.var[0][2], a03 = a.var[0][3],
          a10 = a.var[1][0], a11 = a.var[1][1], a12 = a.var[1][2], a13 = a.var[1][3],
          a20 = a.var[2][0], a21 = a.var[2][1], a22 = a.var[2][2], a23 = a.var[2][3],
          a30 = a.var[3][0], a31 = a.var[3][1], a32 = a.var[3][2], a33 = a.var[3][3],
          b00 = b.var[0][0], b01 = b.var[0][1], b02 = b.var[0][2], b03 = b.var[0][3],
          b10 = b.var[1][0], b11 = b.var[1][1], b12 = b.var[1][2], b13 = b.var[1][3],
          b20 = b.var[2][0], b21 = b.var[2][1], b22 = b.var[2][2], b23 = b.var[2][3],
          b30 = b.var[3][0], b31 = b.var[3][1], b32 = b.var[3][2], b33 = b.var[3][3];

  mat4 m;
  m.var[0][0] = a00 * b00 + a10 * b01 + a20 * b02 + a30 * b03;
  m.var[0][1] = a01 * b00 + a11 * b01 + a21 * b02 + a31 * b03;
  m.var[0][2] = a02 * b00 + a12 * b01 + a22 * b02 + a32 * b03;
  m.var[0][3] = a03 * b00 + a13 * b01 + a23 * b02 + a33 * b03;
  m.var[1][0] = a00 * b10 + a10 * b11 + a20 * b12 + a30 * b13;
  m.var[1][1] = a01 * b10 + a11 * b11 + a21 * b12 + a31 * b13;
  m.var[1][2] = a02 * b10 + a12 * b11 + a22 * b12 + a32 * b13;
  m.var[1][3] = a03 * b10 + a13 * b11 + a23 * b12 + a33 * b13;
  m.var[2][0] = a00 * b20 + a10 * b21 + a20 * b22 + a30 * b23;
  m.var[2][1] = a01 * b20 + a11 * b21 + a21 * b22 + a31 * b23;
  m.var[2][2] = a02 * b20 + a12 * b21 + a22 * b22 + a32 * b23;
  m.var[2][3] = a03 * b20 + a13 * b21 + a23 * b22 + a33 * b23;
  m.var[3][0] = a00 * b30 + a10 * b31 + a20 * b32 + a30 * b33;
  m.var[3][1] = a01 * b30 + a11 * b31 + a21 * b32 + a31 * b33;
  m.var[3][2] = a02 * b30 + a12 * b31 + a22 * b32 + a32 * b33;
  m.var[3][3] = a03 * b30 + a13 * b31 + a23 * b32 + a33 * b33;      
  return m;
}

// Multiply mat4 by a vec4
static inline vec4 Mul_mat4_vec4(mat4 A, vec4 b) {
  vec4 v;
  v.val[0] = A.var[0][0] * b.val[0] + A.var[1][0] * b.val[1] + A.var[2][0] * b.val[2] + A.var[3][0] * b.val[3];
  v.val[1] = A.var[0][1] * b.val[0] + A.var[1][1] * b.val[1] + A.var[2][1] * b.val[2] + A.var[3][1] * b.val[3];
  v.val[2] = A.var[0][2] * b.val[0] + A.var[1][2] * b.val[1] + A.var[2][2] * b.val[2] + A.var[3][2] * b.val[3];
  v.val[3] = A.var[0][3] * b.val[0] + A.var[1][3] * b.val[1] + A.var[2][3] * b.val[2] + A.var[3][3] * b.val[3];
  return v;
}

// mul/divide by rows and colums
// just something i added cause its easy to implement the Gauss-Jordan Elimination for inversing matrices <- picked the Cramers rule instead

static inline mat4 Mul_mat4_row(mat4 a, float b, int row) {
  mat4 m = a;
  m.var[0][row] *= b;
  m.var[1][row] *= b;
  m.var[2][row] *= b;
  m.var[3][row] *= b;
  return m;
}
static inline mat4 Div_mat4_row(mat4 a, float b, int row) {
  return Mul_mat4_row(a, 1/b, row);
}

static inline mat4 Mul_mat4_col(mat4 a, float b, int col) {
  mat4 m = a;
  m.var[0][col] *= b;
  m.var[1][col] *= b;
  m.var[2][col] *= b;
  m.var[3][col] *= b;
  return m;
}

static inline mat4 Div_mat4_col(mat4 a, float b, int col) {
  return Mul_mat4_col(a, 1/b, col);
}

// returns Transpose matrix. swapping the numbers diagonally
static inline mat4 Trans_mat4(mat4 a) {
  mat4 m;
  m.var[0][1] = a.var[1][0];
  m.var[0][2] = a.var[2][0];
  m.var[0][3] = a.var[3][0];
  m.var[1][0] = a.var[0][1];
  m.var[1][2] = a.var[2][1];
  m.var[1][3] = a.var[3][1];
  m.var[2][0] = a.var[0][2];
  m.var[2][1] = a.var[1][2];
  m.var[2][3] = a.var[3][2];
  m.var[3][0] = a.var[0][3];
  m.var[3][1] = a.var[1][3];
  m.var[3][2] = a.var[2][3];
  return m;
}

// returns a mat3 by removing a row and col from a 4x4 matrix
static inline mat3 Minor_mat4(mat4 a, int row, int col) {

  // what i call "skip if is in clear zone" method
  mat3 m;
  int k = 0;
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      if(i == col || j == row)  continue;
      m.arr[k++] = a.var[i][j];
    }
  }
  return m;
}

// returns Determinant of matrix. - magnitude of transformation
static inline float Det_mat4(mat4 a) {
  float plus = a.var[0][0] * Det_mat3(Minor_mat4(a, 0, 0))
              +a.var[2][0] * Det_mat3(Minor_mat4(a, 0, 2));
  float minus= a.var[1][0] * Det_mat3(Minor_mat4(a, 0, 1))
              +a.var[3][0] * Det_mat3(Minor_mat4(a, 0, 3));
  return plus - minus;
}

// returns Cofactor Matrix - signed minor of a matrix
static inline mat4 Cof_mat4(mat4 a) {
  // Mainly just to calculate the Adjugate which is called for the inverse

  mat4 m;
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      float sign = -1 + 2 * ((i+j) % 2 == 0); // 1 when i + j is even -1 when not
      m.var[i][j] = sign * Det_mat3(Minor_mat4(a, j, i));
    }
  }
  return m;
}

// returns Adjugate matrix - transpose of cofactor matrix  
static inline mat4 Adj_mat4(mat4 a) {
  mat4 C = Cof_mat4(a);
  return Trans_mat4(C);
}

// inverse matrix - can reverse transformations
static inline mat4 Inverse_mat4(mat4 m) {
  // using the Cramer's_rule
  float d = Det_mat4(m);
  if (d == 0)   return Zero_mat4();
  mat4 m_inv = Times_mat4(Adj_mat4(m), 1/Det_mat4(m));
  return m_inv;
}

// matrix division
static inline mat4 Div_mat4(mat4 a, mat4 b) {
  mat4 m = a;
  m = Mul_mat4(m, Inverse_mat4(m));
  return m;
}

#endif