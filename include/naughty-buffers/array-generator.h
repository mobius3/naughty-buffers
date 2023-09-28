#ifndef NAUGHTY_BUFFERS_ARRAY_GENERATOR_H
#define NAUGHTY_BUFFERS_ARRAY_GENERATOR_H

/**
 * @file array-generator.h
 * Provides macros to generate a group of typed functions to handle in a type-safe fashion all kind of data types.
 * @defgroup array-generator Array Generator
 * The macros contained here can be used to generate a new struct and a group of functions that provide a type-safe
 * API to handle all kind of types. The functions are equivalent to using the ones defined in buffer.h but they
 * are declared to hold a single type of data. Everything declared and defined by these macros are backed by `nb_buffer`
 * structs and functions.
 *
 * **Usage**
 *
 * The recommended way of using these macros are as follows:
 * - In a single header file, with include guards, place the `NAUGHTY_BUFFERS_ARRAY_DECLARATION` macro:
 * @code
 * // file: int-array.h
 * #ifndef INT_ARRAY_H
 * #define INT_ARRAY_H
 *
 * NAUGHTY_BUFFERS_ARRAY_DECLARATION(int_array, int)
 *
 * #endif
 * @endcode
 * - In a single `c` file, include the `int-array.h` file and place the `NAUGHTY_BUFFERS_ARRAY_DEFINITION` macro:
 * @code
 * // file: int-array.c
 * #include "int-array.h"
 *
 * NAUGHTY_BUFFERS_ARRAY_DEFINITION(int_array, int)
 *
 * @endcode
 * - Compile and link int-array.c with your final executable or library.
 *
 * **Generated code:**
 *
 * For array type `T_array` and data type `T`, the following is generated:
 *
 * - `struct T_array { struct nb_buffer buffer; }`
 * - `void T_init(struct T_array *)`, analogous to ::nb_init
 * - `void T_init_advanced(struct T_array *, nb_alloc_fn, nb_realloc_fn, nb_free_fn, nb_copy_fn, nb_move_fn, void *)`,
 * analogous to ::nb_init_advanced;
 * - `void T_push(struct T_array *, const T)`, analogous to ::nb_push but accepting a copy of the data of the argument.
 * Most useful when the block size is at most `sizeof(ptrdiff_t)`.
 * - `void T_push_ptr(struct T_array *, const T *)`, analogous to ::nb_push and useful when the block size is greater
 * than `sizeof(ptrdiff_t)`.
 * - `void T_assign(struct T_array *, size_t, const T)`, analogous to ::nb_assign but accepting a copy of the item as an
 * argument. Most useful when the block size is at most `sizeof(ptrdiff_t)`.
 * - `void T_assign_ptr(struct T_array *, size_t, const T *)`, analogous to ::nb_assign and useful when the block
 * size is greater than `sizeof(ptrdiff_t)`.
 * - `void T_insert(struct T_array *, size_t, const T)`, analogous to ::nb_insert but accepting a copy of the item as an
 * argument. Most useful when the block size is at most `sizeof(ptrdiff_t)`.
 * - `void T_insert_ptr(struct T_array *, size_t, const T *)`, analogous to ::nb_insert and useful when the block
 * size is greater than `sizeof(ptrdiff_t)`.
 * - `size_t T_count(struct T_array *)`, analogous to ::nb_block_count
 * - `T T_at(struct T_array *, size_t)`, analogous to ::nb_at but returning a copy of the block in the array. Most
 * useful when the block size is at most `sizeof(ptrdiff_t)`.
 * - `T * T_at(struct T_array *, size_t)`, analogous to ::nb_at but returning a pointer to the data directly in the
 * array. Useful when the block size is greater than `sizeof(ptrdiff_t)`.
 * - `T T_front(struct T_array *)`, analogous to ::nb_front but returning a copy of the block in the array. Most
 * useful when the block size is at most `sizeof(ptrdiff_t)`.
 * - `T * T_front_ptr(struct T_array *)`, analogous to ::nb_front but returning a pointer to the data directly in the
 * array. Useful when the block size is greater than `sizeof(ptrdiff_t)`.
 * - `T T_back(struct T_array *)`, analogous to ::nb_back but returning a copy of the block in the array. Most
 * useful when the block size is at most `sizeof(ptrdiff_t)`.
 * - `T * T_back_ptr(struct T_array *)`, analogous to ::nb_back but returning a pointer to the data directly in the
 * array. Useful when the block size is greater than `sizeof(ptrdiff_t)`.
 * - `void T_remove_at(struct T *, size_t)`, analogous to ::nb_remove_at.
 * - `void T_remove_back(struct T *)`, analogous to ::nb_remove_back
 * - `void T_remove_front(struct T *)`, analogous to ::nb_remove_front
 * - `void T_sort(struct T *, nb_compare_fn)`, analogous to ::nb_sort
 * - `void T_release(struct T *)`, analogous to ::nb_release
 */

