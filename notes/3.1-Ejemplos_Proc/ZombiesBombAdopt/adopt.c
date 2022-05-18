#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
	pid_t pids[10];
	int i;

	for (i = 9; i >= 0; --i) {
		pids[i] = fork();
		if (pids[i] == 0) {
			printf( "Soy %d. Mi padre es %d, voy a dormir un rato...\n", getpid(), getppid() );
			sleep(i+1);
			printf( "Soy %d. ...desperté. Mi padre es %d.\n", getpid(), getppid() );
			_exit(0);
		}
	}

	return 0;
}
