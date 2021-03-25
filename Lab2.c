#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>

void handler_cont() {}
void handler_pause() {
	pause();
}

int main(int argc, char **argv) {
    int i, j, inputs[6], n=1;
    bool boolean_correct_numbers_input=true, boolean_same_numbers_input=false;
    pid_t C[6];

    for (i=1; i<argc; i++)
    {
			if (strlen(argv[i])!=1) {
				boolean_correct_numbers_input=false;
			}
			inputs[i]=atoi(argv[i]);    //Converts string to integer(If the string is not a number atoi() returns 0)--We could use argv[i][0]-'0' as an alternative(Utilizing ASCII Table)
			if (inputs[i]>5 || inputs[i]<1) {     //Checks if inputs are 0<inputs<6 and assigns value false if they are not
				boolean_correct_numbers_input=false;
			}
    }

    for (i=1; i<5; i++) {
      for (j=i+1; j<6; j++) {
        if (inputs[i]==inputs[j]) {   //Checks if we have duplicate inputs and assigns value true if we have
          boolean_same_numbers_input=true;
        }
      }
    }

    if (boolean_correct_numbers_input==false || boolean_same_numbers_input==true || argc!=6) {
      printf("Error in input\n");
      return 0;
    }

    for (i=1; i<6; i++) {
      C[i]=fork();
      if (C[i]==0) {
        signal(SIGCONT, handler_cont);   //Waiting for a signal SIGCONT in order to call handler_cont function
				pause(); //Pauses the child processes until a signal arrives
        signal(SIGSTOP, handler_pause);  //Waiting for a signal SIGSTOP in order to call handler_pause function
        while (1) {
          printf("Child%d %d is executed (%d)\n", i, getpid(), n);
          n++;
          sleep(1); //1 sec interval between prints
        }
      }
    }
		usleep(1000);
		for (j=0; j<4; j++) {
			for (i=1; i<6; i++) {
      kill(C[inputs[i]], SIGCONT);  //Sends signal SIGCONT to child process with pid C[inputs[i]]
      sleep(3);   //Waits for 3 seconds so that the child process prints 3 times "Child%d %d is executed (%d)\n"
      kill(C[inputs[i]], SIGSTOP); //Sends signal SIGSTOP to child process with pid C[inputs[i]]
		}
	}
	for (i=1; i<6; i++) {
		kill(C[i], SIGKILL); //Sends signal SIGKILL to child process with pid C[i](Kills the processes)
	}
	return 0;
}
