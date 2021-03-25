#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char **argv) {
  //Checking if the number of arguments is correct
  if (argc!=3) {
    printf("Error in input\n");
    return 0;
  }

  int i, j, number_of_children=atoi(argv[1]), factorial_number=atoi(argv[2]),last_child,number_of_full_loops,loop=0,pipefd[number_of_children+2][2];
  bool correct_numbers_input=true;
  long int val=1, val_temp, len;
  pid_t C[number_of_children+1];
  //Checking children input
  if (number_of_children<1 || number_of_children>10) {
    correct_numbers_input=false;
  }

  if (strlen(argv[1])>1) {
    if (argv[1][1]!='0' || strlen(argv[1])>2) {
      correct_numbers_input=false;
    }
  }
  //Checking factorial input
  if (factorial_number<0) {
    correct_numbers_input=false;
  }
  for (i=0; i<strlen(argv[2]); i++) {
    if (argv[2][i]-'0'<0 || argv[2][i]-'0'>9) {
      correct_numbers_input=false;
    }
  }

  if (correct_numbers_input==false) {
    printf("Error in input\n");
    return 0;
  }
  //Find number of full loops and the last child which will send factorial number to parent
  if (factorial_number==0) {
	  number_of_full_loops=0;
	  last_child=1;
  }
  else if (factorial_number%number_of_children==0) {
	  number_of_full_loops=(factorial_number/number_of_children)-1;
	  last_child=number_of_children;
  }
  else {
	  number_of_full_loops=(factorial_number/number_of_children);
	  last_child=factorial_number%number_of_children;
  }
  //Creating pipes
  for (i=0; i<number_of_children+2; i++) {
	  pipe(pipefd[i]);
  }

  for (i=1; i<number_of_children+1; i++) {
	  C[i]=fork();
	  if (C[i]==0) {
		  if (i==1) {
        close(pipefd[0][1]);
        close(pipefd[1][0]);
     	  for (j=2; j<number_of_children; j++) {
          close(pipefd[j][0]);
     	  	close(pipefd[j][1]);
        }
        close(pipefd[number_of_children][1]);
        close(pipefd[number_of_children+1][0]);
			  len = read(pipefd[0][0], &val, sizeof(val));
       	val_temp=val*(1+loop);
		  	close(pipefd[0][0]);
		  	if (factorial_number==0 || factorial_number==1) {
          close(pipefd[1][1]);
          close(pipefd[number_of_children][0]);
          printf("C1 reads %ld from parent and writes %ld to parent\n", val, val_temp);
          val=val_temp;
     	  	write(pipefd[number_of_children+1][1], &val, sizeof(val));
     		  close(pipefd[number_of_children+1][1]);
     	  	exit(0);
        }
        printf("C1 reads %ld from parent and writes %ld to C2\n", val, val);
        write(pipefd[1][1], &val, sizeof(val));
        loop++;
			  if (number_of_full_loops==0) {
          close(pipefd[number_of_children+1][1]);
     	    close(pipefd[1][1]);
     	  	close(pipefd[number_of_children][0]);
     	  	exit(0);
        }
        while (loop<number_of_full_loops) {
          len = read(pipefd[number_of_children][0], &val, sizeof(val));
     		  val_temp=val*(1+loop*number_of_children);
          printf("C1 reads %ld from C%d and writes %ld to C2\n", val, number_of_children, val_temp);
          val=val_temp;
     		  write(pipefd[1][1], &val, sizeof(val));
     		  loop++;
        }
     	  if (loop==number_of_full_loops && last_child==1) {
          len = read(pipefd[number_of_children][0], &val, sizeof(val));
     		  val_temp=val*(1+loop*number_of_children);
          printf("C1 reads %ld from C%d and writes %ld to parent\n", val, number_of_children, val_temp);
          val=val_temp;
     		  write(pipefd[number_of_children+1][1], &val, sizeof(val));
     		  close(pipefd[number_of_children+1][1]);
     		  close(pipefd[1][1]);
     		  close(pipefd[number_of_children][0]);
     		  exit(0);
        }
       	if (loop==number_of_full_loops && last_child!=1) {
          len = read(pipefd[number_of_children][0], &val, sizeof(val));
     	  	val_temp=val*(1+loop*number_of_children);
          printf("C1 reads %ld from C%d and writes %ld to C2\n", val, number_of_children, val_temp);
          val=val_temp;
     	  	write(pipefd[1][1], &val, sizeof(val));
     	  	close(pipefd[number_of_children+1][1]);
     	  	close(pipefd[1][1]);
     	  	close(pipefd[number_of_children][0]);
     	  	exit(0);
        }
      }
      if (i!=1) {
        close(pipefd[i-1][1]);
       	close(pipefd[i][0]);
      	for (int j=0; j<i-1; j++) {
          close(pipefd[j][0]);
    	  	close(pipefd[j][1]);
        }
        for (int k=i+1; k<number_of_children+1; k++) {
          close(pipefd[k][0]);
       		close(pipefd[k][1]);
        }
        close(pipefd[number_of_children+1][0]);
        while (loop<number_of_full_loops) {
          len = read(pipefd[i-1][0], &val, sizeof(val));
     	   	val_temp=val*(i+loop*number_of_children);
          if (i==number_of_children) {
            printf("C%d reads %ld from C%d and writes %ld to C1\n", number_of_children, val, number_of_children-1, val_temp);
          }
          else {
            printf("C%d reads %ld from C%d and writes %ld to C%d\n", i, val, i-1, val_temp, i+1);
          }
          val=val_temp;
     	  	write(pipefd[i][1], &val, sizeof(val));
     	  	loop++;
        }
        if (loop==number_of_full_loops && i==last_child) {
          len = read(pipefd[i-1][0], &val, sizeof(val));
          val_temp=val*(i+loop*number_of_children);
          printf("C%d reads %ld from C%d and writes %ld to parent\n", i, val, i-1, val_temp);
          val=val_temp;
     		  write(pipefd[number_of_children+1][1], &val, sizeof(val));
          close(pipefd[number_of_children+1][1]);
     		  close(pipefd[i-1][0]);
     	  	close(pipefd[i][1]);
          exit(0);
        }
        if (loop==number_of_full_loops && i>last_child) {
          close(pipefd[number_of_children+1][1]);
          close(pipefd[i-1][0]);
          close(pipefd[i][1]);
          exit(0);
        }
        if (loop==number_of_full_loops && i<last_child) {
          len = read(pipefd[i-1][0], &val, sizeof(val));
     	  	val_temp=val*(i+loop*number_of_children);
          printf("C%d reads %ld from C%d and writes %ld to C%d\n", i, val, i-1, val_temp, i+1);
          val=val_temp;
     	  	write(pipefd[i][1], &val, sizeof(val));
     	  	close(pipefd[number_of_children+1][1]);
     	  	close(pipefd[i-1][0]);
     	  	close(pipefd[i][1]);
     	  	exit(0);
          }
        }
      }
  }
  close(pipefd[number_of_children+1][1]);
  close(pipefd[0][0]);
  for (int i=1; i<number_of_children+1; i++) {
    close(pipefd[i][0]);
    close(pipefd[i][1]);
  }
  close(pipefd[number_of_children+1][1]);
  printf("Parent writes %ld to C1\n", val);
  write(pipefd[0][1], &val, sizeof(val));
  len = read(pipefd[number_of_children+1][0], &val, sizeof(val));
  printf("Parent reads %ld from C%d\n", val, last_child);
  close(pipefd[0][1]);
  close(pipefd[number_of_children+1][0]);
  return 0;
}
