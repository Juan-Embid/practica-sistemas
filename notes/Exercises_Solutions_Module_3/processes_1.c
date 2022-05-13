// Universidad Complutense
// Operating Systems
// Processes and Scheduling Exercises Sheet - Exercise 1
// Prof: Jose Manuel Velasco

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]){

    printf("Parent: %d\n", getpid());

    int i, pid;
    for(i=1; i<=argc; i++){
        pid = fork();

        printf("i: %d - pid: %d - parentid: %d\n", i, getpid(), getppid());
        if (pid==0){
            //break;
        }
    }
    
    while(1);
    return 0;
}