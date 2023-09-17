#include "naughty-buffers/array-generator.h"
#include "criterion/criterion.h"

struct nb_test {
  long value;
};

NAUGHTY_BUFFERS_ARRAY_DECLARATION(test_array, struct nb_test)
NAUGHTY_BUFFERS_ARRAY_DEFINITION(test_array, struct nb_test)

Test(array_generator, init_works) {
  struct test_array test_array;
  test_array_init(&test_array);

  cr_assert(test_array.buffer.block_size == sizeof(struct nb_test));

  test_array_release(&test_array);
}

Test(array_generator, push_adds_correct_values) {
  struct test_array test_array;
  struct nb_test test = {.value = 10};
  struct nb_test * read_test;
  test_array_init(&test_array);

  test_array_push(&test_array, test);
  read_test = test_array_at(&test_array, 0);
  cr_assert(read_test->value == 10);

  test.value = 20;
  test_array_push(&test_array, test);
  read_test = test_array_at(&test_array, 0);
  cr_assert(read_test->value == 10);
  read_test = test_array_at(&test_array, 1);
  cr_assert(read_test->value == 20);

  cr_assert(test_array_count(&test_array) == 2);

  test_array_release(&test_array);
}

Test(array_generator, push_ptr_adds_correct_values_not_pointers) {
  struct test_array test_array;
  struct nb_test test = {.value = 10};
  struct nb_test * read_test;
  test_array_init(&test_array);

  test_array_push_ptr(&test_array, &test);
  read_test = test_array_at(&test_array, 0);
  cr_assert(read_test->value == 10);
  cr_assert(read_test != &test);

  test.value = 20;
  test_array_push_ptr(&test_array, &test);
  read_test = test_array_at(&test_array, 1);
  cr_assert(read_test->value == 20);
  cr_assert(read_test != &test);

  cr_assert(test_array_count(&test_array) == 2);

  test_array_release(&test_array);
}

Test(array_generator, assign_ptr_adds_correct_values_not_pointers) {
  struct test_array test_array;
  struct nb_test test = {.value = 10};
  struct nb_test * read_test;
  test_array_init(&test_array);

  test_array_assign_ptr(&test_array, 0, &test);
  read_test = test_array_at(&test_array, 0);
  cr_assert(read_test->value == 10);
  cr_assert(read_test != &test);

  test.value = 20;
  test_array_assign_ptr(&test_array, 0, &test);
  read_test = test_array_at(&test_array, 0);
  cr_assert(read_test->value == 20);
  cr_assert(read_test != &test);

  cr_assert(test_array_count(&test_array) == 1);

  test_array_release(&test_array);
}

Test(array_generator, assign_adds_correct_values) {
  struct test_array test_array;
  struct nb_test test = {.value = 10};
  struct nb_test * read_test;
  test_array_init(&test_array);

  test_array_assign(&test_array, 0, test);
  read_test = test_array_at(&test_array, 0);
  cr_assert(read_test->value == 10);

  test.value = 20;
  test_array_assign(&test_array, 0, test);
  read_test = test_array_at(&test_array, 0);
  cr_assert(read_test->value == 20);

  cr_assert(test_array_count(&test_array) == 1);

  test_array_release(&test_array);
}

Test(array_generator, insert_ptr_adds_correct_values_not_pointers) {
  struct test_array test_array;
  struct nb_test test = {.value = 10};
  struct nb_test * read_test;
  test_array_init(&test_array);

  test_array_insert_ptr(&test_array, 0, &test);
  read_test = test_array_at(&test_array, 0);
  cr_assert(read_test->value == 10);
  cr_assert(read_test != &test);

  test.value = 20;
  test_array_insert_ptr(&test_array, 0, &test);
  read_test = test_array_at(&test_array, 0);
  cr_assert(read_test->value == 20);
  cr_assert(read_test != &test);

  cr_assert(test_array_count(&test_array) == 2);

  test_array_release(&test_array);
}

Test(array_generator, insert_adds_correct_values) {
  struct test_array test_array;
  struct nb_test test = {.value = 10};
  struct nb_test * read_test;
  test_array_init(&test_array);

  test_array_insert(&test_array, 0, test);
  read_test = test_array_at(&test_array, 0);
  cr_assert(read_test->value == 10);

  test.value = 20;
  test_array_insert(&test_array, 0, test);
  read_test = test_array_at(&test_array, 0);
  cr_assert(read_test->value == 20);

  test_array_release(&test_array);
}

Test(array_generator, automatic_growth) {
  struct test_array test_array;
  struct nb_test test = {.value = 10};
  struct nb_test * read_test;
  test_array_init(&test_array);

  test_array_push(&test_array, test);
  test_array_push(&test_array, test);
  test_array_push(&test_array, test);
  test_array_push(&test_array, test);
  test_array_push(&test_array, test);
  test_array_push(&test_array, test);

  cr_assert(test_array_count(&test_array) == 6);
  cr_assert(test_array.buffer.block_capacity >= 6);

  test_array_release(&test_array);

  test_array_init(&test_array);

  test_array_assign(&test_array, 5, test);

  cr_assert(test_array_count(&test_array) == 6);
  cr_assert(test_array.buffer.block_capacity >= 6);

  test_array_release(&test_array);

  test_array_init(&test_array);

  test_array_insert(&test_array, 0, test);
  test_array_insert(&test_array, 0, test);
  test_array_insert(&test_array, 0, test);
  test_array_insert(&test_array, 0, test);
  test_array_insert(&test_array, 0, test);
  test_array_insert(&test_array, 0, test);

  cr_assert(test_array_count(&test_array) == 6);
  cr_assert(test_array.buffer.block_capacity >= 6);

  test_array_release(&test_array);
}