#include "naughty-buffers/buffer.h"
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "complex.h"

static enum NB_COMPARE_RESULT uint32_compare(const void * ptr_a, const void * ptr_b) {
  const uint32_t * a = ptr_a;
  const uint32_t * b = ptr_b;

  if (*a > *b) return NB_COMPARE_HIGHER;
  if (*a < *b) return NB_COMPARE_LOWER;
  return NB_COMPARE_EQUAL;
}

void insert_increases_count_correctly() {
  struct nb_buffer buffer;
  uint32_t value = 0;
  nb_init(&buffer, sizeof(uint32_t));
  assert(buffer.block_count == 0);
  nb_insert(&buffer, 0, &value);
  assert(buffer.block_count == 1);
  nb_insert(&buffer, 1, &value);
  assert(buffer.block_count == 2);
  nb_insert(&buffer, 2, &value);
  assert(buffer.block_count == 3);
  nb_insert(&buffer, 9, &value);
  assert(buffer.block_count == 10);
  nb_release(&buffer);
}

void insert_stores_the_right_values() {
  struct nb_buffer buffer;
  uint32_t value = 0;
  uint32_t * read_value = NULL;

  nb_init(&buffer, sizeof(uint32_t));

  nb_insert(&buffer, 0, &value);
  read_value = nb_at(&buffer, 0);
  assert(*read_value == 0);

  value = 3;
  nb_insert(&buffer, 0, &value);
  read_value = nb_at(&buffer, 0);
  assert(*read_value == 3);

  value = 33333;
  nb_insert(&buffer, 0, &value);
  read_value = nb_at(&buffer, 0);
  assert(*read_value == 33333);

  nb_release(&buffer);
}

void insert_store_values_not_addresses() {
  struct nb_buffer buffer;
  uint32_t value = 0;
  uint32_t * read_value = NULL;

  nb_init(&buffer, sizeof(uint32_t));

  nb_insert(&buffer, 0, &value);
  read_value = nb_at(&buffer, 0);
  assert(read_value != &value);

  value = 3;
  nb_insert(&buffer, 1, &value);
  read_value = nb_at(&buffer, 1);
  assert(read_value != &value);

  value = 33333;
  nb_insert(&buffer, 2, &value);
  read_value = nb_at(&buffer, 2);
  assert(read_value != &value);

  nb_release(&buffer);
}

void insert_properly_stretches_the_buffer() {
  struct nb_buffer buffer;
  uint32_t value = 0;

  nb_init(&buffer, sizeof(uint32_t));

  nb_insert(&buffer, 33, &value);
  assert(buffer.block_count == 34);
  assert(buffer.block_capacity >= 33);

  nb_insert(&buffer, 63, &value);
  assert(buffer.block_count == 64);
  assert(buffer.block_capacity >= 63);

  nb_release(&buffer);
}

void insert_properly_keeps_other_values() {
  struct nb_buffer buffer;
  uint32_t value = 0;
  uint32_t * read_value = NULL;

  nb_init(&buffer, sizeof(uint32_t));

  nb_insert(&buffer, 0, &value);
  read_value = nb_at(&buffer, 0);
  assert(*read_value == 0);

  value = 3;
  nb_insert(&buffer, 0, &value);
  read_value = nb_at(&buffer, 1);
  assert(*read_value == 0);

  value = 33333;
  nb_insert(&buffer, 0, &value);

  read_value = nb_at(&buffer, 1);
  assert(*read_value == 3);
  read_value = nb_at(&buffer, 2);
  assert(*read_value == 0);

  value = 222;
  nb_insert(&buffer, 1, &value);

  read_value = nb_at(&buffer, 0);
  assert(*read_value == 33333);
  read_value = nb_at(&buffer, 2);
  assert(*read_value == 3);
  read_value = nb_at(&buffer, 3);
  assert(*read_value == 0);

  nb_release(&buffer);
}

