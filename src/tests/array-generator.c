#include "naughty-buffers/array-generator.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct nb_test {
  long value;
};

int nb_test_compare(const void * ptr_a, const void * ptr_b) {
  struct nb_test * test_a = (struct nb_test *)ptr_a;
  struct nb_test * test_b = (struct nb_test *)ptr_b;
  long a = test_a->value;
  long b = test_b->value;
  return (a < b ? -1 : (b < a ? 1 : 0));
}

NAUGHTY_BUFFERS_ARRAY_DECLARATION(test_array, struct nb_test)
NAUGHTY_BUFFERS_ARRAY_DEFINITION(test_array, struct nb_test)

#define assert_eq(a, b) assert((a) == (b))

void * nb_test_alloc(size_t size, void * _) {
  (void)_;
  return malloc(size);
}

void nb_test_release(void * ptr, void * _) {
  (void)_;
  free(ptr);
}

void * nb_test_realloc(void * ptr, size_t size, void * _) {
  (void)_;
  return realloc(ptr, size);
}

void * nb_test_copy(void * destination, const void * source, size_t size, void * _) {
  (void)_;
  return memcpy(destination, source, size);
}

void * nb_test_move(void * destination, const void * source, size_t size, void * _) {
  (void)_;
  return memmove(destination, source, size);
}

struct nb_buffer_memory_context ctx = {
    .move_fn = nb_test_move,
    .alloc_fn = nb_test_alloc,
    .realloc_fn = nb_test_realloc,
    .copy_fn = nb_test_copy,
    .free_fn = nb_test_release
};

void array_generator_init_works() {
  struct test_array test_array;
  test_array_init(&test_array);

  assert(test_array.buffer.block_size == sizeof(struct nb_test));

  test_array_release(&test_array);
}

void array_generator_init_advanced_works() {
  struct test_array test_array;
  test_array_init_advanced(&test_array, &ctx);

  assert(test_array.buffer.memory_context == &ctx);

  assert(test_array.buffer.block_size == sizeof(struct nb_test));

  test_array_release(&test_array);
}

void array_generator_push_adds_correct_values() {
  struct test_array test_array;
  struct nb_test test = {.value = 10};
  struct nb_test * read_test;
  test_array_init(&test_array);

  test_array_push(&test_array, test);
  read_test = test_array_at_ptr(&test_array, 0);
  assert(read_test->value == 10);

  test.value = 20;
  test_array_push(&test_array, test);
  read_test = test_array_at_ptr(&test_array, 0);
  assert(read_test->value == 10);
  read_test = test_array_at_ptr(&test_array, 1);
  assert(read_test->value == 20);

  assert(test_array_count(&test_array) == 2);

  test_array_release(&test_array);
}

void array_generator_push_ptr_adds_correct_values_not_pointers() {
  struct test_array test_array;
  struct nb_test test = {.value = 10};
  struct nb_test * read_test;
  test_array_init(&test_array);

  test_array_push_ptr(&test_array, &test);
  read_test = test_array_at_ptr(&test_array, 0);
  assert(read_test->value == 10);
  assert(read_test != &test);

  test.value = 20;
  test_array_push_ptr(&test_array, &test);
  read_test = test_array_at_ptr(&test_array, 1);
  assert(read_test->value == 20);
  assert(read_test != &test);

  assert(test_array_count(&test_array) == 2);

  test_array_release(&test_array);
}

void array_generator_assign_ptr_adds_correct_values_not_pointers() {
  struct test_array test_array;
  struct nb_test test = {.value = 10};
  struct nb_test * read_test;
  test_array_init(&test_array);

  test_array_assign_ptr(&test_array, 0, &test);
  read_test = test_array_at_ptr(&test_array, 0);
  assert(read_test->value == 10);
  assert(read_test != &test);

  test.value = 20;
  test_array_assign_ptr(&test_array, 0, &test);
  read_test = test_array_at_ptr(&test_array, 0);
  assert(read_test->value == 20);
  assert(read_test != &test);

  assert(test_array_count(&test_array) == 1);

  test_array_release(&test_array);
}

