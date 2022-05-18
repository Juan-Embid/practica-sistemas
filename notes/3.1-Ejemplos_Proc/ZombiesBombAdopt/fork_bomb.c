#include <stdio.h>
#include <unistd.h>

int main()
{
	pid_t pid;

	while(1)
	{
		pid = fork();
		if( pid == -1 )
		{
			printf( "Error en fork\n" );
		}
	}
}
