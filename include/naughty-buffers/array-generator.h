#ifndef NAUGHTY_BUFFERS_ARRAY_GENERATOR_H
#define NAUGHTY_BUFFERS_ARRAY_GENERATOR_H

#include "naughty-buffers/buffer.h"

#define NAUGHTY_BUFFERS_ARRAY_DECLARATION(__NB_ARRAY_TYPE__, __NB_ARRAY_BLOCK_TYPE__)                                  \
  struct __NB_ARRAY_TYPE__ {                                                                                           \
    struct nb_buffer buffer;                                                                                           \
  };                                                                                                                   \
  void __NB_ARRAY_TYPE__##_init(struct __NB_ARRAY_TYPE__ * array);                                                     \
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
  void __NB_ARRAY__TYPE__##_release(struct __NB_ARRAY_TYPE__ * array);

#define NAUGHTY_BUFFERS_ARRAY_DEFINITION(__NB_ARRAY_TYPE__, __NB_ARRAY_BLOCK_TYPE__)                                   \
  void __NB_ARRAY_TYPE__##_init(struct __NB_ARRAY_TYPE__ * array) {                                                    \
    nb_init(&array->buffer, sizeof(__NB_ARRAY_BLOCK_TYPE__));                                                          \
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
    return __NB_ARRAY_TYPE__##_at_ptr(array, 0);                                                                       \
  }                                                                                                                    \
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
  void __NB_ARRAY_TYPE__##_release(struct __NB_ARRAY_TYPE__ * array) { nb_release(&array->buffer); }

#endif // NAUGHTY_BUFFERS_ARRAY_GENERATOR_H
