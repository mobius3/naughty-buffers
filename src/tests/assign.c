#include "criterion/criterion.h"
#include "naughty-buffers/buffer.h"
#include <stdio.h>

Test(assign, assign_increases_count_correctly) {
  struct nb_buffer buffer;
  uint32_t value = 0;
  nb_init(&buffer, sizeof(uint32_t));
  cr_assert(buffer.block_count == 0);
  nb_assign(&buffer, 0, &value);
  cr_assert(buffer.block_count == 1);
  nb_assign(&buffer, 1, &value);
  cr_assert(buffer.block_count == 2);
  nb_assign(&buffer, 2, &value);
  cr_assert(buffer.block_count == 3);
  nb_assign(&buffer, 9, &value);
  cr_assert(buffer.block_count == 10);
  nb_release(&buffer);
}

Test(assign, assign_stores_the_right_values) {
  struct nb_buffer buffer;
  uint32_t value = 0;
  uint32_t * read_value = NULL;

  nb_init(&buffer, sizeof(uint32_t));

  nb_assign(&buffer, 0, &value);
  read_value = nb_at(&buffer, 0);
  cr_assert(*read_value == 0);

  value = 3;
  nb_assign(&buffer, 0, &value);
  read_value = nb_at(&buffer, 0);
  cr_assert(*read_value == 3);

  value = 33333;
  nb_assign(&buffer, 0, &value);
  read_value = nb_at(&buffer, 0);
  cr_assert(*read_value == 33333);

  nb_release(&buffer);
}

Test(assign, assign_store_values_not_addresses) {
  struct nb_buffer buffer;
  uint32_t value = 0;
  uint32_t * read_value = NULL;

  nb_init(&buffer, sizeof(uint32_t));

  nb_assign(&buffer, 0, &value);
  read_value = nb_at(&buffer, 0);
  cr_assert(read_value != &value);

  value = 3;
  nb_assign(&buffer, 1, &value);
  read_value = nb_at(&buffer, 1);
  cr_assert(read_value != &value);

  value = 33333;
  nb_assign(&buffer, 2, &value);
  read_value = nb_at(&buffer, 2);
  cr_assert(read_value != &value);

  nb_release(&buffer);
}

Test(assign, assign_properly_stretches_the_buffer) {
  struct nb_buffer buffer;
  uint32_t value = 0;

  nb_init(&buffer, sizeof(uint32_t));

  nb_assign(&buffer, 33, &value);
  cr_assert(buffer.block_count == 34);
  cr_assert(buffer.block_capacity >= 33);

  nb_assign(&buffer, 63, &value);
  cr_assert(buffer.block_count == 64);
  cr_assert(buffer.block_capacity >= 63);

  nb_release(&buffer);
}