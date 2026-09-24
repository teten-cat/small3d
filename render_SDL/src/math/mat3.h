#ifndef MAT3_H
#define MAT3_H

#include "vec3.h"
#include "mat2.h"

/*
  3x3 matrix definition

vec4 col array
[ col[0].x  col[1].x  col[2].x ]
| col[0].y  col[1].y  col[2].y |
[ col[0].z  col[1].z  col[2].z ]

float array array

[ var[0][0] var[1][0] var[2][0] ]
| var[0][1] var[1][1] var[2][1] |
[ var[0][2] var[1][2] var[2][2] ]


float arr array
[ 0  3  6 ]
| 1  4  7 |
[ 2  5  8 ]

Thus
col[1].z = col[1].val[2] = var[1][2] = arr[5]

*/


typedef union {
  vec3 col[3];
  float var[3][3];
  float arr[9];
} mat3;

static inline void Print_mat3(mat3 m) {
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      printf("%9g ", m.var[j][i]);
    }
    printf("\n");
  }
}

static inline mat3 Fill_mat3(float val) {
  mat3 m;
  for(int i = 0; i < 9; i++) {
    m.arr[i] = val;
  }
  return m;
}

static inline mat3 Zero_mat3() {
  return Fill_mat3(0);
}

// returns the identity matrix. - m[i][j] = 0(i != j), 1(i == j)
static inline mat3 Identity_mat3() {
  mat3 m = Zero_mat3();
  m.var[0][0] = 1;
  m.var[1][1] = 1;
  m.var[2][2] = 1;
  return m;
}

// matrix addition
static inline mat3 Add_mat3(mat3 a, mat3 b) {
  mat3 m;
  m.arr[0] = a.arr[0] + b.arr[0];
  m.arr[1] = a.arr[1] + b.arr[1];
  m.arr[2] = a.arr[2] + b.arr[2];
  m.arr[3] = a.arr[3] + b.arr[3];
  m.arr[4] = a.arr[4] + b.arr[4];
  m.arr[5] = a.arr[5] + b.arr[5];
  m.arr[6] = a.arr[6] + b.arr[6];
  m.arr[7] = a.arr[7] + b.arr[7];
  m.arr[8] = a.arr[8] + b.arr[8];
  return m;
}

// Adds float to all values of matrix
static inline mat3 Plus_mat3(mat3 a, float b) {
  mat3 m = a;
  m.arr[0] += b;
  m.arr[1] += b;
  m.arr[2] += b;
  m.arr[3] += b;
  m.arr[4] += b;
  m.arr[5] += b;
  m.arr[6] += b;
  m.arr[7] += b;
  m.arr[8] += b;
  return m;
}

// Matrix subtraction
static inline mat3 Sub_mat3(mat3 a, mat3 b) {
  mat3 m;
  m.arr[0] = a.arr[0] - b.arr[0];
  m.arr[1] = a.arr[1] - b.arr[1];
  m.arr[2] = a.arr[2] - b.arr[2];
  m.arr[3] = a.arr[3] - b.arr[3];
  m.arr[4] = a.arr[4] - b.arr[4];
  m.arr[5] = a.arr[5] - b.arr[5];
  m.arr[6] = a.arr[6] - b.arr[6];
  m.arr[7] = a.arr[7] - b.arr[7];
  m.arr[8] = a.arr[8] - b.arr[8];
  return m;
}

// multiply all values in matrix a by float b
static inline mat3 Times_mat3(mat3 a, float b) {
  mat3 m = a;
  m.arr[0] *= b;
  m.arr[1] *= b;
  m.arr[2] *= b;
  m.arr[3] *= b;
  m.arr[4] *= b;
  m.arr[5] *= b;
  m.arr[6] *= b;
  m.arr[7] *= b;
  m.arr[8] *= b;
  return m;
}

