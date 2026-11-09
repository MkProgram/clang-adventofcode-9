#include "find_square.h"
#include "vec.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

PointSquare calculate_rect(Point point_a, Point point_b) {
  long long width = (long long)point_a.x - (long long)point_b.x;
  long long height = (long long)point_a.y - (long long)point_b.y;

  if (width < 0) {
    width = -width;
  }
  if (height < 0) {
    height = -height;
  }
  width += 1;
  height += 1;

  long long area = width * height;

  return (PointSquare){point_a, point_b, area};
}

bool largest_rectangle_area(Vec *points, PuzzleSolution *solution) {
  bool square_found = false;
  PointSquare best_rect = {0};
  for (size_t i = 0; i < points->count; ++i) {
    Point point_a = {0};
    vec_get(points, i, &point_a);
    for (size_t j = i + 1; j < points->count; ++j) {
      Point point_b = {0};
      vec_get(points, j, &point_b);

      PointSquare rect = calculate_rect(point_a, point_b);
      if (!square_found || rect.area > best_rect.area) {
        best_rect = rect;
        square_found = true;
      }
    }
  }
  solution->square = best_rect;
  solution->part1 = best_rect.area;
  return square_found;
}

size_t create_points(FILE *file, Vec *out) {

  size_t linecap = 0;
  char *line = NULL;
  size_t count = 0;

  while (getline(&line, &linecap, file) != -1) {
    int x = 0;
    int y = 0;

    if (sscanf(line, "%d,%d", &x, &y) != 2) {
      fprintf(stderr, "Cannot extract points data");
      continue;
    }
    vec_push(out, &(Point){(size_t)x, (size_t)y});
    ++count;
  }

  if (line != NULL) {
    free(line);
  }
  return count;
}

PuzzleSolution find_largest_square(FILE *file) {
  PuzzleSolution solution = {0};
  Vec points = {0};

  vec_init(&points, sizeof(Point));
  create_points(file, &points);
  if (!largest_rectangle_area(&points, &solution)) {
    fprintf(stderr, "No rectangle could be found\n");
  }

  vec_free(&points);
  return solution;
}
