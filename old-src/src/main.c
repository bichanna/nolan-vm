#include <stdio.h>
#include <stdlib.h>

int main(void) {
  _Static_assert(sizeof(double) == 8,
                 "Platform must support 64-bit floating point number (double).");

  printf("Hello, world\n");
  return EXIT_SUCCESS;
}
