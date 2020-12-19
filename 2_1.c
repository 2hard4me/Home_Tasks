#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char **argv) {
	if (argc != 3) {
		fprintf(stderr, "Usage: %s some.file string\n", argv[0]);
		return 1;
	}

	int filedesc = open(argv[1], O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	// Set mode -rw-r--r--
	
	if (filedesc < 0) {
		perror("Failed to open:");
		return 2;
	}

	int count = strlen(argv[2]);
	int counter = 0;
	while (counter < count) {
		ssize_t numofwrbytes = write(filedesc, argv[2], count); //numofwrbytes - number of written bytes is returned
		if (numofwrbytes == -1) {
			perror("Failed to write:");
			close(filedesc);
			return -1;
		}
		counter += numofwrbytes;
	}

	if (close(filedesc) < 0) {
		perror("Failed to close:");
		return 1;
	}
	return 0;
}
