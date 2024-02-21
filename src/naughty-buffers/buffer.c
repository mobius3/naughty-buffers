#include "naughty-buffers/buffer.h"
#include "memory.h"
#include <stdlib.h>

static inline void * ctx_alloc(struct nb_buffer * buffer, size_t size) {
  return buffer->memory_context->alloc_fn(size, buffer->memory_context->context);
}

static inline void * ctx_realloc(struct nb_buffer * buffer, void * ptr, size_t size) {
  return buffer->memory_context->realloc_fn(ptr, size, buffer->memory_context->context);
}

static inline void * ctx_copy(struct nb_buffer * buffer, void * destination, void * source, size_t size) {
  return buffer->memory_context->copy_fn(destination, source, size, buffer->memory_context->context);
}

static inline void * ctx_move(struct nb_buffer * buffer, void * destination, void * source, size_t size) {
  return buffer->memory_context->move_fn(destination, source, size, buffer->memory_context->context);
}

static inline void ctx_release(struct nb_buffer * buffer, void * ptr) {
  buffer->memory_context->free_fn(ptr, buffer->memory_context->context);
}

struct nb_buffer_memory_context default_memory_context = {
    .context = NULL,
    .free_fn = nb_memory_release,
    .copy_fn = nb_memory_copy,
    .realloc_fn = nb_memory_realloc,
    .alloc_fn = nb_memory_alloc,
    .move_fn = nb_memory_move
};

static inline size_t size_t_max(size_t a, size_t b) {
  if (a > b) return a;
  else return b;
}

void nb_init(struct nb_buffer * buffer, size_t block_size) {
  nb_init_advanced(buffer, block_size, &default_memory_context);
}

void nb_init_advanced(struct nb_buffer * buffer, size_t block_size, struct nb_buffer_memory_context * memory_context) {
  buffer->block_size = block_size;
  buffer->block_capacity = 2;
  buffer->block_count = 0;
  buffer->memory_context = memory_context;
  buffer->data = ctx_alloc(buffer, buffer->block_size * 2);
}

uint8_t nb_grow(struct nb_buffer * buffer, size_t desired_capacity) {
  size_t new_block_capacity = buffer->block_capacity * 2;
  while (new_block_capacity <= desired_capacity) new_block_capacity *= 2;
  void * new_data = ctx_realloc(buffer, buffer->data, buffer->block_size * new_block_capacity);
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
  ctx_copy(buffer, block_data, data, buffer->block_size);
  buffer->block_count += 1;
  return NB_PUSH_OK;
}

size_t nb_block_count(struct nb_buffer * buffer) { return buffer->block_count; }

void * nb_at(const struct nb_buffer * buffer, size_t index) {
  uint8_t * buffer_data = buffer->data;
  if (index >= buffer->block_count) return NULL;
  uint8_t * block_address = buffer_data + (buffer->block_size * index);
  return block_address;
}

void * nb_front(struct nb_buffer * buffer) { return nb_at(buffer, 0); }

void * nb_back(struct nb_buffer * buffer) { return nb_at(buffer, buffer->block_count - 1); }

void nb_release(struct nb_buffer * buffer) {
  ctx_release(buffer, buffer->data);

  buffer->block_size = 0;
  buffer->block_capacity = 0;
  buffer->block_count = 0;
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
  ctx_copy(buffer, block_data, data, buffer->block_size);
  if (index >= buffer->block_count) buffer->block_count = index + 1;
  return NB_ASSIGN_OK;
}

enum NB_INSERT_RESULT nb_insert(struct nb_buffer * buffer, size_t index, void * data) {
  size_t required_size = size_t_max(buffer->block_count + 1, index);
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
    ctx_move(buffer, dest, src, move_size);
  }
  ctx_copy(buffer, block_data, data, buffer->block_size);
  if (index >= buffer->block_count) buffer->block_count = index + 1;
  else buffer->block_count += 1;
  return NB_INSERT_OK;
}

void nb_remove_front(struct nb_buffer * buffer) { nb_remove_at(buffer, 0); }

void nb_remove_back(struct nb_buffer * buffer) { nb_remove_at(buffer, buffer->block_count - 1); }

void nb_remove_at(struct nb_buffer * buffer, size_t index) {
  if (index >= buffer->block_count) return;
  if (index == buffer->block_count - 1) {
    buffer->block_count--;
    return;
  }
  uint8_t * buffer_data = buffer->data;
  uint8_t * block_data = buffer_data + (index * buffer->block_size);
  uint8_t * dest = block_data;
  uint8_t * src = buffer_data + ((index + 1) * buffer->block_size);
  size_t move_count = buffer->block_count - index - 1;
  size_t move_size = move_count * buffer->block_size;
  ctx_move(buffer, dest, src, move_size);
  buffer->block_count--;
}

void nb_sort(struct nb_buffer * buffer, nb_compare_fn compare_fn) {
  qsort(buffer->data, buffer->block_count, buffer->block_size, compare_fn);
}

struct nb_buffer_iterator nb_iterator(struct nb_buffer * buffer) {
  return (struct nb_buffer_iterator) {
    .begin = (uint8_t *) buffer->data,
    .end = (uint8_t *) buffer->data + (buffer->block_count * buffer->block_size),
    .increment = buffer->block_size
  };
}