void insert_sorted_reversed_values_works() {
  struct nb_buffer buffer;

  nb_init(&buffer, sizeof(uint32_t));
  uint32_t values[10] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

  for (int i = 0; i < 10; i++) {
    nb_insert_sorted(&buffer, uint32_compare, &values[i]);
  }

  assert(nb_block_count(&buffer) == 10);

  for (size_t i = 1; i < 10; i++) {
    const enum NB_COMPARE_RESULT comparison = uint32_compare(
      nb_at(&buffer, i -1),
      nb_at(&buffer, i)
    );
    assert(comparison != NB_COMPARE_HIGHER);
  }

  nb_release(&buffer);
}

void insert_sorted_random_values_works() {
  struct nb_buffer buffer;

  nb_init(&buffer, sizeof(uint32_t));

  const size_t value_count = rand() % 99 + 2;
  for (int i = 0; i < value_count; i++) {
    uint32_t value = rand() % value_count;
    nb_insert_sorted(&buffer, uint32_compare, &value);
  }

  assert(nb_block_count(&buffer) == value_count);

  for (size_t i = 1; i < value_count; i++) {
    const enum NB_COMPARE_RESULT comparison = uint32_compare(
      nb_at(&buffer, i -1),
      nb_at(&buffer, i)
    );
    assert(comparison != NB_COMPARE_HIGHER);
  }

  nb_release(&buffer);
}


void insert_sorted_sorted_values_works() {
  struct nb_buffer buffer;

  nb_init(&buffer, sizeof(uint32_t));
  uint32_t values[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  for (int i = 0; i < 10; i++) {
    nb_insert_sorted(&buffer, uint32_compare, &values[i]);
  }

  assert(nb_block_count(&buffer) == 10);

  for (size_t i = 1; i < 10; i++) {
    const enum NB_COMPARE_RESULT comparison = uint32_compare(
      nb_at(&buffer, i -1),
      nb_at(&buffer, i)
    );
    assert(comparison != NB_COMPARE_HIGHER);
  }

  nb_release(&buffer);
}

void insert_sorted_with_complex_data_works() {
  struct nb_buffer buffer;
  nb_init(&buffer, sizeof(struct complex_t));

  struct complex_t complex[] = {
    { .real = 2, .imaginary = -0.002 },
    { .real = 2.001, .imaginary = -0.2 },
    { .real = 2.0001, .imaginary = -0.2 },
    { .real = 0, .imaginary = 0 },
    { .real = 0, .imaginary = 1 },
    { .real = 0, .imaginary = -1 },
    { .real = 1, .imaginary = 0 },
    { .real = 1, .imaginary = 0.1 },
    { .real = 1, .imaginary = 0.2 },
    { .real = 2, .imaginary = -0.2 },
  };

  struct complex_t expected[] = {
    { .real = 0.000000, .imaginary = -1.000000 },
    { .real = 0.000000, .imaginary = 0.000000 },
    { .real = 0.000000, .imaginary = 1.000000 },
    { .real = 1.000000, .imaginary = 0.000000 },
    { .real = 1.000000, .imaginary = 0.100000 },
    { .real = 1.000000, .imaginary = 0.200000 },
    { .real = 2.000000, .imaginary = -0.200000 },
    { .real = 2.000000, .imaginary = -0.002000 },
    { .real = 2.000100, .imaginary = -0.200000 },
    { .real = 2.001000, .imaginary = -0.200000 },
  };

  for (size_t i = 0; i < 10; i++) {
    nb_insert_sorted(&buffer, complex_compare, &complex[i]);
  }

  for (size_t i = 0; i < nb_block_count(&buffer); i++) {
    assert(complex_compare(nb_at(&buffer, i), &expected[i]) == NB_COMPARE_EQUAL);
  }
}

int main(void) {
  srand(time(0));
  insert_increases_count_correctly();
  insert_stores_the_right_values();
  insert_store_values_not_addresses();
  insert_properly_stretches_the_buffer();
  insert_properly_keeps_other_values();
  insert_sorted_reversed_values_works();
  insert_sorted_random_values_works();
  insert_sorted_sorted_values_works();
  insert_sorted_with_complex_data_works();

  return 0;
}
