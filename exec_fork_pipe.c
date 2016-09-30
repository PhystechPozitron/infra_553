#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>


int main(int argc, char **argv) {
	// example for exec
	// printf("%d\n", execlp("ls", "ls", "-l", NULL));
	// exit(0);

	// example read from file
	int fd = open("1.txt", O_RDONLY);
	char message1[100];
	int i = 0;
	while (read(fd, &message1[i], 1) > 0) {
		i++;
	}
	close(fd);

	printf("Read from 1.txt: %s\n", message1);



	// main proc
	if (argc < 3) {
		printf("Usage: %s message1 message2\n", argv[0]);
		exit(2);
	}

	// 4 - because create 2 pipes
	int pipefd[4];
	if (pipe(pipefd) == -1) {
		printf("pipe1 failed\n");
		exit(1);
	}
	if (pipe(pipefd + 2) == -1) {
		printf("pipe2 failed\n");
		exit(1);
	}

	for (int j = 0; j < 2; j++) {
		int fork_pid = fork();
		if (fork_pid == -1) {
			printf("fork failed\n");
			exit(2);
		}

		if (fork_pid == 0) {    /* Child reads from pipe */
			// close unused handles
			close(pipefd[2 * j + 1]);
			close(pipefd[2 * (1 - j)]);
			close(pipefd[2 * (1 - j) + 1]);

			char message[100];
			int bytes_read = read(pipefd[2 * j], message, 100);
			message[bytes_read] = 0;

			printf("Read from pipe%d: %s, %d bytes\n", j, message, bytes_read);
			int a = atoi(message);
			printf("Read from pipe%d to int: %d\n", j, a);

			close(pipefd[0]);
			exit(0);
		} else {            /* Parent writes argv[1] to pipe */
			// Close unused read end
			close(pipefd[2 * j]);
			printf("Write to pipe%d %d bytes\n", j, int(strlen(argv[j + 1])));
			write(pipefd[2 * j + 1], argv[j + 1], strlen(argv[j + 1]));
		}
	}


	// Wait for childs
	wait(NULL);

	return 0;
}

