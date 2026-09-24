// for comparing numbesr like max and min
#ifndef UTIL_CMP_H
#define UTIL_CMP_H

static inline int max2i(int a, int b) {
  return (a > b) ? a : b;
}

static inline int min2i(int a, int b) {
  return (a < b) ? a : b;
}

static inline int max3i(int a, int b, int c) {
  return max2i(max2i(a, b), c);
}

static inline int min3i(int a, int b, int c) {
  return min2i(min2i(a, b), c);
}

// 1 if same sign
static inline int sign2i(int a, int b) {

  if((a==0 || b==0)) return 1;

  return ((a<0) == (b<0));
}

static inline int sign3i(int a, int b, int c) {
  return (sign2i(a,b) && sign2i(a,c));
}

static inline int dist2i(int a, int b) {
  return (a-b >= 0) ? a-b : -(a-b);
}

#endif