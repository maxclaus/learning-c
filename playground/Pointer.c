// Program created based on https://www.youtube.com/watch?v=LscgaBzlGdE
// To run it:
// gcc Pointer.c -o Pointer.o && ./Pointer.o
#include <stdio.h>

// Possible pointer syntaxes:
//   - "int *ptr": Not recommended.
//   - "int* ptr"
int main() {
  int age = 25;

  int *ptr = &age;

  printf("Address %p value %d\n", ptr, *ptr);

  int numbers[5] = {1, 2, 3, 4, 5};

  for (int i = 0; i < 5; i++) {
    printf("%d = %p\n", numbers[i], &numbers[i]);
  }

  printf("\n");

  // "&numbers[index]" is the same as "numbers + index".
  for (int i = 0; i < 5; i++) {
    printf("%d = %p\n", *(numbers + i), numbers + i);
  }

  printf("\n");

  *numbers = 10;
  *(numbers + 4) = 50;

  for (int i = 0; i < 5; i++) {
    printf("%d = %p\n", *(numbers + i), numbers + i);
  }

  return 0;
};
