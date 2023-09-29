#include "naughty-buffers/buffer.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

size_t alloc_call_count = 0;
size_t alloc_call_size = 0;

void * nb_test_alloc(size_t size, void * _) {
  (void)_;
  alloc_call_count++;
  alloc_call_size = size;
  return malloc(size);
}

size_t release_call_count = 0;

void nb_test_release(void * ptr, void * _) {
  (void)_;
  release_call_count++;
  free(ptr);
}

size_t realloc_call_count = 0;

void * nb_test_realloc(void * ptr, size_t size, void * _) {
  (void)_;
  realloc_call_count++;
  return realloc(ptr, size);
}

size_t copy_call_count = 0;
size_t copy_call_size = 0;

void * nb_test_copy(void * destination, const void * source, size_t size, void * _) {
  (void)_;
  copy_call_size = size;
  copy_call_count++;
  return memcpy(destination, source, size);
}

size_t move_call_count = 0;
void * nb_test_move(void * destination, const void * source, size_t size, void * _) {
  (void)_;
  move_call_count++;
  return memmove(destination, source, size);
}

void reset() {
  alloc_call_count = 0;
  alloc_call_size = 0;
  realloc_call_count = 0;
  release_call_count = 0;
  copy_call_count = 0;
  copy_call_size = 0;
  move_call_count = 0;
}

void memory_custom_memory_functions_and_context_are_initialized_properly() {
  reset();
  struct nb_buffer buffer;
  nb_init_advanced(
      &buffer, sizeof(uint32_t), nb_test_alloc, nb_test_realloc, nb_test_release, nb_test_copy, nb_test_move, &buffer
  );

  assert(buffer.alloc_fn == nb_test_alloc);
  assert(buffer.realloc_fn == nb_test_realloc);
  assert(buffer.free_fn == nb_test_release);
  assert(buffer.copy_fn == nb_test_copy);
  assert(buffer.memory_context == &buffer);

  nb_release(&buffer);
}

void memory_custom_memory_is_properly_called_with_push() {
  reset();

  struct nb_buffer buffer;
  size_t value = 1;
  nb_init_advanced(
      &buffer, sizeof(uint32_t), nb_test_alloc, nb_test_realloc, nb_test_release, nb_test_copy, nb_test_move, &buffer
  );
  assert(alloc_call_count == 1);
  assert(alloc_call_size == sizeof(uint32_t) * 2);

  realloc_call_count = 0;
  copy_call_count = 0;
  copy_call_size = 0;

  nb_push(&buffer, &value);
  assert(copy_call_count == 1);
  assert(copy_call_size == 4);

  nb_push(&buffer, &value);
  assert(copy_call_count == 2);
  assert(copy_call_size == 4);
  nb_push(&buffer, &value);
  assert(copy_call_count == 3);
  assert(copy_call_size == 4);
  nb_push(&buffer, &value);
  assert(copy_call_count == 4);
  assert(copy_call_size == 4);

  assert(realloc_call_count == 1);

  release_call_count = 0;
  nb_release(&buffer);
  assert(release_call_count == 1);
}

void memory_custom_memory_is_properly_called_with_assign() {
  reset();

  struct nb_buffer buffer;
  size_t value = 1;
  nb_init_advanced(
      &buffer, sizeof(uint32_t), nb_test_alloc, nb_test_realloc, nb_test_release, nb_test_copy, nb_test_move, &buffer
  );

  realloc_call_count = 0;
  copy_call_count = 0;
  copy_call_size = 0;

  nb_assign(&buffer, 5, &value);
  assert(copy_call_count == 1);
  assert(copy_call_size == 4);
  assert(realloc_call_count == 1);

  nb_assign(&buffer, 9, &value);
  assert(copy_call_count == 2);
  assert(copy_call_size == 4);
  assert(realloc_call_count == 2);

  release_call_count = 0;
  nb_release(&buffer);
  assert(release_call_count == 1);
}

void memory_custom_memory_is_properly_called_with_insert() {
  struct nb_buffer buffer;
  nb_init_advanced(
      &buffer, sizeof(uint32_t), nb_test_alloc, nb_test_realloc, nb_test_release, nb_test_copy, nb_test_move, &buffer
  );

  size_t value = 1;
  nb_push(&buffer, &value);

  value = 2;
  nb_push(&buffer, &value);

  reset();

  value = 0;
  nb_insert(&buffer, 0, &value);
  assert(copy_call_count == 1);
  assert(realloc_call_count == 1);
  assert(move_call_count == 1);

  value = 0;
  nb_insert(&buffer, 8, &value);
  assert(copy_call_count == 2);
  assert(realloc_call_count == 2);
  assert(move_call_count == 1);

  release_call_count = 0;
  nb_release(&buffer);
  assert(release_call_count == 1);
}

void memory_custom_memory_is_properly_called_with_remove() {
  struct nb_buffer buffer;
  nb_init_advanced(
      &buffer, sizeof(uint32_t), nb_test_alloc, nb_test_realloc, nb_test_release, nb_test_copy, nb_test_move, &buffer
  );

  size_t value = 1;
  nb_push(&buffer, &value);
  nb_push(&buffer, &value);
  nb_push(&buffer, &value);

  reset();

  nb_remove_front(&buffer);
  assert(move_call_count == 1);

  reset();

  nb_remove_at(&buffer, 0);
  assert(move_call_count == 1);

  nb_release(&buffer);
}

int main(void) {
  memory_custom_memory_functions_and_context_are_initialized_properly();
  memory_custom_memory_is_properly_called_with_push();
  memory_custom_memory_is_properly_called_with_assign();
  memory_custom_memory_is_properly_called_with_insert();
  memory_custom_memory_is_properly_called_with_remove();

  return 0;
}