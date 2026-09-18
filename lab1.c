#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  printf("Your prompt\n");
  char *n = NULL;
  size_t size = 0;
  while (1) {
    ssize_t res = getline(&n, &size, stdin);
    char *saveptr;
    char *ret = strtok_r(n, " ", &saveptr);
    while (ret != NULL) {
      printf("%s\n", ret);
      ret = strtok_r(NULL, " ", &saveptr);
    }
  }
  free(n);
}
