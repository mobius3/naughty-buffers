#include "naughty-buffers/buffer.h"
#include <assert.h>

void assign_many_increases_count_correctly() {
  struct nb_buffer buffer;
  uint32_t value[] = { 0, 1, 2 };
  nb_init(&buffer, sizeof(uint32_t));
  assert(buffer.block_count == 0);
  nb_assign_many(&buffer, 0, &value, 3);
  assert(buffer.block_count == 3);
  nb_assign_many(&buffer, 3, &value, 3);
  assert(buffer.block_count == 6);
  nb_assign_many(&buffer, 3, &value, 6);
  assert(buffer.block_count == 9);
  nb_assign_many(&buffer, 18, &value, 3);
  assert(buffer.block_count == 21);
  nb_release(&buffer);
}

void assign_many_stores_the_right_values() {
  struct nb_buffer buffer;
  uint32_t value_1[] = { 0, 1, 2 };
  uint32_t * read_value = NULL;

  nb_init(&buffer, sizeof(uint32_t));

  nb_assign_many(&buffer, 0, &value_1, 3);
  for (size_t i = 0; i < 3; i++) {
    read_value = nb_at(&buffer, i);
    assert(*read_value == value_1[i]);
  }

  uint32_t value_2[] = { 3, 4, 5 };
  nb_assign_many(&buffer, 0, &value_2, 3);
  for (size_t i = 0; i < 3; i++) {
    read_value = nb_at(&buffer, i);
    assert(*read_value == value_2[i]);
  }

  uint32_t value_3[] = { 33333, 44444, 55555 };
  nb_assign_many(&buffer, 0, value_3, 3);
  for (size_t i = 0; i < 3; i++) {
    read_value = nb_at(&buffer, i);
    assert(*read_value == value_3[i]);
  }

  nb_release(&buffer);
}

void assign_many_store_values_not_addresses() {
  struct nb_buffer buffer;
  uint32_t value[] = { 0, 1, 2, 3 };
  uint32_t * read_value = NULL;

  nb_init(&buffer, sizeof(uint32_t));

  nb_assign_many(&buffer, 0, &value, 4);
  for (size_t i = 0; i < 4; i++) {
    read_value = nb_at(&buffer, i);
    assert(read_value != &value[i]);
  }

  nb_release(&buffer);
}

void assign_many_properly_stretches_the_buffer() {
  struct nb_buffer buffer;
  uint32_t value[] = { 0, 1, 2 };

  nb_init(&buffer, sizeof(uint32_t));

  nb_assign_many(&buffer, 33, &value, 3);
  assert(buffer.block_count == 36);
  assert(buffer.block_capacity >= 35);


  nb_assign_many(&buffer, 63, &value, 3);
  assert(buffer.block_count == 66);
  assert(buffer.block_capacity >= 67);

  nb_release(&buffer);
}

int main(void) {
  assign_many_increases_count_correctly();
  assign_many_stores_the_right_values();
  assign_many_store_values_not_addresses();
  assign_many_properly_stretches_the_buffer();
}
