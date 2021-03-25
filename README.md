# Operating-Systems

## Lab 1
Create a **process tree** with Father at root and two children processes C1, C2. C1 has two leaves C3, C4 and C2 has another leaf, C5. Each process prints 10 times its pid and the pid of its parent id with 1 second interval.

## Lab 2
Father process creates **5 children** which are initially paused. He then unpauses each one of them, with an **order given by the user** from the command line. He lets them run for 3 seconds and pauses them again, repeating this task for 4 times. Each child prints its id, its parent id and how many times it has printed with 1 second interval. The pause and the start of the processes are done through **signals**.

## Lab 3
The user provides, from the command line, how many processes to be created and the number whose **factorial** wants to calculate. The father process starts by creating the processes and writing 1 through a pipe to the first chlid. Then the chlidren start **cycling through pipes**, each making a multiplication, until they calculate the factorial of the given number.

## Lab 4
We create 3 processes, with different **critical** and **non-critical** sections, which can be seen at each process' code. We also create a function, which checks if a process is in critical mode, via the use of a **pipe**, and if so, it forbids all other processes to enter their critical section. All processes print whether they are executing a critical (5 times) or a non-critical (7 times) section.

##
*Collaborator: [Dimitris Kranias](https://github.com/dimitriskranias)*
