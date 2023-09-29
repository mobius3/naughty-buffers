#include "naughty-buffers/buffer.h"
#include <assert.h>

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

int main(void) {
  insert_increases_count_correctly();
  insert_stores_the_right_values();
  insert_store_values_not_addresses();
  insert_properly_stretches_the_buffer();
  insert_properly_keeps_other_values();

  return 0;
}