// Matrix multiplication
static inline mat3 Mul_mat3(mat3 a, mat3 b) {
  mat3 m;
  float a00 = a.var[0][0], a01 = a.var[0][1], a02 = a.var[0][2],
        a10 = a.var[1][0], a11 = a.var[1][1], a12 = a.var[1][2],
        a20 = a.var[2][0], a21 = a.var[2][1], a22 = a.var[2][2],
        b00 = b.var[0][0], b01 = b.var[0][1], b02 = b.var[0][2],
        b10 = b.var[1][0], b11 = b.var[1][1], b12 = b.var[1][2],
        b20 = b.var[2][0], b21 = b.var[2][1], b22 = b.var[2][2];

  m.var[0][0] = a00 * b00 + a10 * b01 + a20 * b02;
  m.var[0][1] = a01 * b00 + a11 * b01 + a21 * b02;
  m.var[0][2] = a02 * b00 + a12 * b01 + a22 * b02;
  m.var[1][0] = a00 * b10 + a10 * b11 + a20 * b12;
  m.var[1][1] = a01 * b10 + a11 * b11 + a21 * b12;
  m.var[1][2] = a02 * b10 + a12 * b11 + a22 * b12;
  m.var[2][0] = a00 * b20 + a10 * b21 + a20 * b22;
  m.var[2][1] = a01 * b20 + a11 * b21 + a21 * b22;
  m.var[2][2] = a02 * b20 + a12 * b21 + a22 * b22;

  return m;
}


// mul/divide by rows and colums
// just something i added cause its easy to implement the Gauss-Jordan Elimination for inversing matrices

static inline mat3 Mul_mat3_row(mat3 a, float b, int row) {
  mat3 m = a;
  m.var[0][row] *= b;
  m.var[1][row] *= b;
  m.var[2][row] *= b;
  return m;
}
static inline mat3 Div_mat3_row(mat3 a, float b, int row) {
  return Mul_mat3_row(a, 1/b, row);
}

static inline mat3 Mul_mat3_col(mat3 a, float b, int col) {
  mat3 m = a;
  m.var[0][col] *= b;
  m.var[1][col] *= b;
  m.var[2][col] *= b;
  return m;
}

static inline mat3 Div_mat3_col(mat3 a, float b, int col) {
  return Mul_mat3_col(a, 1/b, col);
}

// returns Transpose matrix. swapping the numbers diagonally
static inline mat3 Trans_mat3(mat3 a) {
  mat3 m = a;
  m.var[0][1] = a.var[1][0];
  m.var[0][2] = a.var[2][0];
  m.var[1][0] = a.var[0][1];
  m.var[1][2] = a.var[2][1];
  m.var[2][0] = a.var[0][2];
  m.var[2][1] = a.var[1][2];
  return m;
}

// returns a mat2 by removing a row and col from a 3x3 matrix
static inline mat2 Minor_mat3(mat3 a, int row, int col) {
  // what i call "skip if is in clear zone" method
  mat2 m;
  int k = 0;
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      if(i == col || j == row)  continue;
      m.arr[k++] = a.var[i][j];
    }
  }
  return m;
}

// returns Determinant of matrix. - magnitude of transformation
static inline float Det_mat3(mat3 a) {
  // rule of sarrus
  float plus = a.arr[0]*a.arr[4]*a.arr[8] +
               a.arr[2]*a.arr[3]*a.arr[7] +
               a.arr[1]*a.arr[5]*a.arr[6];

  float minus= a.arr[2]*a.arr[4]*a.arr[6] +
               a.arr[1]*a.arr[3]*a.arr[8] +
               a.arr[0]*a.arr[5]*a.arr[7];
  return plus - minus;
}


// returns Cofactor matrix - signed minor of a matrix
static inline mat3 Cof_mat3(mat3 a) {
  // Mainly just to calculate the Adjugate which is called for the inverse

  mat3 m;
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      float sign = -1 + 2 * ((i+j) % 2 == 0); // 1 when i + j is even -1 when not
      m.var[i][j] = sign * Det_mat2(Minor_mat3(a, j, i));
    }
  }
  return m;
}

// returns Adjugate matrix - transpose of cofactor matrix  
static inline mat3 Adj_mat3(mat3 a) {
  mat3 C = Cof_mat3(a);
  return Trans_mat3(C);
}

// inverse matrix - can reverse transformations
static inline mat3 Inverse_mat3(mat3 m) {
  // using the Cramer's_rule
  float d = Det_mat3(m);
  if (d == 0)   return Zero_mat3();
  mat3 m_inv = Times_mat3(Adj_mat3(m), 1/Det_mat3(m));
  return m_inv;
}

// matrix division
static inline mat3 Div_mat3(mat3 a, mat3 b) {
  mat3 m = a;
  m = Mul_mat3(m, Inverse_mat3(b));
  return m;
}

#endif