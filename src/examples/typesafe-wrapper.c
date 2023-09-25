#include "naughty-buffers/array-generator.h"
#include <stdio.h>

struct my_t {
  uint32_t hi;
  uint32_t lo;
};

NAUGHTY_BUFFERS_ARRAY_DECLARATION(t_array, struct my_t)
NAUGHTY_BUFFERS_ARRAY_DEFINITION(t_array, struct my_t)

int main(void) {
  struct t_array array;
  t_array_init(&array);

  printf("filling with values:\n");

  struct my_t value1 = { 10, 10 };
  t_array_push(&array, value1);

  struct my_t value2 = { 20, 20 };
  t_array_push(&array, value2);

  struct my_t value3 = { 30, 30 };
  t_array_push(&array, value3);

  for (size_t i = 0; i < t_array_count(&array); i++) {
    struct my_t read_value = t_array_at(&array, i);
    printf("%lu: { .hi = %d, .lo = %d }\n", i, read_value.hi, read_value.lo);
  }

  t_array_remove_front(&array);
  printf("\nafter remove_front: \n");

  for (size_t i = 0; i < t_array_count(&array); i++) {
    struct my_t read_value = t_array_at(&array, i);
    printf("%lu: { .hi = %d, .lo = %d }\n", i, read_value.hi, read_value.lo);
  }

  t_array_remove_back(&array);
  printf("\nafter remove_back: \n");

  for (size_t i = 0; i < t_array_count(&array); i++) {
    struct my_t read_value = t_array_at(&array, i);
    printf("%lu: { .hi = %d, .lo = %d }\n", i, read_value.hi, read_value.lo);
  }

  struct my_t value5 = { .hi = 50, .lo = 50 };
  t_array_assign(&array, 5, value5);


  printf("\nafter assign index 5 (expect random values between index 1 to 4): \n");

  for (size_t i = 0; i < t_array_count(&array); i++) {
    struct my_t read_value = t_array_at(&array, i);
    printf("%lu: { .hi = %d, .lo = %d }\n", i, read_value.hi, read_value.lo);
  }

  t_array_release(&array);

  return 0;
}