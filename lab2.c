#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  char *line = NULL;
  ssize_t len = 0;

  while (1) {
    printf("Enter your prigram to run.\n> ");
    ssize_t n = getline(&line, &len, stdin);
    if (n == -1) {
      break;
    }
    line[n - 1] = '\0';
    pid_t pid = fork();
    if (pid == 0) {
      execlp(line, line, NULL);
      printf("exec failure\n");
      exit(1);
    } else if (pid > 0) {
      int status;
      if (waitpid(pid, &status, 0) == -1) {
        printf("waitpid failed\n");
      }
    } else {
      printf("for failed\n");
    }
  }

  free(line);
  return 0;
}
