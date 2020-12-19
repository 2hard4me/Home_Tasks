#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char **argv) {
	if (argc != 3) {
		printf("Usage: %s some.file string\n", argv[0]);
		return 1;
	}

	int filedesc = open(argv[1], O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	// Set mode -rw-r--r--
	
	if (filedesc < 0) {
		perror("Failed to open:");
		return 2;
	}

	int counter = 0;
	int numofchpr = dprintf(filedesc, "%s", argv[2]); //number of characters printed

	if (numofchpr < 0) {
		perror("Error");
		close(filedesc);
		return 1;
	}

	if (close(filedesc) < 0) {
		perror("Failed to close:");
		return 1;
	}
	return 0;
}
