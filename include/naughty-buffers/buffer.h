#ifndef NAUGHTY_BUFFERS_BUFFER_H
#define NAUGHTY_BUFFERS_BUFFER_H

/**
 * @file buffer.h
 * This file contains the structure nb_buffer which is the core of the naughty-buffers library.
 *
 * @defgroup buffer Buffer
 * All the functions and types to manage a buffer are here. Buffers can automatically grow to handle pushes and
 * insertions. They start with enough space for 2 blocks and grow by powers of 2 (4, 8, 16, 32, etc)
 */

#include "naughty-buffers/naughty-buffers-export.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Type of a function able to allocate a block of memory that will be called when the buffer gets initialized. It
 * should function as `malloc`.
 * @ingroup buffer
 */
typedef void * (*nb_alloc_fn)(size_t size, void * context);

/**
 * @brief Type of a function able to realloc a memory block, to be called when a buffer needs to enlarge its internal
 * memory it will call this function. It should function as `realloc`.
 * @ingroup buffer
 */
typedef void * (*nb_realloc_fn)(void * ptr, size_t new_size, void * context);

/**
 * @brief Type of a function able to copy a chunk of memory from source to destination, to be called when inserting,
 * assigning or pushing data. It will never be called with overlapping memory. It should function as `memcpy`.
 * @ingroup buffer
 */
typedef void * (*nb_copy_fn)(void * destination, const void * source, size_t size, void * context);

/**
 * @brief Type of a function able to move a chunk of memory from source to destination, to be called when inserting or
 * removing data from the buffer. It needs to handle overlapping memory and should function like `memmove`.
 * @ingroup buffer
 */
typedef void * (*nb_move_fn)(void * destination, const void * source, size_t size, void * context);

/**
 * @brief Type of a function able to release memory pointed by `ptr`, to be called when the buffer gets released. It
 * should function like `free`.
 * @ingroup buffer
 */
typedef void (*nb_free_fn)(void * ptr, void * context);

/**
 * @brief Type of a function that can compare two blocks, returning `< 0` if `*ptr_a < *ptr_b`, `0` if `*ptr_a == *ptr_b`
 * or `> 0` if `*ptr_a > *ptr_b`.
 * @ingroup buffer
 */
typedef int (*nb_compare_fn)(const void * ptr_a, const void * ptr_b);

/**
 * @brief a structure holding the buffer data and metadata about the blocks.
 *
 * It should be treated as an opaque structure and be access through naughty-buffers functions.
 *
 * Initialize it by using ::nb_init or ::nb_init_advanced. Don't use before initialization.
 *
 * @ingroup buffer
 * @sa ::nb_init
 * @sa ::nb_init_advanced
 * @sa ::nb_release
 */
struct nb_buffer {
  size_t block_size;
  size_t block_count;
  size_t block_capacity;

  nb_alloc_fn alloc_fn;
  nb_realloc_fn realloc_fn;
  nb_free_fn free_fn;
  nb_copy_fn copy_fn;
  nb_move_fn move_fn;
  void * memory_context;

  void * data;
};

/**
 * @brief Result of calling ::nb_push
 * @ingroup buffer
 */
enum NB_PUSH_RESULT { NB_PUSH_OUT_OF_MEMORY, NB_PUSH_OK };

/**
 * @brief Result of calling ::nb_assign
 * @ingroup buffer
 */
enum NB_ASSIGN_RESULT { NB_ASSIGN_OUT_OF_MEMORY, NB_ASSIGN_OK };

/**
 * @brief Result of calling ::nb_insert
 * @ingroup buffer
 */
enum NB_INSERT_RESULT { NB_INSERT_OUT_OF_MEMORY, NB_INSERT_OK };

/**
 * @brief Initializes a ::nb_buffer struct with default values and pointers.
 *
 * It will allocate enough memory to contain two blocks. All memory functions
 * will be set to end up calling the default ones (malloc/realloc/etc).
 *
 * @param buffer A pointer to a ::nb_buffer struct to be initialized
 * @param block_size The size, in bytes, for each buffer block
 *
 * **Example**
 * @code
 * // initializes and releases a buffer to hold int-sized blocks
 *
 * void main() {
 *   struct nb_buffer buffer;
 *   nb_init(&buffer, sizeof(int));
 *   nb_release(&buffer);
 * }
 * @endcode
 * @ingroup buffer
 */
NAUGHTY_BUFFERS_EXPORT void nb_init(struct nb_buffer * buffer, size_t block_size);

