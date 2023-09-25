#include "naughty-buffers/array-generator.h"
#include "criterion/criterion.h"

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
  read_test = test_array_at_ptr(&test_array, 0);
  cr_assert(read_test->value == 10);

  test.value = 20;
  test_array_push(&test_array, test);
  read_test = test_array_at_ptr(&test_array, 0);
  cr_assert(read_test->value == 10);
  read_test = test_array_at_ptr(&test_array, 1);
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
  read_test = test_array_at_ptr(&test_array, 0);
  cr_assert(read_test->value == 10);
  cr_assert(read_test != &test);

  test.value = 20;
  test_array_push_ptr(&test_array, &test);
  read_test = test_array_at_ptr(&test_array, 1);
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
  read_test = test_array_at_ptr(&test_array, 0);
  cr_assert(read_test->value == 10);
  cr_assert(read_test != &test);

  test.value = 20;
  test_array_assign_ptr(&test_array, 0, &test);
  read_test = test_array_at_ptr(&test_array, 0);
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
  read_test = test_array_at_ptr(&test_array, 0);
  cr_assert(read_test->value == 10);

  test.value = 20;
  test_array_assign(&test_array, 0, test);
  read_test = test_array_at_ptr(&test_array, 0);
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
  read_test = test_array_at_ptr(&test_array, 0);
  cr_assert(read_test->value == 10);
  cr_assert(read_test != &test);

  test.value = 20;
  test_array_insert_ptr(&test_array, 0, &test);
  read_test = test_array_at_ptr(&test_array, 0);
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
  read_test = test_array_at_ptr(&test_array, 0);
  cr_assert(read_test->value == 10);

  test.value = 20;
  test_array_insert(&test_array, 0, test);
  read_test = test_array_at_ptr(&test_array, 0);
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

Test(array_generator, sort_sorts) {
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

  for (int i = 0; i < test_array_count(&test_array); i++) {
    cr_expect_eq(test_array_at_ptr(&test_array, i)->value, i);
  }

  test_array_release(&test_array);
}

Test(array_generator, decreases_count_correctly) {
  struct test_array test_array;
  struct nb_test test = { .value = 0 };
  test_array_init(&test_array);
  cr_assert(test_array_count(&test_array) == 0);
  test_array_push_ptr(&test_array, &test);
  test_array_push_ptr(&test_array, &test);
  test_array_push_ptr(&test_array, &test);
  test_array_push_ptr(&test_array, &test);
  test_array_push_ptr(&test_array, &test);
  test_array_push_ptr(&test_array, &test);

  test_array_remove_back(&test_array);
  cr_assert(test_array_count(&test_array) == 5);

  test_array_remove_front(&test_array);
  cr_assert(test_array_count(&test_array) == 4);

  test_array_remove_at(&test_array, 2);
  cr_assert(test_array_count(&test_array) == 3);

  test_array_release(&test_array);
}

Test(array_generator, keeps_values_and_ordering) {
  struct test_array test_array;
  struct nb_test test;

  test_array_init(&test_array);

  for (uint32_t i = 0; i < 10; i++) {
    test.value = i;
    test_array_push_ptr(&test_array, &test);
  }

  test_array_remove_front(&test_array);
  cr_expect_eq(test_array_at_ptr(&test_array, 0)->value, 1);
  cr_expect_eq(test_array_at_ptr(&test_array, 1)->value, 2);
  cr_expect_eq(test_array_at_ptr(&test_array, 2)->value, 3);
  cr_expect_eq(test_array_at_ptr(&test_array, 3)->value, 4);
  cr_expect_eq(test_array_at_ptr(&test_array, 4)->value, 5);
  cr_expect_eq(test_array_at_ptr(&test_array, 5)->value, 6);
  cr_expect_eq(test_array_at_ptr(&test_array, 6)->value, 7);
  cr_expect_eq(test_array_at_ptr(&test_array, 7)->value, 8);
  cr_expect_eq(test_array_at_ptr(&test_array, 8)->value, 9);

  test_array_remove_back(&test_array);
  cr_expect_eq(test_array_at_ptr(&test_array, 0)->value, 1);
  cr_expect_eq(test_array_at_ptr(&test_array, 1)->value, 2);
  cr_expect_eq(test_array_at_ptr(&test_array, 2)->value, 3);
  cr_expect_eq(test_array_at_ptr(&test_array, 3)->value, 4);
  cr_expect_eq(test_array_at_ptr(&test_array, 4)->value, 5);
  cr_expect_eq(test_array_at_ptr(&test_array, 5)->value, 6);
  cr_expect_eq(test_array_at_ptr(&test_array, 6)->value, 7);
  cr_expect_eq(test_array_at_ptr(&test_array, 7)->value, 8);

  test_array_remove_at(&test_array, 3);
  cr_expect_eq(test_array_at_ptr(&test_array, 0)->value, 1);
  cr_expect_eq(test_array_at_ptr(&test_array, 1)->value, 2);
  cr_expect_eq(test_array_at_ptr(&test_array, 2)->value, 3);
  cr_expect_eq(test_array_at_ptr(&test_array, 3)->value, 5);
  cr_expect_eq(test_array_at_ptr(&test_array, 4)->value, 6);
  cr_expect_eq(test_array_at_ptr(&test_array, 5)->value, 7);
  cr_expect_eq(test_array_at_ptr(&test_array, 6)->value, 8);

  test_array_remove_at(&test_array, 4);
  cr_expect_eq(test_array_at_ptr(&test_array, 0)->value, 1);
  cr_expect_eq(test_array_at_ptr(&test_array, 1)->value, 2);
  cr_expect_eq(test_array_at_ptr(&test_array, 2)->value, 3);
  cr_expect_eq(test_array_at_ptr(&test_array, 3)->value, 5);
  cr_expect_eq(test_array_at_ptr(&test_array, 4)->value, 7);
  cr_expect_eq(test_array_at_ptr(&test_array, 5)->value, 8);

  test_array_release(&test_array);
}