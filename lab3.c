#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HISTORY_SIZE 5

typedef struct {
  char *lines[HISTORY_SIZE];
  int next;
  int count;
} History;

static void history_init(History *h) {
  for (int i = 0; i < HISTORY_SIZE; i++) {
    h->lines[i] = NULL;
  }
  h->next = 0;
  h->count = 0;
}

static void history_add(History *h, char *line) {
  free(h->lines[h->next]);
  h->lines[h->next] = line;
  h->next = (h->next + 1) % HISTORY_SIZE;
  if (h->count < HISTORY_SIZE) {
    h->count++;
  }
}

static void history_print(const History *h) {
  int start = (h->count < HISTORY_SIZE) ? 0 : h->next;
  for (int i = 0; i < h->count; i++) {
    int idx = (start + i) % HISTORY_SIZE;
    printf("%s\n", h->lines[idx]);
  }
}

static void history_free(History *h) {
  for (int i = 0; i < HISTORY_SIZE; i++) {
    free(h->lines[i]);
    h->lines[i] = NULL;
  }
}
int main(void) {
  History history;
  history_init(&history);

  while (1) {
    printf("Enter input: ");
    fflush(stdout);

    char *line = NULL;
    size_t size = 0;
    ssize_t len = getline(&line, &size, stdin);
    if (len == -1) {
      free(line);
      break;
    }

    if (len > 0 && line[len - 1] == '\n') {
      line[len - 1] = '\0';
    }

    int is_print = (strcmp(line, "print") == 0);
    history_add(&history, line);

    if (is_print) {
      history_print(&history);
    }
  }

  printf("\n");
  history_free(&history);
  return 0;
}
