#include "criterion/criterion.h"
#include "naughty-buffers/buffer.h"

Test(push, push_increases_count_correctly) {
  struct nb_buffer buffer;
  uint32_t value = 0;
  nb_init(&buffer, sizeof(uint32_t));
  cr_assert(buffer.block_count == 0);
  nb_push(&buffer, &value);
  cr_assert(buffer.block_count == 1);
  nb_push(&buffer, &value);
  cr_assert(buffer.block_count == 2);
  nb_push(&buffer, &value);
  cr_assert(buffer.block_count == 3);
  nb_release(&buffer);
}

Test(push, push_stores_the_right_values) {
  struct nb_buffer buffer;
  uint32_t value = 0;
  uint32_t * read_value = NULL;

  nb_init(&buffer, sizeof(uint32_t));

  nb_push(&buffer, &value);
  read_value = nb_at(&buffer, 0);
  cr_assert(*read_value == 0);

  value = 3;
  nb_push(&buffer, &value);
  read_value = nb_at(&buffer, 1);
  cr_assert(*read_value == 3);

  value = 33333;
  nb_push(&buffer, &value);
  read_value = nb_at(&buffer, 2);
  cr_assert(*read_value == 33333);

  nb_release(&buffer);
}

Test(push, push_store_values_not_addresses) {
  struct nb_buffer buffer;
  uint32_t value = 0;
  uint32_t * read_value = NULL;

  nb_init(&buffer, sizeof(uint32_t));

  nb_push(&buffer, &value);
  read_value = nb_at(&buffer, 0);
  cr_assert(read_value != &value);

  value = 3;
  nb_push(&buffer, &value);
  read_value = nb_at(&buffer, 1);
  cr_assert(read_value != &value);

  value = 33333;
  nb_push(&buffer, &value);
  read_value = nb_at(&buffer, 2);
  cr_assert(read_value != &value);

  nb_release(&buffer);
}