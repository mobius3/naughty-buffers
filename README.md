# Naughty Buffers 😈

Naughty Buffers is a C99 library that provides generic, type-unsafe, _"stretchy buffer"_ that can hold any type of data
internally and allows you to access them later. It automatically grows when assigning, pushing or inserting new data
blocks.

It also allows you to specify your own memory-management replacement functions for when you don't want to use standard
ones (`malloc` and friends)

## Features

- Buffer automatically grows to accommodate for data
- Allows for custom memory functions set at runtime
- Macros to generate type-safe* wrappers
- No external dependencies
- Clear, easy to use and fully documented API
- Unit tested 😁
- _Valgrind_: All heap blocks were freed -- no leaks are possible

Wrappers are not really type-safe at implementation level, but they allow you to use a type-safe API.

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

Check [the tests folder](/src/tests) and [the examples folder](/src/examples) for more complex examples

## Integrating with your code

### Using pre-built releases

Download a pre-built release package suitable for your platform and
uncompress it somewhere you'll remember later (usually where you put
other development libraries). Let's assume you uncompressed naughty-buffers to
`/home/me/libs/naughty-buffers`.

**Using CMake**: If you're using CMake, specify the variable
`CMAKE_PREFIX_PATH` to point to it before running CMake in your project:

```shell script
cmake -DCMAKE_PREFIX_PATH=/home/me/libs
```

Then you can use `find_package(naughty-buffers)` and
`target_link_libraries(<your-executable> PUBLIC naughty-buffers)` to link
`<your-executable>` against naughty-buffers.

**Not using CMake**: Assuming you uncompressed naughty-buffers to the same path as
above, you should configure your build system to look for include files
inside `/home/me/libs/naughty-buffers/include` and to look for shared objects to
link against inside `/home/me/libs/naughty-buffers/lib`. In case of Windows, you
should also point your linker to `naughty-buffers/bin` as well.

### Using a source release and CMake

Uncompress naughty-buffers in a folder inside your project (e.g,
`your-project/third-party/naughty-buffers`) and then use
`add_subdirectory(third-party/naughty-buffers EXCLUDE_FROM_ALL)` to add the
library target. Then you can use `find_package(naughty-buffers)` and
`target_link_libraries(<your-executable> PUBLIC naughty-buffers)` to link
`<your-executable>` against naughty-buffers.

Compiling the `.c` files directly in your project is not recommended nor
supported.

## Compile from source

You'll need CMake installed and in your path and also capable of finding
you compiler and linker. Then, after checking out this repository:

```shell script
mkdir build/
cd build/
cmake ..
cmake --build .
```

## Examples

Examples for C and C++ are in the [src/examples](src/examples) folder. To build them, when running cmake as in [Compile from source](#compile-from-source), add the following variable:

```shell script
cmake .. -DBUILD_EXAMPLES=1
```

You will need SDL2 available and CMake needs to be able to find it.

## Documentation

See [here](https://mobius3.github.io/naughty-buffers)
