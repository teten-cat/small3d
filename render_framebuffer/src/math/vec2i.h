#pragma once 

#include <stdio.h>
#include <math.h>

#include "vec2.h"

typedef union {
  struct {
    int x;
    int y;
  };

  int val[2];
} vec2int;

static inline vec2int round_vec2(vec2 vf) {
  vec2int vi;
  vi.x = (int)round(vf.x);
  vi.y = (int)round(vf.y);
  return vi;
}

static inline void print_vec2i(vec2int v) {
  printf("{%d, %d}\n", v.x,v.y);
}

static inline vec2int Fill_vec2i(int value) {
  vec2int v;
  v.x = v.y = value;
  return v;
}

static inline vec2int One_vec2i() {
  return Fill_vec2i(1);
}

static inline vec2int Zero_vec2i() {
  return Fill_vec2i(0);
}

static inline vec2int Add_vec2i(vec2int a, vec2int b) {
  vec2int v;
  v.x = a.x + b.x;
  v.y = a.y + b.y;
  return v;
}

static inline vec2int Neg_vec2i(vec2int v) {
  vec2int nv;
  v.x = -v.x;
  v.y = -v.y;
  return nv;
}

static inline vec2int NegX_vec2i(vec2int v) {
  vec2int nv = v;
  nv.x = -v.x;
  return nv;
}

static inline vec2int NegY_vec2i(vec2int v) {
  vec2int nv = v;
  nv.y = -v.y;
  return nv;
}


static inline vec2int Sub_vec2i(vec2int a, vec2int b) {
  vec2int v;
  v.x = a.x - b.x;
  v.y = a.y - b.y;
  return v;
}

static inline vec2int Mul_vec2i(vec2int a, vec2int b) {
  vec2int v;
  v.x = a.x * b.x;
  v.y = a.y * b.y;
  return v;
}

static inline vec2int Times_vec2i(vec2int a, int b) {
  vec2int v;
  v.x = a.x * b;
  v.y = a.y * b;
  return v;
}


static inline int Dot_vec2i(vec2int a, vec2int b) {
  return a.x*b.x + a.y*b.y;
}

static inline int Cross_vec2i(vec2int a, vec2int b) {
  return a.x*b.y - a.y*b.x;
}
