#include "naughty-buffers/buffer.h"
#include <assert.h>
#include <stdlib.h>

void iterator_works() {
  struct nb_buffer buffer;
  nb_init(&buffer, sizeof(uint32_t));
  int count = rand() % 300;

  for (uint32_t i = 0; i < count; i++) {
    uint32_t value = rand();
    nb_push(&buffer, &value);
  }

  struct nb_buffer_iterator itr = nb_iterator(&buffer);

  size_t i = 0;
  for (void * block = itr.begin; block != itr.end; block += itr.increment) {
    uint32_t * read_value = block;
    uint32_t * expected_value = nb_at(&buffer, i);
    assert(read_value == expected_value);
    i++;
  }

  nb_release(&buffer);
}


int main(void) {
  iterator_works();
  return 0;
}