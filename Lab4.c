#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void critical_section(int child_number, int critical, int pipefd[2]) {
  read(pipefd[0], &critical, sizeof(critical));
  while (critical==1) {
    read(pipefd[0], &critical, sizeof(critical));
  }
  critical = 1;
  write(pipefd[1], &critical, sizeof(critical));
  for (int i=0; i<5; i++) {
    printf("Child%d %d executes a critical section\n", child_number+1, getpid());
  }
  critical = 0;
  write(pipefd[1], &critical, sizeof(critical));
}

void non_critical_section(int child_number) {
  for (int i=0; i<7; i++) {
    printf("Child%d %d executes a non critical section\n", child_number+1, getpid());
  }
}

int main() {
  int critical=0, i, pipefd[2], status;
  pid_t C[3];

  pipe(pipefd);

  for (i=0; i<3; i++) {
    C[i]=fork();
    if (C[i]==0) {
      if (i==0) {
        critical_section(i, critical, pipefd);
        non_critical_section(i);
        non_critical_section(i);
        critical_section(i, critical, pipefd);
        non_critical_section(i);
        critical_section(i, critical, pipefd);
        close(pipefd[0]);
        close(pipefd[1]);
        exit(0);
      }
      if (i==1) {
        non_critical_section(i);
        critical_section(i, critical, pipefd);
        non_critical_section(i);
        critical_section(i, critical, pipefd);
        non_critical_section(i);
        critical_section(i, critical, pipefd);
        close(pipefd[0]);
        close(pipefd[1]);
        exit(0);
      }
      if (i==2) {
        critical_section(i, critical, pipefd);
        non_critical_section(i);
        critical_section(i, critical, pipefd);
        non_critical_section(i);
        critical_section(i, critical, pipefd);
        non_critical_section(i);
        close(pipefd[0]);
        close(pipefd[1]);
        exit(0);
      }
    }
  }
  write(pipefd[1], &critical, sizeof(critical));
  close(pipefd[0]);
  close(pipefd[1]);
  wait(&status);
  wait(&status);
  wait(&status);
  return 0;
}
