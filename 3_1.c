#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#define N 1024


int main(int argc, char** argv) {
	if (argc != 3) {
		printf("Usage: %s source file target\n", argv[0]);
		return 1;
	}
	
	struct stat fileStat;
	
	if (lstat(argv[1], &fileStat) < 0) {
		perror("Failed to stat");
		return 2;
	}

	if (((fileStat.st_mode) & (S_IFMT)) != S_IFREG) {
		printf("File is not regular\n");
		return 1;
	}
	
	int sourcefd = open(argv[1], O_RDONLY);
	
	if (sourcefd < 0) {
		perror("Failed to open file to copy");
		return 2;
	}

	int destinationfd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); // set mode -rw-r--r--
	if (destinationfd < 0) {
		perror("Failed to open file to write");
		return 2;
	}
	
	int size = fileStat.st_size;

	char buf[N];

	size_t readbytes, writebytes, bytes;

	while (1) {
		readbytes = read(sourcefd, buf, sizeof(buf));

		if (readbytes < 0) {
			perror("Failed to read file to read");
			close(sourcefd);
			close(destinationfd);
			return 2;
		}
		
		if (readbytes == 0) {
			break;
		}

		bytes = readbytes;
		
		while (bytes) {
			writebytes = write(destinationfd, &buf[readbytes - bytes], bytes);
			
			if (writebytes < 0) {
				perror("Failed to write");
				close(sourcefd);
				close(destinationfd);
				return 2;
			}

			bytes -= writebytes;

		}
		
	}
	fsync(destinationfd);
	if (close(sourcefd) < 0) {
		perror("Failed to close source file");
		return 3;	
	}

	if (close(destinationfd) < 0) {
		perror("Failed to close destination file");
		return 3;
	}

	return 0;

}
