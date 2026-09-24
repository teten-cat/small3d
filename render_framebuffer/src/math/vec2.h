#pragma once

#include <stdio.h>
#include <math.h>


typedef union {
  struct {
    float x;
    float y;
  };

  float val[2];
} vec2;


static inline void Print_vec2(vec2 v) {
  printf("{%g, %g}\n", v.x,v.y);
}

static inline vec2 Fill_vec2(float value) {
  vec2 v;
  v.x = v.y = value;
  return v;
}

static inline vec2 One_vec2() {
  return Fill_vec2(1);
}

static inline vec2 Zero_vec2() {
  return Fill_vec2(0);
}

static inline vec2 Add_vec2(vec2 a, vec2 b) {
  vec2 v;
  v.x = a.x + b.x;
  v.y = a.y + b.y;
  return v;
}

static inline vec2 Neg_vec2(vec2 v) {
  vec2 nv;
  v.x = -v.x;
  v.y = -v.y;
  return nv;
}

static inline vec2 NegX_vec2(vec2 v) {
  vec2 nv = v;
  nv.x = -v.x;
  return nv;
}

static inline vec2 NegY_vec2(vec2 v) {
  vec2 nv = v;
  nv.y = -v.y;
  return nv;
}

static inline vec2 Sub_vec2(vec2 a, vec2 b) {
  vec2 v;
  v.x = a.x - b.x;
  v.y = a.y - b.y;
  return v;
}

static inline vec2 Mul_vec2(vec2 a, vec2 b) {
  vec2 v;
  v.x = a.x * b.x;
  v.y = a.y * b.y;
  return v;
}

static inline vec2 Times_vec2(vec2 a, float b) {
  vec2 v;
  v.x = a.x * b;
  v.y = a.y * b;
  return v;
}


static inline vec2 Div_vec2(vec2 a, vec2 b) {
  vec2 v;
  v.x = a.x / b.x;
  v.y = a.y / b.y;
  return v;
}

// returns dot product of 2 2D vectors
static inline float Dot_vec2(vec2 a, vec2 b) {
  return a.x * b.x + a.y * b.y;
}

// returns cross product of 2 2D vectors
static inline float Cross_vec2(vec2 a, vec2 b) {
  return a.x * b.y - a.y * b.x;
}

static inline float Abs_vec2(vec2 a) {
  return sqrt(a.x * a.x + a.y * a.y);
}


static inline vec2 Normal_vec2(vec2 a) {
  return Div_vec2(a, Fill_vec2(Abs_vec2(a)));
}