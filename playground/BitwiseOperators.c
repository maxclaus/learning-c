// Program created based on the "C Programming Full Course for free" tutorial
// To run it:
// gcc BitwiseOperators.c -o BitwiseOperators.o && ./BitwiseOperators.o
#include <stdio.h>

// function to convert decimal to binary
void printIntAsBinary(int n) {
  // array to store binary number
  int binaryNum[1000];

  // counter for binary array
  int i = 0;
  while (n > 0) {

    // storing remainder in binary array
    binaryNum[i] = n % 2;
    n = n / 2;
    i++;
  }

  printf("(");

  // printing binary array in reverse order
  for (int j = i - 1; j >= 0; j--)
    printf("%d", binaryNum[j]);

  printf(")\n");
}

int main() {
  // BITWISE OPERATORS = special operators used in bit level programming:
  //
  //   & = AND
  //   | = OR
  //   ^ = XOR
  //   << = left shift
  //   >> = right shift

  int x = 6;  // 6  = 00000110
  int y = 12; // 12 = 00001100
  int z = 0;  // 0  = 00000000

  // Only copy '1' when both have '1'
  z = x & y;
  printf("AND = %d ", z);
  printIntAsBinary(z);

  // Copy '1' when any have '1'
  z = x | y;
  printf("OR = %d", z);
  printIntAsBinary(z);

  // Only copy '1' when just one have '1'
  z = x ^ y;
  printf("XOR = %d", z);
  printIntAsBinary(z);

  // Move 2 bit to left
  z = x << 2;
  printf("left shift = %d", z);
  printIntAsBinary(z);

  // Move 2 bit to right
  z = x >> 2;
  printf("right shift = %d", z);
  printIntAsBinary(z);

  return 0;
};
