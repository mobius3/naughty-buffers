#include <stdlib.h>
#include <string.h>

#include "memory.h"

void * nb_memory_alloc(size_t size, void * _) {
  (void)_;
  return malloc(size);
}

void nb_memory_release(void * ptr, void * _) {
  (void)_;
  free(ptr);
}

void * nb_memory_realloc(void * ptr, size_t size, void * _) {
  (void)_;
  return realloc(ptr, size);
}

void * nb_memory_copy(void * destination, const void * source, size_t size, void * _) {
  (void)_;
  return memcpy(destination, source, size);
}

void * nb_memory_move(void * destination, const void * source, size_t size, void * _) {
  (void)_;
  return memmove(destination, source, size);
}
