// Program created based on https://www.youtube.com/watch?v=yFboyOwk2oM
// To run it:
// gcc DynamicMemory.c -o DynamicMemory.o && ./DynamicMemory.o
#include <stdio.h>
#include <stdlib.h>

int reallocExample() {
  int *ptr, n = 2, i;

  ptr = malloc(n * sizeof(int));

  ptr[0] = 1;
  ptr[1] = 2;

  n = n + 2; // Increase array size by 2

  ptr = realloc(ptr, n * sizeof(int));

  if (ptr == NULL) {
    printf("Error! Unable to allocate memory.");
    exit(0);
  }

  ptr[2] = 3;
  ptr[3] = 4;

  printf("\nRealloc:");

  // Print all elements
  for (i = 0; i < n; i++) {
    printf(" %d", ptr[i]);
  }

  printf("\n");

  free(ptr);
  return 0;
}

int main() {
  // The size of a pointer variable depends on the system architecture. It can
  // be 4 bytes or 8 bytes, depending on whether it is a 32-bit or 64-bit
  // system.
  //
  //     On 32-bit systems, pointers are usually 4 bytes in size
  //
  //     On 64-bit systems, pointers are usually 8 bytes in size
  //
  // The size of a pointer variable does not depend on the type of data it is
  // pointing to. A pointer to an int and a pointer to a char will have the same
  // size, depending on the system architecture. Reference:
  //
  // https://visheshraghuvanshi.tech/c-pointers#heading-size-of-pointer-variable
  printf("It takes %zu bytes per pointer\n", sizeof(void *));
  printf("\n");

  // Allocate memory on the heap instead of the stack
  int *p = malloc(sizeof(int));
  if (p == NULL) {
    printf("Error! Unable to allocate memory\n");
    exit(0);
  }

  *p = 10;
  printf("Pointer %p has value %d\n", p, *p);

  printf("\n");

  // Allocate memory this time for an array
  int *ps = malloc(sizeof(int) * 3);

  ps[1] = 10;
  ps[2] = 20;

  // Using malloc does not set missing values to 0.
  printf("Pointers %p has values %d, %d, %d\n", ps, ps[0], ps[1], ps[2]);

  printf("\n");

  // Allocate memory this time for an array
  int *ps2 = calloc(3, sizeof(int));

  ps2[1] = 10;
  ps2[2] = 20;

  // Using malloc does not set missing values to 0.
  printf("Pointers %p has values %d, %d, %d\n", ps2, ps2[0], ps2[1], ps2[2]);

  // Free all pointers
  free(p);
  free(ps);
  free(ps2);

  reallocExample();

  return 0;
};
