// Universidad Complutense
// Operating Systems
// Processes and Scheduling Exercises Sheet - Exercise 3
// Prof: Jose Manuel Velasco

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

int a = 3;
void main() {
    int b = 2;
    int i, p, status;

    printf("Parent ID: %d\n", getpid());

    for (i = 0; i < 4; i++) {
        p=fork();
        if (p==0) {
            b++;
            //a++;
            execlp("ls", "ls", "-l", "/", (char *)NULL);
            a++;
        }
        else {
            wait(&status);
            a++;
            b--;
        }
    }
    printf("PID: %d - a = %d, b = %d\n", getpid(), a, b);
}
