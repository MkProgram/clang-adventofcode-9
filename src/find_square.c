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

static bool on_segment(Point p, Point a, Point b) {
  if (a.x == b.x) {
    return p.x == a.x &&
           ((a.y <= p.y && p.y <= b.y) || (b.y <= p.y && p.y <= a.y));
  }

  if (a.y == b.y) {
    return p.y == a.y &&
           ((a.x <= p.x && p.x <= b.x) || (b.x <= p.x && p.x <= a.x));
  }

  return false;
}

static bool point_in_polygon(Point p, const Vec *points) {
  bool inside = false;

  for (size_t i = 0; i < points->count; ++i) {
    Point a = {0};
    Point b = {0};
    vec_get(points, i, &a);
    vec_get(points, (i + 1) % points->count, &b);

    if (on_segment(p, a, b)) {
      return true;
    }

    if (a.x == b.x &&
        ((a.y <= p.y && p.y < b.y) || (b.y <= p.y && p.y < a.y)) && a.x > p.x) {
      inside = !inside;
    }
  }

  return inside;
}

static bool rectangle_inside(Point p1, Point p2, Vec *points) {
  size_t min_x = p1.x < p2.x ? p1.x : p2.x;
  size_t min_y = p1.y < p2.y ? p1.y : p2.y;
  size_t max_x = p1.x > p2.x ? p1.x : p2.x;
  size_t max_y = p1.y > p2.y ? p1.y : p2.y;

  for (size_t y = min_y;; ++y) {
    for (size_t x = min_x;; ++x) {
      Point point = {x, y};
      if (!point_in_polygon(point, points)) {
        return false;
      }
      if (x == max_x)
        break;
    }
    if (y == max_y)
      break;
  }

  return true;
}

static bool rectangle_has_boundary_inside(Point p1, Point p2, Vec *points) {
  size_t min_x = p1.x < p2.x ? p1.x : p2.x;
  size_t min_y = p1.y < p2.y ? p1.y : p2.y;
  size_t max_x = p1.x > p2.x ? p1.x : p2.x;
  size_t max_y = p1.y > p2.y ? p1.y : p2.y;

  for (size_t i = 0; i < points->count; ++i) {
    Point a = {0};
    Point b = {0};
    vec_get(points, i, &a);
    vec_get(points, (i + 1) % points->count, &b);

    if (a.x == b.x) {
      if (min_x < a.x && a.x < max_x && a.y < max_y && b.y > min_y) {
        return true;
      }
      if (min_x < a.x && a.x < max_x && b.y < max_y && a.y > min_y) {
        return true;
      }
    } else {
      if (min_y < a.y && a.y < max_y && a.x < max_x && b.x > min_x) {
        return true;
      }
      if (min_y < a.y && a.y < max_y && b.x < max_x && a.x > min_x) {
        return true;
      }
    }
  }
  return false;
}

bool largest_rectangle_area(Vec *points, PuzzleSolution *solution) {
  bool square_found = false;
  PointSquare best_rect = {0};
  PointSquare best_rect_inside = {0};
  bool rect_inside_found = false;
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
      bool rectangle_boundary =
          rectangle_has_boundary_inside(point_a, point_b, points);

      if (!rectangle_boundary &&
          (!rect_inside_found || rect.area > best_rect_inside.area)) {
        best_rect_inside = rect;
        rect_inside_found = true;
      }
    }
  }
  solution->square = best_rect;
  solution->part1 = best_rect.area;
  if (rect_inside_found) {
    solution->part2 = best_rect_inside.area;
    solution->part2_found = true;
  }
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
