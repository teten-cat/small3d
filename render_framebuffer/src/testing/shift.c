#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

struct Color white = {255,255,255};

static inline uint32_t Load_Pixel() {
  return ((white.r << 16) |
         (white.g << 8) |
         (white.b << 0));
}


int main(void) {
  // 00000000000000000000000011111111
  // for (int i = 31; i >= 0; --i) {
  //       printf("%"PRIu32, loaded_color >> i & 1);
  //   }
  //   printf("\n");
  return 0;
}
