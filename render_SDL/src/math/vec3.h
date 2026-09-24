#pragma once

#include <stdio.h>
#include <math.h>

typedef union {
  struct {
    float x;
    float y;
    float z;
  };

  float val[3];
} vec3;



static inline void Print_vec3(vec3 v) {
  printf("{%g, %g, %g}\n", v.x,v.y, v.z);
}

static inline int Equal_vec3(vec3 a, vec3 b) {
  return (a.x == b.x && a.y == b.y && a.z == b.z);
}

static inline vec3 Fill_vec3(float value) {
  vec3 v;
  v.x = v.y = v.z = value;
  return v;
}

static inline vec3 One_vec3() {
  return Fill_vec3(1);
}

static inline vec3 Zero_vec3() {
  return Fill_vec3(0);
}

static inline vec3 Add_vec3(vec3 a, vec3 b) {
  vec3 v;
  v.x = a.x + b.x;
  v.y = a.y + b.y;
  v.z = a.z + b.z;
  return v;
}

static inline vec3 Neg_vec3(vec3 v) {
  vec3 nv;
  v.x = -v.x;
  v.y = -v.y;
  v.z = -v.z;
  return nv;
}

static inline vec3 NegX_vec3(vec3 v) {
  vec3 nv = v;
  nv.x = -v.x;
  return nv;
}

static inline vec3 NegY_vec3(vec3 v) {
  vec3 nv = v;
  nv.y = -v.y;
  return nv;
}

static inline vec3 NegZ_vec3(vec3 v) {
  vec3 nv = v;
  nv.z = -v.z;
  return nv;
}

static inline vec3 Sub_vec3(vec3 a, vec3 b) {
  vec3 v;
  v.x = a.x - b.x;
  v.y = a.y - b.y;
  v.z = a.z - b.z;
  return v;
}

static inline vec3 Mul_vec3(vec3 a, vec3 b) {
  vec3 v;
  v.x = a.x * b.x;
  v.y = a.y * b.y;
  v.z = a.z * b.z;
  return v;
}

static inline vec3 Times_vec3(vec3 a, float b) {
  vec3 v;
  v.x = a.x * b;
  v.y = a.y * b;
  v.z = a.z * b;
  return v;
}

static inline vec3 Div_vec3(vec3 a, vec3 b) {
  vec3 v;
  v.x = a.x / b.x;
  v.y = a.y / b.y;
  v.z = a.z / b.z;
  return v;
}

// returns dot product of 2 2D vectors
static inline float Dot_vec3(vec3 a, vec3 b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

// returns cross product of 2 2D vectors
static inline vec3 Cross_vec3(vec3 a, vec3 b) {
  vec3 v;
  v.x = a.y * b.z - a.z * b.y;
  v.y = a.z * b.x - a.x * b.z;
  v.z = a.x * b.y - a.y * b.x;
  return v;
}

static inline float Abs_vec3(vec3 a) {
  return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}


static inline vec3 Normal_vec3(vec3 a) {
  return Div_vec3(a, Fill_vec3(Abs_vec3(a)));
}