#include "naughty-buffers/buffer.h"

/**
 * @brief Declares a struct named using `__NB_ARRAY_TYPE__` to handle blocks of type `__NB_ARRAY_BLOCK_TYPE__`.
 * @ingroup array-generator
 */
#define NAUGHTY_BUFFERS_ARRAY_DECLARATION(__NB_ARRAY_TYPE__, __NB_ARRAY_BLOCK_TYPE__)                                  \
  struct __NB_ARRAY_TYPE__ {                                                                                           \
    struct nb_buffer buffer;                                                                                           \
  };                                                                                                                   \
  void __NB_ARRAY_TYPE__##_init(struct __NB_ARRAY_TYPE__ * array);                                                     \
  void __NB_ARRAY_TYPE__##_init_advanced(                                                                              \
      struct __NB_ARRAY_TYPE__ * array, nb_alloc_fn alloc_fn, nb_realloc_fn realloc_fn, nb_free_fn free_fn,            \
      nb_copy_fn copy_fn, nb_move_fn move_fn, void * memory_context                                                    \
  );                                                                                                                   \
  enum NB_PUSH_RESULT __NB_ARRAY_TYPE__##_push(struct __NB_ARRAY_TYPE__ * array, const __NB_ARRAY_BLOCK_TYPE__ item);  \
  enum NB_PUSH_RESULT __NB_ARRAY_TYPE__##_push_ptr(                                                                    \
      struct __NB_ARRAY_TYPE__ * array, const __NB_ARRAY_BLOCK_TYPE__ * item                                           \
  );                                                                                                                   \
  enum NB_ASSIGN_RESULT __NB_ARRAY_TYPE__##_assign(                                                                    \
      struct __NB_ARRAY_TYPE__ * array, size_t index, const __NB_ARRAY_BLOCK_TYPE__ item                               \
  );                                                                                                                   \
  enum NB_ASSIGN_RESULT __NB_ARRAY_TYPE__##_assign_ptr(                                                                \
      struct __NB_ARRAY_TYPE__ * array, size_t index, const __NB_ARRAY_BLOCK_TYPE__ * item                             \
  );                                                                                                                   \
  enum NB_INSERT_RESULT __NB_ARRAY_TYPE__##_insert(                                                                    \
      struct __NB_ARRAY_TYPE__ * array, size_t index, const __NB_ARRAY_BLOCK_TYPE__ item                               \
  );                                                                                                                   \
  enum NB_INSERT_RESULT __NB_ARRAY_TYPE__##_insert_ptr(                                                                \
      struct __NB_ARRAY_TYPE__ * array, size_t index, const __NB_ARRAY_BLOCK_TYPE__ * item                             \
  );                                                                                                                   \
  size_t __NB_ARRAY_TYPE__##_count(struct __NB_ARRAY_TYPE__ * array);                                                  \
  __NB_ARRAY_BLOCK_TYPE__ __NB_ARRAY_TYPE__##_at(struct __NB_ARRAY_TYPE__ * buffer, size_t index);                     \
  __NB_ARRAY_BLOCK_TYPE__ * __NB_ARRAY_TYPE__##_at_ptr(struct __NB_ARRAY_TYPE__ * buffer, size_t index);               \
  __NB_ARRAY_BLOCK_TYPE__ * __NB_ARRAY_TYPE__##_front_ptr(struct __NB_ARRAY_TYPE__ * buffer);                          \
  __NB_ARRAY_BLOCK_TYPE__ * __NB_ARRAY_TYPE__##_back_ptr(struct __NB_ARRAY_TYPE__ * buffer);                           \
  __NB_ARRAY_BLOCK_TYPE__ __NB_ARRAY_TYPE__##_front(struct __NB_ARRAY_TYPE__ * buffer);                                \
  __NB_ARRAY_BLOCK_TYPE__ __NB_ARRAY_TYPE__##_back(struct __NB_ARRAY_TYPE__ * buffer);                                 \
  void __NB_ARRAY_TYPE__##_remove_at(struct __NB_ARRAY_TYPE__ * buffer, size_t index);                                 \
  void __NB_ARRAY_TYPE__##_remove_front(struct __NB_ARRAY_TYPE__ * buffer);                                            \
  void __NB_ARRAY_TYPE__##_remove_back(struct __NB_ARRAY_TYPE__ * buffer);                                             \
  void __NB_ARRAY_TYPE__##_sort(struct __NB_ARRAY_TYPE__ * buffer, nb_compare_fn compare_fn);                          \
  void __NB_ARRAY__TYPE__##_release(struct __NB_ARRAY_TYPE__ * array);

