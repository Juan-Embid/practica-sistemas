#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int my_system(const char * command) {
    // creamos el proceso hijo
    pid_t pid; // comprobar si tiene que ser global o no
    pid = fork();
    int status;

    switch (pid)
    {
    case -1:
        exit(1);

    case 0: // hijo
        execl("/bin/bash", "/bin/bash", "-c", command, NULL); // /bin/bash , -c, command, NULL

    default: // padre
        wait(&status);
    }

    return status;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf (stderr, "Usage: %s <command> \n", argv[0]);
        exit(1);
    }

    return my_system(argv[1]);
}