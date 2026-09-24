#ifndef MAT2_H
#define MAT2_H

#include "vec2.h"

/*
  2x2 matrix definition

vec4 col array
[ col[0].x  col[1].x ]
[ col[0].y  col[1].y ]

float array array

[ var[0][0] var[1][0] ]
[ var[0][1] var[1][1] ]


float arr array
[ 0  2]
[ 1  3]

Thus
col[1].y = col[1].val[1] = var[1][1] = arr[3]

*/


typedef union {
  vec2 col[2];
  float var[2][2];
  float arr[4];
} mat2;

static inline void Print_mat2(mat2 m) {
  for(int i = 0; i < 2; i++) {
    for(int j = 0; j < 2; j++) {
      printf("%9g ", m.var[j][i]);
    }
    printf("\n");
  }
}

// sets all values of matrix into val
static inline mat2 Fill_mat2(float val) {
  mat2 m;
  for(int i = 0; i < 4; i++) {
    m.arr[i] = val;
  }
  return m;
}

// matrix filled with 0's
static inline mat2 Zero_mat2() {
  return Fill_mat2(0);
}

// returns the identity matrix. - m[i][j] = 0(i != j), 1(i == j)
static inline mat2 Identity_mat2() {
  mat2 m = Zero_mat2();
  m.var[0][0] = 1;
  m.var[1][1] = 1;
  return m;
}

// matrix addition
static inline mat2 Add_mat2(mat2 a, mat2 b) {
  mat2 m;
  m.arr[0] = a.arr[0] + b.arr[0];
  m.arr[1] = a.arr[1] + b.arr[1];
  m.arr[2] = a.arr[2] + b.arr[2];
  m.arr[3] = a.arr[3] + b.arr[3];
  return m;
}

// Adds float to all values of matrix
static inline mat2 Plus_mat2(mat2 a, float b) {
  mat2 m = a;
  m.arr[0] += b;
  m.arr[1] += b;
  m.arr[2] += b;
  m.arr[3] += b;
  return m;
}

// Matrix subtraction
static inline mat2 Sub_mat2(mat2 a, mat2 b) {
  mat2 m;
  m.arr[0] = a.arr[0] - b.arr[0];
  m.arr[1] = a.arr[1] - b.arr[1];
  m.arr[2] = a.arr[2] - b.arr[2];
  m.arr[3] = a.arr[3] - b.arr[3];
  return m;
}

// Multiply all values in matrix a by float b
static inline mat2 Times_mat2(mat2 a, float b) {
  mat2 m = a;
  m.arr[0] *= b;
  m.arr[1] *= b;
  m.arr[2] *= b;
  m.arr[3] *= b;

  return m;
}

// Matrix multiplication
static inline mat2 Mul_mat2(mat2 a, mat2 b) {
  mat2 m;
  float a00 = a.var[0][0], a01 = a.var[0][1],
        a10 = a.var[1][0], a11 = a.var[1][1],
        b00 = b.var[0][0], b01 = b.var[0][1],
        b10 = b.var[1][0], b11 = b.var[1][1];

  m.var[0][0] = a00 * b00 + a10 * b01;
  m.var[0][1] = a01 * b00 + a11 * b01;
  m.var[1][0] = a00 * b10 + a10 * b11;
  m.var[1][1] = a01 * b10 + a11 * b11;
  return m;
}


// mul/divide by rows and colums
// just something i added cause its easy to implement the Gauss-Jordan Elimination later

// Mul_mat2_row : multiple every element on row'th(counting from 0) row of matrix A
// the rest is obvious

// multiplies mat2 a by float b at row row
static inline mat2 Mul_mat2_row(mat2 a, float b, int row) {
  mat2 m = a;
  m.var[0][row] *= b;
  m.var[1][row] *= b;
  return m;
}

// devides mat2 a by float b at row row
static inline mat2 Div_mat2_row(mat2 a, float b, int row) {
  return Mul_mat2_row(a, 1/b, row);
}

// multiplies mat2 a by float b at column col
static inline mat2 Mul_mat2_col(mat2 a, float b, int col) {
  mat2 m = a;
  m.var[0][col] *= b;
  m.var[1][col] *= b;
  return m;
}

// devides mat2 a by float b at column col
static inline mat2 Div_mat2_col(mat2 a, float b, int col) {
  return Mul_mat2_col(a, 1/b, col);
}

// returns Transpose matrix. swapping the numbers diagonally
static inline mat2 Trans_mat2(mat2 a) {
  mat2 m = a;
  m.var[0][1] = a.var[1][0];
  m.var[1][0] = a.var[0][1];
  return m;
}

// returns Determinant of matrix. - magnitude of transformation
static inline float Det_mat2(mat2 a) {
  float d = 0;
  d = a.arr[0] * a.arr[3] - a.arr[1] * a.arr[2];
  return d;
}

// returns adjugate matrix
static inline mat2 Adj_mat2(mat2 a) {
  // simple swap and touch for a 2x2 but for the rest its
  // a transpose of a Cofactor(each elemehnt is determinant of when remove its row and collum * (-1)^i+j)
  mat2 m;
  m.arr[0] =  a.arr[3];
  m.arr[1] = -a.arr[1];
  m.arr[2] = -a.arr[2];
  m.arr[3] = -a.arr[0];
  return m;
}

// inverse matrix - can reverse transformations
static inline mat2 Inverse_mat2(mat2 m) {
  // not yet probably use the Cramers rule
  float d = Det_mat2(m);
  if (d == 0)   return Zero_mat2();
  mat2 m_inv = Times_mat2(Adj_mat2(m), 1/Det_mat2(m));
  return m_inv;
}

// matrix division
static inline mat2 Div_mat2(mat2 a, mat2 b) {
  mat2 m = a;
  m = Mul_mat2(m, Inverse_mat2(b));
  return m;
}

#endif