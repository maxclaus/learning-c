// Program created based on the "C Programming Full Course for free" tutorial
// To run it:
// gcc UserInput.c -o UserInput.o && ./UserInput.o
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef char userName[25];

// This is Function prototype.
// It ensures that calls to this function pass the correct arguments.
// Without this the compiler won't bother if it is missing an argument.
void hello(const char[], int);
void trimString(char *);

// Example of normal function (not a prototype).
bool isStringEmpty(const char name[]) {
  int len = strlen(name);
  return len == 1 && name[len - 1] == '\n';
}

void sort(int arr[], int size) {
  // int size = sizeof(*arr) / sizeof(*arr[0]);
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (arr[j] > arr[j + 1]) {
        int tmp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = tmp;
      }
    }
  }
}

struct Player {
  char name[12];
  int score;
};

enum Day { Sun = 1, Mon = 2, Tue = 3, Wed = 4, Thu = 5, Fri = 6, Sat = 7 };

int main() {
  char name[25]; // bytes
  int age;

  printf("\nWhat is your name?");
  // scanf reads up to any whitespace, that is why we need
  // to use instead fgets.
  // scanf("%s", name);
  fgets(name, 25, stdin);

  trimString(name);
  if (isStringEmpty(name)) {
    printf("Name cannot be empty");
    return 1;
  }

  // get rid of the '\n' at the end.
  name[strlen(name) - 1] = '\0';

  printf("\nWhat is your age?");
  scanf("%d", &age);

  if (age < 0 || age > 100) {
    printf("Age is invalid");
    return 1;
  }

  hello(name, age);

  printf("\n");

  for (int i = 0; i < 10; i++) {
    printf("Loop, index %d\n", i);
  }

  printf("\n");

  const double prices[] = {10.0, 50.0, 100.0};
  // Go get the length of an array, we need to divide the size in bytes of the
  // the whole array by the size of a single element of that array. In this
  // case, it is an array of doubles, each double has a 8 bytes size, and the
  // whole array has 24 bytes, since it contains 3 items.
  int pricesSize = sizeof(prices) / sizeof(prices[0]);
  for (int i = 0; i < pricesSize; i++) {
    printf("Price: $%.2lf\n", prices[i]);
  }

  printf("\n");

  // 2d arrays
  int matrix[2][1] = {{100}, {200}};
  int rowsSize = sizeof(matrix) / sizeof(matrix[0]);
  for (int i = 0; i < rowsSize; i++) {
    int cellsSize = sizeof(matrix[i]) / sizeof(matrix[i][0]);
    for (int j = 0; j < cellsSize; j++) {
      printf("Matrix: [%d][%d]=%d\n", i, j, matrix[i][j]);
    }
  }

  printf("\n");

  // an array of "strings" where each string has max length of 10
  char cars[][10] = {"Mustang", "Camaro"};
  strcpy(cars[0], "Tesla");
  int carsSize = sizeof(cars) / sizeof(cars[0]);
  for (int i = 0; i < carsSize; i++) {
    printf("Car: %s\n", cars[i]);
  }

  printf("\n");

  int arr[] = {5, 3, 1, 4, 2};
  int arrSize = sizeof(arr) / sizeof(arr[0]);
  sort(arr, arrSize);
  printf("Array :");
  for (int i = 0; i < arrSize; i++) {
    printf(" %d", arr[i]);
  }

  printf("\n\n");

  struct Player player1;
  strcpy(player1.name, "Max");
  player1.score = 100;

  struct Player player2 = {"Jake", 20};

  printf("Player1: %s - %d", player1.name, player1.score);
  printf("Player2: %s - %d", player2.name, player2.score);

  printf("\n");

  userName user1 = "Max";
  printf("Username: %s\n", user1);

  printf("\n");

  enum Day today = Wed;
  printf("Day %d", today);

  return 0;
}

void hello(const char name[], int age) {
  printf("\nHello %s, how are you?", name);
  printf("\nYou are %d years old.", age);
}

// based on https://www.delftstack.com/howto/c/trim-string-in-c/
void trimString(char *str) {
  // Points to the first non-whitespace character
  char *start = str + strspn(str, " \t\n\r");

  // Points to the last character of the string
  char *end = str + strlen(str) - 1;

  // Move the end pointer backward while it points to whitespace.
  // First, compare if the end pointer is biggen than start pointer.
  // Then, if end got to a non-whitespace char.
  while (end > start &&
         (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
    // printf("  while end='%c'\n", *end);
    --end;
  }

  // Place the null terminator after the last non-whitespace character
  *(end + 1) = '\0';

  if (start > str) {
    // Move the trimmed string to the start of the buffer.
    // +2 to include the last character and null terminator
    memmove(str, start, end - start + 2);
  }
}
