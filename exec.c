#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t pid = fork();

  if (pid < 0) {
    perror("fork failed");
    return 1;
  } else if (pid == 0) {
    execlp("ls", "ls", "-a", "-1", "-h", NULL);
    perror("excelp failed");
  } else {
    execlp("ls", "ls", "-a", NULL);
    perror("excelp failed");
  }
  return 0;
}
