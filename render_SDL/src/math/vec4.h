#pragma once

#include <stdio.h>
#include <math.h>

#include "vec3.h"

typedef union {
  struct {
    float x;
    float y;
    float z;
    float w;
  };

  float val[4];
} vec4;


static inline void Print_vec4(vec4 v) {
  printf("{%g, %g, %g, %g}\n", v.x,v.y, v.z, v.w);
}

static inline vec4 Fill_vec4(float value) {
  vec4 v;
  v.x = v.y = v.z = v.w = value;
  return v;
}

static inline vec4 vec4_from_vec3(vec3 a) {
  vec4 v = {.x = a.x, .y = a.y, .z = a.z, .w = 1};
  return v;
}

// returns vec3 with x/w. y/w, z/w
static inline vec3 vec3_from_vec4(vec4 a) {
  vec3 v = {.x = a.x/a.w, .y = a.y/a.w, .z = a.z/a.w};
  return v;
}

static inline vec4 One_vec4() {
  return Fill_vec4(1);
}

static inline vec4 Zero_vec4() {
  return Fill_vec4(0);
}

static inline vec4 Add_vec4(vec4 a, vec4 b) {
  vec4 v;
  v.x = a.x + b.x;
  v.y = a.y + b.y;
  v.z = a.z + b.z;
  v.w = a.w + b.w;
  return v;
}

static inline vec4 Neg_vec4(vec4 v) {
  vec4 nv;
  v.x = -v.x;
  v.y = -v.y;
  v.z = -v.z;
  return nv;
}

static inline vec4 NegX_vec4(vec4 v) {
  vec4 nv = v;
  nv.x = -v.x;
  return nv;
}

static inline vec4 NegY_vec4(vec4 v) {
  vec4 nv = v;
  nv.y = -v.y;
  return nv;
}

static inline vec4 NegZ_vec4(vec4 v) {
  vec4 nv = v;
  nv.z = -v.z;
  return nv;
}

static inline vec4 NegW_vec4(vec4 v) {
  vec4 nv = v;
  nv.w = -v.w;
  return nv;
}

static inline vec4 Sub_vec4(vec4 a, vec4 b) {
  vec4 v;
  v.x = a.x - b.x;
  v.y = a.y - b.y;
  v.z = a.z - b.z;
  v.w = a.w - b.w;
  return v;
}

static inline vec4 Mul_vec4(vec4 a, vec4 b) {
  vec4 v;
  v.x = a.x * b.x;
  v.y = a.y * b.y;
  v.z = a.z * b.z;
  v.w = a.w * b.w;
  return v;
}

static inline vec4 Times_vec4(vec4 a, float b) {
  vec4 v;
  v.x = a.x * b;
  v.y = a.y * b;
  v.z = a.z * b;
  v.w = a.w * b;
  return v;
}


static inline vec4 Div_vec4(vec4 a, vec4 b) {
  vec4 v;
  v.x = a.x / b.x;
  v.y = a.y / b.y;
  v.z = a.z / b.z;
  v.w = a.w / b.w;
  return v;
}

// returns dot product of 2 2D vectors
static inline float Dot_vec4(vec4 a, vec4 b) {
  return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

// cross product of 4d vectors dont even exist

static inline float Abs_vec4(vec4 a) {
  return sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}


static inline vec4 Normal_vec4(vec4 a) {
  return Div_vec4(a, Fill_vec4(Abs_vec4(a)));
}