void array_generator_assign_adds_correct_values() {
  struct test_array test_array;
  struct nb_test test = {.value = 10};
  struct nb_test * read_test;
  test_array_init(&test_array);

  test_array_assign(&test_array, 0, test);
  read_test = test_array_at_ptr(&test_array, 0);
  assert(read_test->value == 10);

  test.value = 20;
  test_array_assign(&test_array, 0, test);
  read_test = test_array_at_ptr(&test_array, 0);
  assert(read_test->value == 20);

  assert(test_array_count(&test_array) == 1);

  test_array_release(&test_array);
}

void array_generator_insert_ptr_adds_correct_values_not_pointers() {
  struct test_array test_array;
  struct nb_test test = {.value = 10};
  struct nb_test * read_test;
  test_array_init(&test_array);

  test_array_insert_ptr(&test_array, 0, &test);
  read_test = test_array_at_ptr(&test_array, 0);
  assert(read_test->value == 10);
  assert(read_test != &test);

  test.value = 20;
  test_array_insert_ptr(&test_array, 0, &test);
  read_test = test_array_at_ptr(&test_array, 0);
  assert(read_test->value == 20);
  assert(read_test != &test);

  assert(test_array_count(&test_array) == 2);

  test_array_release(&test_array);
}

void array_generator_insert_adds_correct_values() {
  struct test_array test_array;
  struct nb_test test = {.value = 10};
  struct nb_test * read_test;
  test_array_init(&test_array);

  test_array_insert(&test_array, 0, test);
  read_test = test_array_at_ptr(&test_array, 0);
  assert(read_test->value == 10);

  test.value = 20;
  test_array_insert(&test_array, 0, test);
  read_test = test_array_at_ptr(&test_array, 0);
  assert(read_test->value == 20);

  test_array_release(&test_array);
}

void array_generator_automatic_growth() {
  struct test_array test_array;
  struct nb_test test = {.value = 10};
  test_array_init(&test_array);

  test_array_push(&test_array, test);
  test_array_push(&test_array, test);
  test_array_push(&test_array, test);
  test_array_push(&test_array, test);
  test_array_push(&test_array, test);
  test_array_push(&test_array, test);

  assert(test_array_count(&test_array) == 6);
  assert(test_array.buffer.block_capacity >= 6);

  test_array_release(&test_array);

  test_array_init(&test_array);

  test_array_assign(&test_array, 5, test);

  assert(test_array_count(&test_array) == 6);
  assert(test_array.buffer.block_capacity >= 6);

  test_array_release(&test_array);

  test_array_init(&test_array);

  test_array_insert(&test_array, 0, test);
  test_array_insert(&test_array, 0, test);
  test_array_insert(&test_array, 0, test);
  test_array_insert(&test_array, 0, test);
  test_array_insert(&test_array, 0, test);
  test_array_insert(&test_array, 0, test);

  assert(test_array_count(&test_array) == 6);
  assert(test_array.buffer.block_capacity >= 6);

  test_array_release(&test_array);
}

void array_generator_sort_sorts() {
  struct test_array test_array;
  struct nb_test test = {.value = 0};
  test_array_init(&test_array);

  test.value = 0;
  test_array_push_ptr(&test_array, &test);

  test.value = 8;
  test_array_push_ptr(&test_array, &test);

  test.value = 3;
  test_array_push_ptr(&test_array, &test);

  test.value = 1;
  test_array_push_ptr(&test_array, &test);

  test.value = 2;
  test_array_push_ptr(&test_array, &test);

  test.value = 5;
  test_array_push_ptr(&test_array, &test);

  test.value = 4;
  test_array_push_ptr(&test_array, &test);

  test.value = 9;
  test_array_push_ptr(&test_array, &test);

  test.value = 7;
  test_array_push_ptr(&test_array, &test);

  test.value = 6;
  test_array_push_ptr(&test_array, &test);

  test_array_sort(&test_array, nb_test_compare);

  for (int i = 0; i < test_array_count(&test_array); i++) { assert_eq(test_array_at_ptr(&test_array, i)->value, i); }

  test_array_release(&test_array);
}