/**
 * @brief Initializes a ::nb_buffer struct with custom memory functions and context.
 *
 * You need to provide all custom memory functions and optionally a memory context.
 *
 * It will allocate enough memory to contain two blocks with `alloc_fn`
 *
 * @param buffer A pointer to a ::nb_buffer struct to be initialized
 * @param block_size Size, in bytes, for each buffer block
 * @param alloc_fn A function to allocate a memory block. It needs to have the same semantics of `malloc`
 * @param realloc_fn A function to reallocate a memory block. It needs to have the same semantics of `realloc`
 * @param free_fn A function to release a memory block allocated by `alloc_fn` or `realloc_fn`. Needs to have the same
 * semantics of `free`
 * @param copy_fn A function to copy non-overlapping data from a block to another. It needs to have the same semantics
 * of `memcpy`
 * @param move_fn A function to copy possibly overlapping data from a block to another. It needs to have the same
 * semantics of `memmove`
 * @param memory_context A pointer to an optional context that will be passed to each memory function.
 *
 * **Example**
 * @code
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
      memory_context
    );

    nb_release(&buffer);
    return 0;
  }
 *
 * @endcode
 *
 * @ingroup buffer
 */
NAUGHTY_BUFFERS_EXPORT void nb_init_advanced(
    struct nb_buffer * buffer,
    size_t block_size,
    nb_alloc_fn alloc_fn,
    nb_realloc_fn realloc_fn,
    nb_free_fn free_fn,
    nb_copy_fn copy_fn,
    nb_move_fn move_fn,
    void * memory_context
);

/**
 * @brief Copies data to the end of the buffer, possibly reallocating it if more space needed.
 *
 * @param buffer A pointer to a ::nb_buffer struct
 * @param data The data to copy.
 * @return `NB_PUSH_OK` if successful, `NB_PUSH_OUT_OF_MEMORY` if no more memory could be allocated.
 * @warning Because the buffer when reallocated can change places, all previous pointers returned by ::nb_at may be
 * invalid after calling this function.
 *
 * **Example**
 * @code
  int main(void) {
    struct nb_buffer buffer;
    nb_init(&buffer, sizeof(int));

    int value = 0;
    nb_push(&buffer, &value);
    assert(nb_block_count(&buffer) == 1);

    nb_release(&buffer);
    return 0;
  }
 * @endcode
 *
 * @ingroup buffer
 */
NAUGHTY_BUFFERS_EXPORT enum NB_PUSH_RESULT nb_push(struct nb_buffer * buffer, void * data);

/**
 * @brief Returns the block count of the buffer.
 * @param buffer A pointer to a ::nb_buffer struct
 * @return The block count of the buffer
 * @ingroup buffer
 *
 * **Example**
 *
 * See ::nb_push example
 */
NAUGHTY_BUFFERS_EXPORT size_t nb_block_count(struct nb_buffer * buffer);

/**
 * @brief Returns a pointer to the block at position `index` or NULL if the index is out of bounds
 * @param buffer A pointer to a ::nb_buffer struct
 * @param index The index to read
 * @return A pointer to the block data or NULL if the index is out of bounds
 * @warning Using ::nb_push, ::nb_insert or ::nb_assign might invalidate previous pointers returned by this function
 * @ingroup buffer
 *
 * **Example**
 * @code
  int main(void) {
    struct nb_buffer buffer;
    nb_init(&buffer, sizeof(int));

    int value = 10;
    int * read_value;
    nb_push(&buffer, &value);
    read_value = (int*) nb_at(&buffer, 0);
    assert(*read_value = 10);

    nb_release(&buffer);
    return 0;
  }
 * @endcode
 */
NAUGHTY_BUFFERS_EXPORT void * nb_at(const struct nb_buffer * buffer, size_t index);

/**
 * @brief Returns a pointer to the first block or NULL if the buffer is empty.
 * This is equivalent to calling ::nb_at with index 0
 *
 * @param buffer A pointer to a ::nb_buffer struct
 * @param index The index to read
 * @return A pointer to the block data or NULL if the buffer is empty
 * @warning Using ::nb_push, ::nb_insert or ::nb_assign might invalidate previous pointers returned by this function
 * @ingroup buffer
 */
NAUGHTY_BUFFERS_EXPORT void * nb_front(struct nb_buffer * buffer);

/**
 * @brief Returns a pointer to the last block or NULL if the buffer is empty
 * This is equivalent to calling ::nb_at with index `nb_block_count(&buffer) -1`
 *
 * @param buffer A pointer to a ::nb_buffer struct
 * @param index The index to read
 * @return A pointer to the block data or NULL if the buffer is empty
 * @warning Using ::nb_push, ::nb_insert or ::nb_assign might invalidate previous pointers returned by this function
 * @ingroup buffer
 */
NAUGHTY_BUFFERS_EXPORT void * nb_back(struct nb_buffer * buffer);

