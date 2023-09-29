#include "naughty-buffers/buffer.h"
#include <assert.h>

#define assert_eq(a, b) assert((a) == (b))

uint32_t buffer_read_uint32_t(struct nb_buffer * buffer, size_t index) {
  uint32_t * read_value = nb_at(buffer, index);
  return *read_value;
}

void remove_decreases_count_correctly() {
  struct nb_buffer buffer;
  uint32_t value = 0;
  nb_init(&buffer, sizeof(uint32_t));
  assert(buffer.block_count == 0);
  nb_push(&buffer, &value);
  nb_push(&buffer, &value);
  nb_push(&buffer, &value);
  nb_push(&buffer, &value);
  nb_push(&buffer, &value);
  nb_push(&buffer, &value);

  nb_remove_back(&buffer);
  assert(nb_block_count(&buffer) == 5);

  nb_remove_front(&buffer);
  assert(nb_block_count(&buffer) == 4);

  nb_remove_at(&buffer, 2);
  assert(nb_block_count(&buffer) == 3);

  nb_release(&buffer);
}

void remove_keeps_values_and_ordering() {
  struct nb_buffer buffer;

  nb_init(&buffer, sizeof(uint32_t));

  for (uint32_t i = 0; i < 10; i++) { nb_push(&buffer, &i); }

  nb_remove_front(&buffer);
  assert_eq(buffer_read_uint32_t(&buffer, 0), 1);
  assert_eq(buffer_read_uint32_t(&buffer, 1), 2);
  assert_eq(buffer_read_uint32_t(&buffer, 2), 3);
  assert_eq(buffer_read_uint32_t(&buffer, 3), 4);
  assert_eq(buffer_read_uint32_t(&buffer, 4), 5);
  assert_eq(buffer_read_uint32_t(&buffer, 5), 6);
  assert_eq(buffer_read_uint32_t(&buffer, 6), 7);
  assert_eq(buffer_read_uint32_t(&buffer, 7), 8);
  assert_eq(buffer_read_uint32_t(&buffer, 8), 9);

  nb_remove_back(&buffer);
  assert_eq(buffer_read_uint32_t(&buffer, 0), 1);
  assert_eq(buffer_read_uint32_t(&buffer, 1), 2);
  assert_eq(buffer_read_uint32_t(&buffer, 2), 3);
  assert_eq(buffer_read_uint32_t(&buffer, 3), 4);
  assert_eq(buffer_read_uint32_t(&buffer, 4), 5);
  assert_eq(buffer_read_uint32_t(&buffer, 5), 6);
  assert_eq(buffer_read_uint32_t(&buffer, 6), 7);
  assert_eq(buffer_read_uint32_t(&buffer, 7), 8);

  nb_remove_at(&buffer, 3);
  assert_eq(buffer_read_uint32_t(&buffer, 0), 1);
  assert_eq(buffer_read_uint32_t(&buffer, 1), 2);
  assert_eq(buffer_read_uint32_t(&buffer, 2), 3);
  assert_eq(buffer_read_uint32_t(&buffer, 3), 5);
  assert_eq(buffer_read_uint32_t(&buffer, 4), 6);
  assert_eq(buffer_read_uint32_t(&buffer, 5), 7);
  assert_eq(buffer_read_uint32_t(&buffer, 6), 8);

  nb_remove_at(&buffer, 4);
  assert_eq(buffer_read_uint32_t(&buffer, 0), 1);
  assert_eq(buffer_read_uint32_t(&buffer, 1), 2);
  assert_eq(buffer_read_uint32_t(&buffer, 2), 3);
  assert_eq(buffer_read_uint32_t(&buffer, 3), 5);
  assert_eq(buffer_read_uint32_t(&buffer, 4), 7);
  assert_eq(buffer_read_uint32_t(&buffer, 5), 8);

  nb_release(&buffer);
}

int main(void) {
  remove_decreases_count_correctly();
  remove_keeps_values_and_ordering();

  return 0;
}