#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

void do_calc(int read_fd, int write_fd) {
	
}

int main()
{
	printf("%d\n", execlp("ls", "ls", "-l", NULL));
	exit(0);

	int fd = open("3.c", O_RDONLY);
	char message1[100];
	int ii = 0;
	while (read(fd, &message1[ii], 1) > 0) {
		ii++;
	}
	close(fd);

	printf("Read from fd: %s\n", message1);


	int pipefd[4];
	char buf;

	if (pipe(pipefd) == -1) {
		printf("pipe1 failed");
		exit(1);
	}
	if (pipe(pipefd + 2) == -1) {
		printf("pipe2 failed");
		exit(1);
	}

	for (int j = 0; j < 2; j++) {
		pid_t cpid = fork();
		if (cpid == -1) {
			printf("fork failed");
			exit(2);
		}

		if (cpid == 0) {    /* Child reads from pipe */
			close(pipefd[2 * j + 1]);          /* Close unused write end */
			close(pipefd[2 * (1 - j)]);          /* Close unused write end */
			close(pipefd[2 * (1 - j) + 1]);          /* Close unused write end */

			char message[100];
			int i = 0;
			while (read(pipefd[2 * j], &message[i], 1) > 0) {
				i++;
			}

			printf("Read from pipe: %s\n", message);
			int a = atoi(message);
			printf("Read from pipe: %d\n", a);

			close(pipefd[0]);
			exit(0);
		} else {            /* Parent writes argv[1] to pipe */
			close(pipefd[2 * j]);          /* Close unused read end */
		}
	}

	for (int j = 0; j < 2; j++) {
		char message[100];
		sprintf(message, "qwe%d", j);
		write(pipefd[2 * j + 1], message, strlen(message));
		close(pipefd[2 * j + 1]);          /* Reader will see EOF */
	}

	wait(NULL);                /* Wait for childs */

	return 0;
}
