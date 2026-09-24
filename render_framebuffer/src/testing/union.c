#include <stdio.h>

#include "../math/vec4.h"

union mat4 {
  vec4 col[4];
  float value[16];
};

int main() {

  union mat4 data = {
    .value = { 2, 2, 2, 2,
              2, 2, 2, 2,
              2, 2, 2, 2,
              2, 2, 2, 2 }
  };

  data.col[3].w = 0;

  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      printf("%g ", data.value[4*i + j]);
    }
    printf("\n");
  }


  printf("Size of data: %ld\n", sizeof(float));
  printf("Size of data: %ld\n", sizeof(union mat4));


  return 0;
}