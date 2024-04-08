// Program created based on the "C Programming Full Course for free" tutorial
// To run it:
// gcc GuessNumber.c -o GuessNumber.o && ./GuessNumber.o
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  const int MIN = 0;
  const int MAX = 10;

  int guess, guesses = 0, answer;

  // uses the current time as a seed
  srand(time(0));

  // generate random number between MIN and Max
  answer = (rand() % MAX) + MIN;

  do {
    printf("Guess a number: ");
    scanf("%d", &guess);

    if (guess > answer) {
      printf("Too high!\n");
    } else if (guess < answer) {
      printf("Too low!\n");
    } else {
      printf("Correct!\n");
    }
    guesses++;
  } while (guess != answer);

  printf("answer %d\n", answer);
  printf("guesses %d\n", guesses);

  return 0;
}
