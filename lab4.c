#define _DEFAULT_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 128
#define EXTRA_SIZE 256
#define BLOCK_SIZE 128

struct header {
  uint64_t size;
  struct header *next;
};

void handle_error(const char *msg) {
  perror(msg);
  _exit(EXIT_FAILURE);
}

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len = snprintf(buf, BUF_SIZE, format,
                         data_size == sizeof(uint64_t) ? *(uint64_t *)data : *(void **)data);
  if (len < 0) {
    handle_error("snprintf");
  }
  write(STDOUT_FILENO, buf, len);
}

void print_data(struct header *block) {
  unsigned char *data = (unsigned char *)block + sizeof(struct header);
  size_t data_len = block->size - sizeof(struct header);
  for (size_t i = 0; i < data_len; i++) {
    char buf[BUF_SIZE];
    ssize_t len = snprintf(buf, BUF_SIZE, "%d\n", data[i]);
    if (len < 0) {
      handle_error("snprintf");
    }
    write(STDOUT_FILENO, buf, len);
  }
}

void initialize_block(struct header *block, uint64_t size, struct header *next,
                      unsigned char value) {
  block->size = size;
  block->next = next;
  memset((unsigned char *)block + sizeof(struct header), value, size - sizeof(struct header));
}

int main(void) {
  void *heap_start = sbrk(EXTRA_SIZE);
  if (heap_start == (void *)-1) {
    handle_error("sbrk");
  }

  struct header *first_block = (struct header *)heap_start;
  struct header *second_block = (struct header *)((char *)heap_start + BLOCK_SIZE);
  initialize_block(first_block, BLOCK_SIZE, NULL, 0);
  initialize_block(second_block, BLOCK_SIZE, first_block, 1);

  print_out("first block:       %p\n", &first_block, sizeof(&first_block));
  print_out("second block:      %p\n", &second_block, sizeof(&second_block));
  print_out("first block size:  %ld\n", &first_block->size, sizeof(first_block->size));
  print_out("first block next:  %p\n", &first_block->next, sizeof(first_block->next));
  print_out("second block size: %ld\n", &second_block->size, sizeof(second_block->size));
  print_out("second block next: %p\n", &second_block->next, sizeof(second_block->next));
  print_data(first_block);
  print_data(second_block);

  return 0;
}
