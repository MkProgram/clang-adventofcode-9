#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
  size_t x;
  size_t y;
} Point;

typedef struct {
  Point point_a;
  Point point_b;
  long long area;
} PointSquare;

typedef struct {
  PointSquare square;
  long long part1;
  bool part2_found;
  long long part2;
} PuzzleSolution;

PointSquare calculate_rect(Point point_a, Point point_b);
PuzzleSolution find_largest_square(FILE *file);