void array_generator_remove_decreases_count_correctly() {
  struct test_array test_array;
  struct nb_test test = {.value = 0};
  test_array_init(&test_array);
  assert(test_array_count(&test_array) == 0);
  test_array_push_ptr(&test_array, &test);
  test_array_push_ptr(&test_array, &test);
  test_array_push_ptr(&test_array, &test);
  test_array_push_ptr(&test_array, &test);
  test_array_push_ptr(&test_array, &test);
  test_array_push_ptr(&test_array, &test);

  test_array_remove_back(&test_array);
  assert(test_array_count(&test_array) == 5);

  test_array_remove_front(&test_array);
  assert(test_array_count(&test_array) == 4);

  test_array_remove_at(&test_array, 2);
  assert(test_array_count(&test_array) == 3);

  test_array_release(&test_array);
}

void array_generator_remove_keeps_values_and_ordering() {
  struct test_array test_array;
  struct nb_test test;

  test_array_init(&test_array);

  for (uint32_t i = 0; i < 10; i++) {
    test.value = i;
    test_array_push_ptr(&test_array, &test);
  }

  test_array_remove_front(&test_array);
  assert_eq(test_array_at_ptr(&test_array, 0)->value, 1);
  assert_eq(test_array_at_ptr(&test_array, 1)->value, 2);
  assert_eq(test_array_at_ptr(&test_array, 2)->value, 3);
  assert_eq(test_array_at_ptr(&test_array, 3)->value, 4);
  assert_eq(test_array_at_ptr(&test_array, 4)->value, 5);
  assert_eq(test_array_at_ptr(&test_array, 5)->value, 6);
  assert_eq(test_array_at_ptr(&test_array, 6)->value, 7);
  assert_eq(test_array_at_ptr(&test_array, 7)->value, 8);
  assert_eq(test_array_at_ptr(&test_array, 8)->value, 9);

  test_array_remove_back(&test_array);
  assert_eq(test_array_at_ptr(&test_array, 0)->value, 1);
  assert_eq(test_array_at_ptr(&test_array, 1)->value, 2);
  assert_eq(test_array_at_ptr(&test_array, 2)->value, 3);
  assert_eq(test_array_at_ptr(&test_array, 3)->value, 4);
  assert_eq(test_array_at_ptr(&test_array, 4)->value, 5);
  assert_eq(test_array_at_ptr(&test_array, 5)->value, 6);
  assert_eq(test_array_at_ptr(&test_array, 6)->value, 7);
  assert_eq(test_array_at_ptr(&test_array, 7)->value, 8);

  test_array_remove_at(&test_array, 3);
  assert_eq(test_array_at_ptr(&test_array, 0)->value, 1);
  assert_eq(test_array_at_ptr(&test_array, 1)->value, 2);
  assert_eq(test_array_at_ptr(&test_array, 2)->value, 3);
  assert_eq(test_array_at_ptr(&test_array, 3)->value, 5);
  assert_eq(test_array_at_ptr(&test_array, 4)->value, 6);
  assert_eq(test_array_at_ptr(&test_array, 5)->value, 7);
  assert_eq(test_array_at_ptr(&test_array, 6)->value, 8);

  test_array_remove_at(&test_array, 4);
  assert_eq(test_array_at_ptr(&test_array, 0)->value, 1);
  assert_eq(test_array_at_ptr(&test_array, 1)->value, 2);
  assert_eq(test_array_at_ptr(&test_array, 2)->value, 3);
  assert_eq(test_array_at_ptr(&test_array, 3)->value, 5);
  assert_eq(test_array_at_ptr(&test_array, 4)->value, 7);
  assert_eq(test_array_at_ptr(&test_array, 5)->value, 8);

  test_array_release(&test_array);
}

int main(void) {
  array_generator_init_works();
  array_generator_init_advanced_works();
  array_generator_push_adds_correct_values();
  array_generator_push_ptr_adds_correct_values_not_pointers();
  array_generator_assign_ptr_adds_correct_values_not_pointers();
  array_generator_assign_adds_correct_values();
  array_generator_insert_ptr_adds_correct_values_not_pointers();
  array_generator_insert_adds_correct_values();
  array_generator_automatic_growth();
  array_generator_sort_sorts();
  array_generator_remove_decreases_count_correctly();
  array_generator_remove_keeps_values_and_ordering();

  return 0;
}
