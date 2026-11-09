#include "find_square.h"
#include <stdio.h>
#include <stdlib.h>

struct CalculateRectCase {
  Point point_a;
  Point point_b;
  long long expected_area;
};

static struct CalculateRectCase rectCases[] = {
    {(Point){2, 5}, (Point){9, 7}, 24},
    {(Point){7, 1}, (Point){11, 7}, 35},
    {(Point){7, 3}, (Point){2, 3}, 6},
    {(Point){2, 5}, (Point){11, 1}, 50},

};

size_t test_calculate_rect(void) {
  size_t failures = 0;

  for (size_t i = 0; i < sizeof rectCases / sizeof rectCases[0]; ++i) {
    struct CalculateRectCase current = rectCases[i];
    PointSquare rect = calculate_rect(current.point_a, current.point_b);
    if (rect.area != current.expected_area) {
      fprintf(stderr,
              "FAILURE: For point (%zu, %zu) and (%zu, %zu) expect area was "
              "%lld, got %lld \n",
              current.point_a.x, current.point_a.y, current.point_b.x,
              current.point_b.y, current.expected_area, rect.area);
      ++failures;
    }
  }
  return failures;
}

size_t test_find_example(void) {
  long long expected_solution = 50;
  char *floc = "tests/example.txt";
  FILE *file = NULL;
  file = fopen(floc, "r");
  if (file == NULL) {
    fprintf(stderr, "Can't open file \"%s\"\n", floc);
    return 1;
  }
  PuzzleSolution solution = find_largest_square(file);
  if (solution.part1 != expected_solution) {
    fprintf(stderr, "FAILURE: Expected solution was %lld, got %lld\n",
            expected_solution, solution.part1);
    return 1;
  }
  return 0;
}

int main(void) {
  size_t failures = 0;

  failures += test_calculate_rect();
  failures += test_find_example();
  if (failures == 0) {
    printf("All tests passed!\n");
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}