/**
 * @brief Copies `data` to the block at index `index`.
 *
 * If `index` is larger than the buffer capacity the buffer data will be reallocated to accommodate the new data.
 * Uninitialized data will be present in the blocks preceding the new data if they were not present before.
 *
 * @param buffer A pointer to a ::nb_buffer struct
 * @param index The block index to assign the data to
 * @param data A pointer to the data to be copied in the buffer at the specified index.
 * @return NB_ASSIGN_OK if assignment was successful or NB_ASSIGN_OUT_OF_MEMORY if out of memory
 * @ingroup buffer
 *
 * **Example**
 * @code
  int main(void) {
    struct nb_buffer buffer;
    nb_init(&buffer, sizeof(int));

    int value = 10;
    int * read_value;
    nb_assign(&buffer, 20, &value);
    read_value = (int*) nb_at(&buffer, 20);
    assert(*read_value = 10);

    nb_release(&buffer);

    return 0;
  }
 *  @endcode
 */
NAUGHTY_BUFFERS_EXPORT enum NB_ASSIGN_RESULT nb_assign(struct nb_buffer * buffer, size_t index, void * data);

/**
 * @brief Inserts `data` to the block at index `index` moving all blocks past the index forward one position.
 *
 * If `index` is larger than the buffer capacity the buffer data will be reallocated to accommodate the new data.
 * Uninitialized data will be present in the blocks preceding the new data if they were not present before.
 *
 * All blocks in positions greater or equal than the one specified with `index` will be moved forward by 1 position.
 * @param buffer A pointer to a ::nb_buffer struct
 * @param index The block index to assign the data to
 * @param data A pointer to the data to be copied in the buffer at the specified index.
 * @return NB_INSERT_OK if assignment was successful or NB_INSERT_OUT_OF_MEMORY if out of memory
 * @ingroup buffer
 *
 * **Example**
 * @code
  int main(void) {
    struct nb_buffer buffer;
    nb_init(&buffer, sizeof(int));

    int value = 0;
    nb_push(&buffer, &value);

    value = 10;
    nb_insert(&buffer, 0, &value);
    assert(nb_block_count(&buffer) == 2);

    nb_release(&buffer);

    return 0;
  }
 * @endcode
 */
NAUGHTY_BUFFERS_EXPORT enum NB_INSERT_RESULT nb_insert(struct nb_buffer * buffer, size_t index, void * data);

/**
 * @brief Removes the block at index 0 from the array moving all other blocks to the beginning.
 *
 * This is equivalent of calling ::nb_remove_at with index `nb_block_count(&buffer) -1`.
 *
 * @warning This function will invalidate all previously returned pointers with `nb_at`
 * @param buffer A pointer to a ::nb_buffer struct
 * @ingroup buffer
 * @sa nb_remove_at
 * @sa nb_remove_back
 */
NAUGHTY_BUFFERS_EXPORT void nb_remove_front(struct nb_buffer * buffer);

/**
 * @brief Removes the block at the last index from the array.
 *
 * This is equivalent of calling ::nb_remove_at with index `nb_block_count(&buffer) -1`.
 *
 * @warning This function will invalidate pointers to the last block of the buffer previously returned by `nb_at`
 * @param buffer A pointer to a ::nb_buffer struct
 * @ingroup buffer
 * @sa nb_remove_at
 * @sa nb_remove_front
 */
NAUGHTY_BUFFERS_EXPORT void nb_remove_back(struct nb_buffer * buffer);

/**
 * @brief Removes the block at the specified index.
 *
 * @warning This function will invalidate pointers previously returned by ::nb_at for blocks at the index and past it
 * @param buffer A pointer to a ::nb_buffer struct
 * @sa nb_remove_front
 * @sa nb_remove_back
 * @ingroup buffer
 *
 * **Example**
 * @code
  int main(void) {
    struct nb_buffer buffer;
    nb_init(&buffer, sizeof(int));

    int value = 0;
    nb_push(&buffer, &value);

    value = 10;
    nb_push(&buffer, &value);
    assert(nb_block_count(&buffer) == 2);

    nb_remove_at(&buffer, 0);
    assert(nb_block_count(&buffer) == 1);

    nb_release(&buffer);

    return 0;
  }
 * @endcode
 */
NAUGHTY_BUFFERS_EXPORT void nb_remove_at(struct nb_buffer * buffer, size_t index);

/**
 * @brief Sorts the buffer using stdlib's qsort function.
 *
 * @param buffer A pointer to a ::nb_buffer struct
 * @param compare_fn A comparison fuction returnin < 0 if the first element should come before the second, 0 if they're
 * equal and > 0 if the first element should come after the second
 * @ingroup buffer
 */
NAUGHTY_BUFFERS_EXPORT void nb_sort(struct nb_buffer * buffer, nb_compare_fn compare_fn);

/**
 * @brief Releases all allocated memory by the buffer and resets all internal metadata effectively making it an
 * uninitialized buffer.
 *
 * You can reuse the same buffer after another call to ::nb_init or ::nb_init_advanced.
 * @param buffer A pointer to a ::nb_buffer struct
 * @ingroup buffer
 */
NAUGHTY_BUFFERS_EXPORT void nb_release(struct nb_buffer * buffer);

#ifdef __cplusplus
};
#endif

#endif // NAUGHTY_BUFFERS_BUFFER_H
