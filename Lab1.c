#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int i, status;
    pid_t F, C1, C2;

    F=fork(); //Η F δημιουργεί μια child process C1
    if (F<0)
    {
       printf("Error");
    }
    else if (F==0) //Άρα πάμε στην child process που είναι η C1
    {
       C1=fork(); //Η C1 δημιουργεί μια child process C3
       if (C1<0)
       {
	  printf("Error");
       }
       else if (C1==0) //Άρα πάμε στην child process που είναι η C3
       {
	  for (i=0; i<10; i++)
          {
            printf("Process %d is executed, my father is %d\n", getpid(), getppid());
            sleep(1);
	  }
          exit(0);
       }

       C1=fork(); //Η C1 δημιουργεί μια child process C4
       if (C1<0)
       {
          printf("Error");
       }
       else if (C1==0) //Άρα πάμε στην child process που είναι η C4
       {
	  for (i=0; i<10; i++)
          {
	      printf("Process %d is executed, my father is %d\n", getpid(), getppid());
	      sleep(1);
	  }
	  exit(0);
       }


       else //Όταν C1>0 επιστρέφουμε στην parent process F με την τιμή ID της child process C1
       {
          for (i=0; i<10; i++)
          {
	      printf("Process %d is executed, my father is %d\n", getpid(), getppid());
	      sleep(1);
          }
          wait(&status);
          wait(&status);
          exit(0);
       }
    }

    F=fork(); //Η F δημιουργεί μια child process C2
    if (F<0)
    {
       printf("Error");
    }
    else if (F==0) //Άρα πάμε στην child process που είναι η C2
    {
       C2=fork(); //Η C2 δημιουργεί μια child process C5
       if (C2<0)
       {
          printf ("Error");
       }
       else if (C2==0) //Άρα πάμε στην child process που είναι η C5
       {
	  for (i=0; i<10; i++)
          {
              printf("Process %d is executed, my father is %d\n", getpid(), getppid());
	      sleep(1);
	  }
       exit(0);
       }


       else //Όταν C2>0 επιστρέφουμε στην parent process F με την τιμή ID της child process C2
       {
          for (i=0; i<10; i++)
          {
	      printf("Process %d is executed, my father is %d\n", getpid(), getppid());
              sleep(1);
          }
          wait(&status);
          exit(0);
       }
    }
    wait(&status);
    wait(&status);
    return 0;
}
