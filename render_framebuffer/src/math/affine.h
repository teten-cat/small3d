#ifndef AFFINE_H
#define AFFINE_H

#include "mat4.h"
#include "vec3.h"
#include "vec4.h"
#include <math.h>


/*
  3D space is in Right-handed Cartesian Coordinates

          y
          ^
          |
          |
          |
          O ---------->X
         /
       /
      /
     Z
*/

// returns radian value from degrees. 
// 2PI base to 360 base
static inline float rad_from_deg(float deg) {
  return deg * M_PI / 180;
}

// returns degrees value from radian. 
// 360 base to 2PI base
static inline float deg_from_rad(float rad) {
  return 180 * rad / M_PI;
}

static inline vec3 vec3_rad_from_deg(vec3 deg) {
  vec3 v;
  v.x = rad_from_deg(deg.x);
  v.y = rad_from_deg(deg.y);
  v.z = rad_from_deg(deg.z);
  return v;
}

static inline vec3 vec3_deg_from_rad(vec3 rad) {
  vec3 v;
  v.x = deg_from_rad(rad.x);
  v.y = deg_from_rad(rad.y);
  v.z = deg_from_rad(rad.z);
  return v;
}

// returns translation matrix
static inline mat4 Translate(vec3 v) {

  /*
    if v = {a, b, c}
    M_t = 
    [ 1  0  0  a ]
    | 0  1  0  b |
    | 0  0  1  c |
    [ 0  0  0  1 ]

    Multiplying this with a position vector 4 (.w = 1) {x, y, z} will result in {x+a, y+b, z+c} 

    Usage:
      vec4 original_coordinates = {.x = 2, .y = 5, .z = 8, .w = 1};
      vec3 move_by = {.x = 2, .y = 3, .z = 4};
      mat4 translate_matrix = Translate(move_by);

      vec4 new_coordinates = Mul_mat4_vec4(translate_matrix, original_coordinates);
      Print_vec4(new_coordinates);
    Output
      original Coordiantes
      {2, 5, 8, 1}
      Result 
      {4, 8, 12, 1}

  */
  mat4 M = Identity_mat4();
  M.col[3].x = v.x;
  M.col[3].y = v.y;
  M.col[3].z = v.z;
  return M;
}



// returns already translated values
static inline vec4 Translate_to(vec3 original, vec3 moveby) {
  //not recomended use. Recomend to Scale Rotate Translate the matrix first cause can apply that one matrix to every vertex

  vec4 moveby_v4 = {.x = moveby.x, .y = moveby.y, .z = moveby.z, .w = 1};
  return Mul_mat4_vec4(Translate(original), moveby_v4);
}

// returns scale tranfomation matrix
static inline mat4 Scale(vec3 v) {

  /*
    if v = {a, b, c}
    M_t = 
    [ a  0  0  0 ]
    | 0  b  0  0 |
    | 0  0  c  0 |
    [ 0  0  0  1 ]

    Multiplying this with a position vec4 (.w = 1) {x, y, z} will result in {ax, by, cz} 

    Usage:
      vec4 original_coordinates = {.x = 2, .y = 5, .z = 8, .w = 1};

      vec3 scale_by = {.x = 2, .y = 3, .z = 4};
      mat4 scale_matrix = Scale(scale_by);

      vec4 new_coordinates = Mul_mat4_vec4(scale_matrix, original_coordinates);

    Output
      original coordinates
      {2, 5, 8}
      scale by
      {2, 3, 4}
      result
      {4, 15, 32, 1}

  */

  mat4 m = Identity_mat4();
  m.var[0][0] = v.x;
  m.var[1][1] = v.y;
  m.var[2][2] = v.z;
  return m;
}

// returns X axis rotation matrix that rotates the position it's multiplied by rad.
static inline mat4 Rotate_X(float rad) {
  // rotation is origin based. rotates based on the X axis
  // so the X doesnt cahnge
  /*
    FORMULA: let R be the angle in radians
    [    1     0     0     0 ]
    |    0   cosR -sinR    0 |
    |    0   sinR  cosR    0 |
    [    0     0     0     1 ]


    Z
    |
    |   <=  rotates
    |     \ this way
    |     |
    X ___________Y
  */
  float cr = cosf(rad);
  float sr = sinf(rad);
  mat4 m = Identity_mat4();
  m.var[1][1] = cr;
  m.var[1][2] = sr;
  m.var[2][1] = -sr;
  m.var[2][2] = cr;
  return m;
}

// returns Y axis rotation matrix that rotates the position it's multiplied by rad.
static inline mat4 Rotate_Y(float rad) {
  // rotation is origin based. rotates based on the Y axis
  // so the Y doesnt cahnge
  /*
    FORMULA: let R be the angle in radians
    [  cosR    0   sinR    0 ]
    |    0     1     0     0 |
    | -sinR    0   cosR    0 |
    [    0     0     0     1 ]


    X
    |
    |   <=  rotates
    |     \ this way
    |     |
    Y ___________Z
  */
  float cr = cosf(rad);
  float sr = sinf(rad);
  mat4 m = Identity_mat4();
  m.var[0][0] =  cr;
  m.var[0][2] = -sr;
  m.var[2][0] =  sr;
  m.var[2][2] =  cr;
  return m;
}

// returns Z axis rotation matrix that rotates the position it's multiplied by rad.
static inline mat4 Rotate_Z(float rad) {
  // rotation is origin based. rotates based on the Z axis
  // so the Z doesnt cahnge
  /*
    FORMULA: let R be the angle in radians
    [  cosR  -sinR   0     0 ]
    |  sinR  cosR    0     0 |
    |    0     0     1     0 |
    [    0     0     0     1 ]


    Y
    |
    |   <=  rotates
    |     \ this way
    |     |
    Z ___________X
  */
  float cr = cosf(rad);
  float sr = sinf(rad);
  mat4 m = Identity_mat4();
  m.var[0][0] =  cr;
  m.var[0][1] =  sr;
  m.var[1][0] =  -sr;
  m.var[1][1] =  cr;
  return m;
}

static inline mat4 Rotate(vec3 rads) {
  mat4 m;
  m = Rotate_X(rads.x);
  m = Mul_mat4(m, Rotate_Y(rads.y));
  m = Mul_mat4(m, Rotate_Z(rads.z));
  return m;
}

static inline mat4 Perspective(float FoV_rad, float aspect_ratio, float near, float far) {
  // assumes symetric( l=-r, b=-t)

  // note: FoV is the vertival angle. not the horizontal
  float t = near * tanf(FoV_rad/2);
  float b = -t;
  float r = t * aspect_ratio;
  float l = -r;

  mat4 m = Zero_mat4();
  m.arr[0] = near / r;
  m.arr[5] = near / t;
  m.arr[10] = (-(far + near)) / (far - near);
  m.arr[11] = -1;
  m.arr[14] = (-2 * far * near) / (far - near);

  return m;
}

#endif