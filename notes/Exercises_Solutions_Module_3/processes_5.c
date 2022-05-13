// Universidad Complutense
// Operating Systems
// Processes and Scheduling Exercises Sheet - Exercise 5
// Prof: Jose Manuel Velasco

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

int main(){

    int status;
    pid_t pid, pt_id[9];


    pt_id[0] = getpid();
    printf("P0 - Master ID: %d\n", pt_id[0]);

    pid = fork();
    if (pid <0){
        printf("Error creating P1!");
        exit(-1);
    }
    if (pid  == 0){       
        printf("Start P1: %d\n", getpid());
        sleep(1);
        printf("End P1\n");
        exit(EXIT_SUCCESS);
    } 
    pt_id[1] = pid;

    waitpid(pt_id[1], &status, 0);
    printf("Master: End P1 \n");
    printf("Master: Creating P2, P5 and P7\n");

    pid = fork();
    if (pid <0){
        printf("Error creating P2!");
        exit(-1);
    }
    if (pid  == 0){      
        printf("Start P2: %d\n", getpid());
        sleep(1);
        printf("End P2\n");
        exit(EXIT_SUCCESS);
    } 
    pt_id[2] = pid;

    pid = fork();
    if (pid <0){
        printf("Error creating P5!");
        exit(-1);
    }
    if (pid  == 0){      
        printf("Start P5: %d\n", getpid());
        sleep(1);
        printf("End P5\n");
        exit(EXIT_SUCCESS);
    } 
    pt_id[5] = pid;

    pid = fork();
    if (pid <0){
        printf("Error creating P7!");
        exit(-1);
    }
    if (pid  == 0){      
        printf("Start P7: %d\n", getpid());
        sleep(1);
        printf("End P7\n");
        exit(EXIT_SUCCESS);
    } 
    pt_id[7] = pid;

    waitpid(pt_id[2], &status, 0);
    printf("Master: End P2 \n");
    printf("Master: Creating P3 and P4\n");

    pid = fork();
    if (pid <0){
        printf("Error creating P3!");
        exit(-1);
    }
    if (pid  == 0){      
        printf("Start P3: %d\n", getpid());
        sleep(1);
        printf("End P3\n");
        exit(EXIT_SUCCESS);
    } 
    pt_id[3] = pid;

    pid = fork();
    if (pid <0){
        printf("Error creating P4!");
        exit(-1);
    }
    if (pid  == 0){      
        printf("Start P4: %d\n", getpid());
        sleep(1);
        printf("End P4\n");
        exit(EXIT_SUCCESS);
    } 
    pt_id[4] = pid;


    waitpid(pt_id[5], &status, 0);
    printf("Master: End P5 \n");
    waitpid(pt_id[4], &status, 0);
    printf("Master: End P4 \n");
    printf("Master: Creating P6\n");

    pid = fork();
    if (pid <0){
        printf("Error creating P6!");
        exit(-1);
    }
    if (pid  == 0){      
        printf("Start P6: %d\n", getpid());
        sleep(1);
        printf("End P6\n");
        exit(EXIT_SUCCESS);
    } 
    pt_id[6] = pid;

    waitpid(pt_id[3], &status, 0);
    printf("Master: End P3 \n");
    waitpid(pt_id[7], &status, 0);
    printf("Master: End P7 \n");
    waitpid(pt_id[6], &status, 0);
    printf("Master: End P6 \n");
    printf("Master: Creating P8\n");

    pid = fork();
    if (pid <0){
        printf("Error creating P8!");
        exit(-1);
    }
    if (pid  == 0){      
        printf("Start P8: %d\n", getpid());
        sleep(1);
        printf("End P8\n");
        exit(EXIT_SUCCESS);
    } 
    pt_id[8] = pid;
    waitpid(pt_id[8], &status, 0);
    printf("Master: End P8 \n");

    for (int i =0; i< 9;i++){
        printf("P%d - PID: %d\n", i, pt_id[i]);
    }
    
    printf("End Master\n");

    return 0;
}
