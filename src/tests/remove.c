#include "criterion/criterion.h"
#include "naughty-buffers/buffer.h"
#include <stdio.h>

uint32_t buffer_read_uint32_t(struct nb_buffer * buffer, size_t index) {
  uint32_t * read_value = nb_at(buffer, index);
  return *read_value;
}

Test(remove, remove_decreases_count_correctly) {
  struct nb_buffer buffer;
  uint32_t value = 0;
  nb_init(&buffer, sizeof(uint32_t));
  cr_assert(buffer.block_count == 0);
  nb_push(&buffer, &value);
  nb_push(&buffer, &value);
  nb_push(&buffer, &value);
  nb_push(&buffer, &value);
  nb_push(&buffer, &value);
  nb_push(&buffer, &value);

  nb_remove_back(&buffer);
  cr_assert(nb_block_count(&buffer) == 5);

  nb_remove_front(&buffer);
  cr_assert(nb_block_count(&buffer) == 4);

  nb_remove_at(&buffer, 2);
  cr_assert(nb_block_count(&buffer) == 3);

  nb_release(&buffer);
}

Test(remove, remove_keeps_values_and_ordering) {
  struct nb_buffer buffer;

  nb_init(&buffer, sizeof(uint32_t));

  for (uint32_t i = 0; i < 10; i++) { nb_push(&buffer, &i); }

  nb_remove_front(&buffer);
  printf("%u\n", buffer_read_uint32_t(&buffer, 0));
  cr_expect_eq(buffer_read_uint32_t(&buffer, 0), 1);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 1), 2);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 2), 3);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 3), 4);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 4), 5);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 5), 6);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 6), 7);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 7), 8);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 8), 9);

  nb_remove_back(&buffer);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 0), 1);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 1), 2);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 2), 3);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 3), 4);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 4), 5);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 5), 6);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 6), 7);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 7), 8);

  nb_remove_at(&buffer, 3);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 0), 1);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 1), 2);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 2), 3);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 3), 5);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 4), 6);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 5), 7);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 6), 8);

  nb_remove_at(&buffer, 4);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 0), 1);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 1), 2);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 2), 3);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 3), 5);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 4), 7);
  cr_expect_eq(buffer_read_uint32_t(&buffer, 5), 8);

  nb_release(&buffer);
}