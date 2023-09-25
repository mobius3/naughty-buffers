# Naughty Buffers 😈

Naughty Buffers is a C99 library that provides generic, type-unsafe, _"stretchy buffer"_ that can hold any type of data internally and allows you to access them later. It automatically grows when assigning, pushing or inserting new data blocks.

It also allows you to specify your own memory-management replacement functions for when you don't want to use standard ones (`malloc` and friends)

## Features

- Buffer automatically grows to accommodate for data
- Allows for custom memory functions set at runtime
- No external dependencies
- Clear, easy to use and fully documented API
- Unit tests 😁

## Examples

**Non-advanced usage**

```c
#include "naughty-buffers/buffer.h"
#include <stdio.h>

int read_int(struct nb_buffer * buffer, size_t index) {
  int * read_value = nb_at(&buffer, index);
  return *read_value;
}

int main(void) {
    struct nb_buffer buffer;
    nb_init(&buffer, sizeof(int));
    
    int value = 10;
    nb_push(&buffer, &value);
    printf("%d\n", read_int(&buffer, 0)); // prints 10
    
    value = 30;
    nb_insert(&buffer, 0, &value);
    printf("%d\n", read_int(&buffer, 0)); // prints 30
    
    
    value = 50;
    nb_assign(&buffer, 1, &value);
    printf("%d\n", read_int(&buffer, 1)); // prints 50
    printf("%d\n", read_int(&buffer, 0)); // prints 30
    
    return 0;
}
```

**With custom memory functions**

```c
#include "naughty-buffers/buffer.h"

void * my_alloc(size_t memory_size, void * context);
void my_release(void * ptr, void * context);
void * my_realloc(void * ptr, size_t memory_size, void * context);
void * my_copy(void * destination, const void * source, size_t size, void * context);
void * my_move(void * destination, const void * source, size_t size, void * context);
void * memory_context;


int main(void) {
  struct nb_buffer buffer;
  nb_init_advanced(
      &buffer,
      sizeof(int),
      my_alloc,
      my_realloc,
      my_release,
      my_copy,
      my_move,
      memory_contex
  );
  
  return 0;
}
```