#include <stdlib.h>
#include <string.h>

#include "memory.h"

void * nb_memory_alloc(const size_t memory_size, const void * context) {
  (void) context;
  return malloc(memory_size);
}

void nb_memory_release(void * ptr, const void * context) {
  (void) context;
  free(ptr);
}

void * nb_memory_realloc(void * ptr, const size_t memory_size, const void * context) {
  (void) context;
  return realloc(ptr, memory_size);
}

void * nb_memory_copy(void * destination, const void * source, const size_t size, const void * context) {
  (void) context;
  return memcpy(destination, source, size);
}

void * nb_memory_move(void * destination, const void * source, const size_t size, const void * context) {
  (void) context;
  return memmove(destination, source, size);
}
