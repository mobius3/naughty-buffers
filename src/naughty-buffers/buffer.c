#include "naughty-buffers/buffer.h"
#include "memory.h"
#include <stdlib.h>

static size_t size_t_max(size_t a, size_t b) {
  if (a > b) return a;
  else return b;
}

void nb_init(struct nb_buffer * buffer, size_t block_size) {
  nb_init_advanced(
      buffer,
      block_size,
      nb_memory_alloc,
      nb_memory_realloc,
      nb_memory_release,
      nb_memory_copy,
      nb_memory_move,
      NULL
  );
}

void nb_init_advanced(
    struct nb_buffer * buffer,
    size_t block_size,
    nb_alloc_fn alloc_fn,
    nb_realloc_fn realloc_fn,
    nb_free_fn free_fn,
    nb_copy_fn copy_fn,
    nb_move_fn move_fn,
    void * memory_context
) {
  buffer->block_size = block_size;
  buffer->block_capacity = 2;
  buffer->block_count = 0;
  buffer->alloc_fn = alloc_fn;
  buffer->realloc_fn = realloc_fn;
  buffer->free_fn = free_fn;
  buffer->copy_fn = copy_fn;
  buffer->move_fn = move_fn;
  buffer->memory_context = memory_context;
  buffer->data = buffer->alloc_fn(buffer->block_size * 2, buffer->memory_context);
}

uint8_t nb_grow(struct nb_buffer * buffer, size_t desired_capacity) {
  size_t new_block_capacity = buffer->block_capacity * 2;
  while (new_block_capacity <= desired_capacity) new_block_capacity *= 2;
  void * new_data = buffer->realloc_fn(buffer->data, buffer->block_size * new_block_capacity, buffer->memory_context);
  if (!new_data) return 0;
  buffer->data = new_data;
  buffer->block_capacity = new_block_capacity;
  return 1;
}

enum NB_PUSH_RESULT nb_push(struct nb_buffer * buffer, void * data) {
  if (buffer->block_count >= buffer->block_capacity) {
    uint8_t grow_success = nb_grow(buffer, buffer->block_count + 1);
    if (!grow_success) return NB_PUSH_OUT_OF_MEMORY;
  }

  uint8_t * buffer_data = buffer->data;
  void * block_data = buffer_data + (buffer->block_count * buffer->block_size);
  buffer->copy_fn(block_data, data, buffer->block_size, buffer->memory_context);
  buffer->block_count += 1;
  return NB_PUSH_OK;
}

size_t nb_block_count(struct nb_buffer * buffer) {
  return buffer->block_count;
}

void * nb_at(struct nb_buffer * buffer, size_t index) {
  uint8_t * buffer_data = buffer->data;
  if (index >= buffer->block_count) return NULL;
  uint8_t * block_address = buffer_data + (buffer->block_size * index);
  return block_address;
}

void * nb_front(struct nb_buffer * buffer) {
  return nb_at(buffer, 0);
}

void * nb_back(struct nb_buffer * buffer) {
  return nb_at(buffer, buffer->block_count -1);
}

void nb_release(struct nb_buffer * buffer) {
  buffer->free_fn(buffer->data, buffer->memory_context);

  buffer->block_size = 0;
  buffer->block_capacity = 0;
  buffer->block_count = 0;
  buffer->alloc_fn = NULL;
  buffer->realloc_fn = NULL;
  buffer->free_fn = NULL;
  buffer->copy_fn = NULL;
  buffer->memory_context = NULL;
  buffer->data = NULL;
}

enum NB_ASSIGN_RESULT nb_assign(struct nb_buffer * buffer, size_t index, void * data) {
  if (index >= buffer->block_capacity) {
    uint8_t grow_success = nb_grow(buffer, index + 1);
    if (!grow_success) return NB_ASSIGN_OUT_OF_MEMORY;
  }
  uint8_t * buffer_data = buffer->data;
  void * block_data = buffer_data + (index * buffer->block_size);
  buffer->copy_fn(block_data, data, buffer->block_size, buffer->memory_context);
  if (index >= buffer->block_count) buffer->block_count = index +1;
  return NB_ASSIGN_OK;
}

enum NB_INSERT_RESULT nb_insert(struct nb_buffer * buffer, size_t index, void * data) {
  size_t required_size = size_t_max(buffer->block_count +1, index);
  if (required_size >= buffer->block_capacity) {
    uint8_t grow_success = nb_grow(buffer, required_size);
    if (!grow_success) return NB_INSERT_OUT_OF_MEMORY;
  }
  uint8_t * buffer_data = buffer->data;
  uint8_t * block_data = buffer_data + (index * buffer->block_size);
  if (index < buffer->block_count) {
    uint8_t * dest = buffer_data + ((index + 1) * buffer->block_size);
    uint8_t * src = block_data;
    size_t move_size = (buffer->block_count - index) * buffer->block_size;
    buffer->move_fn(dest, src, move_size, buffer->memory_context);
  }
  buffer->copy_fn(block_data, data, buffer->block_size, buffer->memory_context);
  if (index >= buffer->block_count) buffer->block_count = index +1;
  else buffer->block_count += 1;
  return NB_INSERT_OK;
}

void nb_remove_front(struct nb_buffer * buffer) {
  nb_remove_at(buffer, 0);
}

void nb_remove_back(struct nb_buffer * buffer) {
  nb_remove_at(buffer, buffer->block_count -1);
}

void nb_remove_at(struct nb_buffer * buffer, size_t index) {
  if (index >= buffer->block_count) return;
  if (index == buffer->block_count -1) {
    buffer->block_count--;
    return;
  }
  uint8_t * buffer_data = buffer->data;
  uint8_t * block_data = buffer_data + (index * buffer->block_size);
  uint8_t * dest = block_data;
  uint8_t * src = buffer_data + ((index + 1) * buffer->block_size);
  size_t move_count = buffer->block_count - index - 1;
  size_t move_size = move_count * buffer->block_size;
  buffer->move_fn(dest, src, move_size, buffer->memory_context);
  buffer->block_count--;
}

void nb_sort(struct nb_buffer * buffer, nb_compare_fn compare_fn) {
  qsort(buffer, buffer->block_count, buffer->block_size, compare_fn);
}
