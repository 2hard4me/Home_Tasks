//statfs() является BSD-и Linux-специфичным, с различными структурами на каждом

//statvfs() ,он является стандартным POSIX и лучше обрабатывает большие размеры файлов.
//

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

	struct statvfs buf;

	if (statvfs(argv[1], &buf) < 0) {
		handle_error("Error: statvfs");
	}

	printf("Size of total memory: %lu \n", buf.f_blocks * buf.f_bsize); 
	printf("Size of memory available for unpriviled user: %lu\n", buf.f_bsize * buf.f_bavail); //f_bavail - free blocks for unprivileged users
	printf("Size of memory available in file system: %lu \n", buf.f_bsize * buf.f_bfree); //f_bfree - free blocks
	printf("Size of used memory: %lu \n", buf.f_bsize * (buf.f_blocks - buf.f_bavail));
       
	return 0;
}
