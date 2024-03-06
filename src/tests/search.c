#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "complex.h"
#include "naughty-buffers/buffer.h"

static enum NB_COMPARE_RESULT uint32_compare(const void * ptr_a, const void * ptr_b) {
  const uint32_t * a = ptr_a;
  const uint32_t * b = ptr_b;

  if (*a > *b) return NB_COMPARE_HIGHER;
  if (*a < *b) return NB_COMPARE_LOWER;
  return NB_COMPARE_EQUAL;
}


void search_works() {
  struct nb_buffer buffer;
  nb_init(&buffer, sizeof(uint32_t));
  uint32_t values[10] = { 0, 11, 22, 33, 44, 55, 66, 77, 88, 99 };
  for (size_t i = 0; i < 10; i++) {
    nb_insert_sorted(&buffer, uint32_compare, &values[i]);
  }

  uint32_t search_value = 55;
  uint32_t * found_value = nb_search(&buffer, &search_value, uint32_compare);
  assert(*found_value == 55);

  search_value = 1;
  assert(nb_search(&buffer, &search_value, uint32_compare) == NULL);
}

void search_with_complex_data_works() {
  struct nb_buffer buffer;
  nb_init(&buffer, sizeof(struct complex_t));

  struct complex_t complex[] = {
    { 0, 0 },
    { 0, 1 },
    { 1, 1 },
    { 1, 2 },
    { 2, 2 },
    { 2, 3 },
    { 3, 3 },
    { 3, 4 },
    { 4, 5 },
    { 5, 5 },
  };

  struct complex_t lookup = { 4, 5 };

  for (size_t i = 0; i < 10; i++) {
    nb_insert_sorted(&buffer, complex_compare, &complex[i]);
  }

  assert(
    complex_compare(
      &lookup,
      nb_search(&buffer, &lookup, complex_compare)
    ) == NB_COMPARE_EQUAL
  );
}


int main() {
  search_works();
  search_with_complex_data_works();
}
