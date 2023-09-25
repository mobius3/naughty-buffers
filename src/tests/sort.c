#include "criterion/criterion.h"
#include "naughty-buffers/buffer.h"

size_t compare_call_count = 0;
int int_compare(const void * ptr_a, const void * ptr_b) {
  compare_call_count++;
  int a = *((int *)ptr_a);
  int b = *((int *)ptr_b);
  return (a < b ? -1 : (b < a ? 1 : 0));
}

int read_int(const struct nb_buffer * buffer, size_t index) {
  int * read_value = nb_at(buffer, index);
  return *read_value;
}

Test(sort, sort_sorts) {
  struct nb_buffer buffer;
  int value = 0;
  nb_init(&buffer, sizeof(uint32_t));

  value = 0;
  nb_push(&buffer, &value);

  value = 8;
  nb_push(&buffer, &value);

  value = 3;
  nb_push(&buffer, &value);

  value = 1;
  nb_push(&buffer, &value);

  value = 2;
  nb_push(&buffer, &value);

  value = 5;
  nb_push(&buffer, &value);

  value = 4;
  nb_push(&buffer, &value);

  value = 9;
  nb_push(&buffer, &value);

  value = 7;
  nb_push(&buffer, &value);

  value = 6;
  nb_push(&buffer, &value);

  nb_sort(&buffer, int_compare);

  for (int i = 0; i < nb_block_count(&buffer); i++) { cr_expect_eq(read_int(&buffer, i), i); }

  nb_release(&buffer);
}

Test(sort, sort_calls_compare_fn) {
  compare_call_count = 0;
  struct nb_buffer buffer;
  int value = 0;
  nb_init(&buffer, sizeof(uint32_t));

  value = 2;
  nb_push(&buffer, &value);

  value = 1;
  nb_push(&buffer, &value);

  nb_sort(&buffer, int_compare);

  cr_assert(compare_call_count > 0);

  nb_release(&buffer);
}