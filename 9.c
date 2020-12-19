#include <stdio.h>
#include <sys/vfs.h>
#include <sys/statvfs.h>
#include <stdlib.h>

#define handle_error(message) \
	do { perror(message); exit(EXIT_FAILURE); } while (0)


int main(int argc, char *argv[]) {
	if (argc != 2) {
		handle_error("Incorrect usage");
	}

	struct statfs buf;

	if (statfs(argv[1], &buf) < 0) {
		handle_error("Error: statfs");
	}

	printf("Total memory: %lu \n", buf.f_blocks * buf.f_bsize);
	printf("Available memory: %lu \n", buf.f_bsize * buf.f_bavail);
	printf("Used memory: %lu \n", buf.f_bsize * (buf.f_blocks - buf.f_bavail));
       
	return 0;
}
