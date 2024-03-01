#ifndef NAUGHTY_BUFFERS_MEMORY_H
#define NAUGHTY_BUFFERS_MEMORY_H

#include "naughty-buffers/naughty-buffers-export.h"

NAUGHTY_BUFFERS_NO_EXPORT void * nb_memory_alloc(size_t memory_size, const void * context);
NAUGHTY_BUFFERS_NO_EXPORT void nb_memory_release(void * ptr, const void * context);
NAUGHTY_BUFFERS_NO_EXPORT void * nb_memory_realloc(void * ptr, size_t memory_size, const void * context);
NAUGHTY_BUFFERS_NO_EXPORT void * nb_memory_copy(void * destination, const void * source, size_t size, const void * context);
NAUGHTY_BUFFERS_NO_EXPORT void * nb_memory_move(void * destination, const void * source, size_t size, const void * context);

#endif // NAUGHTY_BUFFERS_MEMORY_H
