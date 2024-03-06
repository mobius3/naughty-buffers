#ifndef NB_TESTS_COMPLEX_H
#define NB_TESTS_COMPLEX_H

#include <math.h>

#include "naughty-buffers/buffer.h"

struct complex_t {
  double real;
  double imaginary;
};

static enum NB_COMPARE_RESULT complex_compare(const void * ptr_a, const void * ptr_b) {
  const struct complex_t * a = ptr_a;
  const struct complex_t * b = ptr_b;
  const double real_diff = a->real - b->real;
  const double imaginary_delta = a->imaginary - b->imaginary;
  if (fabs(real_diff) < 0.000001) {
    if (fabs(imaginary_delta) < 0.000001) return NB_COMPARE_EQUAL;
    if (imaginary_delta > 0) return NB_COMPARE_HIGHER;
    if (imaginary_delta < 0) return NB_COMPARE_LOWER;
  }
  else {
    if (real_diff > 0) return NB_COMPARE_HIGHER;
    if (real_diff < 0) return NB_COMPARE_LOWER;
  }
  return NB_COMPARE_EQUAL;
}

#endif
