#include "find_square.h"
#include <stdio.h>
#include <stdlib.h>
int main(void) {
  printf("Lets make some squares!\n");
  char *floc = "input.txt";
  FILE *file = NULL;

  file = fopen(floc, "r");
  if (file == NULL) {
    fprintf(stderr, "Can't open file: %s\n", floc);
    return EXIT_FAILURE;
  }
  PuzzleSolution solution = find_largest_square(file);
  printf("Solution of part 1: %lld\n", solution.part1);

  if (solution.part2_found) {
    printf("Solution for part 2: %lld\n", solution.part2);
  }

  return EXIT_SUCCESS;
}