/**
 * @brief Generates definitions for functions declared with `NAUGHTY_BUFFERS_ARRAY_DECLARATION`.
 * @ingroup array-generator
 */
#define NAUGHTY_BUFFERS_ARRAY_DEFINITION(__NB_ARRAY_TYPE__, __NB_ARRAY_BLOCK_TYPE__)                                   \
  void __NB_ARRAY_TYPE__##_init(struct __NB_ARRAY_TYPE__ * array) {                                                    \
    nb_init(&array->buffer, sizeof(__NB_ARRAY_BLOCK_TYPE__));                                                          \
  }                                                                                                                    \
  void __NB_ARRAY_TYPE__##_init_advanced(                                                                              \
      struct __NB_ARRAY_TYPE__ * array, nb_alloc_fn alloc_fn, nb_realloc_fn realloc_fn, nb_free_fn free_fn,            \
      nb_copy_fn copy_fn, nb_move_fn move_fn, void * memory_context                                                    \
  ) {                                                                                                                  \
    nb_init_advanced(                                                                                                  \
        &array->buffer, sizeof(__NB_ARRAY_BLOCK_TYPE__), alloc_fn, realloc_fn, free_fn, copy_fn, move_fn,              \
        memory_context                                                                                                 \
    );                                                                                                                 \
  }                                                                                                                    \
  enum NB_PUSH_RESULT __NB_ARRAY_TYPE__##_push(struct __NB_ARRAY_TYPE__ * array, const __NB_ARRAY_BLOCK_TYPE__ item) { \
    return nb_push(&array->buffer, (void *)&item);                                                                     \
  }                                                                                                                    \
  enum NB_PUSH_RESULT __NB_ARRAY_TYPE__##_push_ptr(                                                                    \
      struct __NB_ARRAY_TYPE__ * array, const __NB_ARRAY_BLOCK_TYPE__ * item                                           \
  ) {                                                                                                                  \
    return nb_push(&array->buffer, (void *)item);                                                                      \
  }                                                                                                                    \
  enum NB_ASSIGN_RESULT __NB_ARRAY_TYPE__##_assign(                                                                    \
      struct __NB_ARRAY_TYPE__ * array, size_t index, const __NB_ARRAY_BLOCK_TYPE__ item                               \
  ) {                                                                                                                  \
    return nb_assign(&array->buffer, index, (void *)&item);                                                            \
  }                                                                                                                    \
  enum NB_ASSIGN_RESULT __NB_ARRAY_TYPE__##_assign_ptr(                                                                \
      struct __NB_ARRAY_TYPE__ * array, size_t index, const __NB_ARRAY_BLOCK_TYPE__ * item                             \
  ) {                                                                                                                  \
    return nb_assign(&array->buffer, index, (void *)item);                                                             \
  }                                                                                                                    \
                                                                                                                       \
  enum NB_INSERT_RESULT __NB_ARRAY_TYPE__##_insert(                                                                    \
      struct __NB_ARRAY_TYPE__ * array, size_t index, const __NB_ARRAY_BLOCK_TYPE__ item                               \
  ) {                                                                                                                  \
    return nb_insert(&array->buffer, index, (void *)&item);                                                            \
  }                                                                                                                    \
  enum NB_INSERT_RESULT __NB_ARRAY_TYPE__##_insert_ptr(                                                                \
      struct __NB_ARRAY_TYPE__ * array, size_t index, const __NB_ARRAY_BLOCK_TYPE__ * item                             \
  ) {                                                                                                                  \
    return nb_insert(&array->buffer, index, (void *)item);                                                             \
  }                                                                                                                    \
                                                                                                                       \
  __NB_ARRAY_BLOCK_TYPE__ * __NB_ARRAY_TYPE__##_front_ptr(struct __NB_ARRAY_TYPE__ * array) {                          \
    return (__NB_ARRAY_BLOCK_TYPE__ *)nb_front(&array->buffer);                                                        \
  }                                                                                                                    \
                                                                                                                       \
  __NB_ARRAY_BLOCK_TYPE__ __NB_ARRAY_TYPE__##_front(struct __NB_ARRAY_TYPE__ * array) {                                \
    return *__NB_ARRAY_TYPE__##_front_ptr(array);                                                                      \
  }                                                                                                                    \
                                                                                                                       \
  __NB_ARRAY_BLOCK_TYPE__ * __NB_ARRAY_TYPE__##_back_ptr(struct __NB_ARRAY_TYPE__ * array) {                           \
    return (__NB_ARRAY_BLOCK_TYPE__ *)nb_back(&array->buffer);                                                         \
  }                                                                                                                    \
                                                                                                                       \
  __NB_ARRAY_BLOCK_TYPE__ __NB_ARRAY_TYPE__##_back(struct __NB_ARRAY_TYPE__ * array) {                                 \
    return *__NB_ARRAY_TYPE__##_back_ptr(array);                                                                       \
  }                                                                                                                    \
                                                                                                                       \
  void __NB_ARRAY_TYPE__##_remove_at(struct __NB_ARRAY_TYPE__ * array, size_t index) {                                 \
    nb_remove_at(&array->buffer, index);                                                                               \
  }                                                                                                                    \
                                                                                                                       \
  void __NB_ARRAY_TYPE__##_remove_back(struct __NB_ARRAY_TYPE__ * array) { nb_remove_back(&array->buffer); }           \
                                                                                                                       \
  void __NB_ARRAY_TYPE__##_remove_front(struct __NB_ARRAY_TYPE__ * array) { nb_remove_front(&array->buffer); }         \
                                                                                                                       \
  size_t __NB_ARRAY_TYPE__##_count(struct __NB_ARRAY_TYPE__ * array) { return nb_block_count(&array->buffer); }        \
                                                                                                                       \
  __NB_ARRAY_BLOCK_TYPE__ * __NB_ARRAY_TYPE__##_at_ptr(struct __NB_ARRAY_TYPE__ * array, size_t index) {               \
    return (__NB_ARRAY_BLOCK_TYPE__ *)nb_at(&array->buffer, index);                                                    \
  }                                                                                                                    \
                                                                                                                       \
  __NB_ARRAY_BLOCK_TYPE__ __NB_ARRAY_TYPE__##_at(struct __NB_ARRAY_TYPE__ * buffer, size_t index) {                    \
    return *__NB_ARRAY_TYPE__##_at_ptr(buffer, index);                                                                 \
  }                                                                                                                    \
                                                                                                                       \
  void __NB_ARRAY_TYPE__##_sort(struct __NB_ARRAY_TYPE__ * array, nb_compare_fn compare_fn) {                          \
    nb_sort(&array->buffer, compare_fn);                                                                               \
  }                                                                                                                    \
                                                                                                                       \
  void __NB_ARRAY_TYPE__##_release(struct __NB_ARRAY_TYPE__ * array) { nb_release(&array->buffer); }

#endif // NAUGHTY_BUFFERS_ARRAY_GENERATOR_H
