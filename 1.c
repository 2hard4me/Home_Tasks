#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#ifdef S_BLKSIZE
#define BL_SIZE S_BLKSIZE
#else
#include <sys/param.h>
#define BL_SIZE DEV_BSIZE
#endif

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Usage: %s some.file\n", argv[0]);
		return 1;
	}
	
	struct stat fileStat;

	if (stat(argv[1], &fileStat) < 0) {
		perror("Failed to stat");
		return 2;
	}

	printf("Information for %s\n", argv[1]);
	printf("---------------------------\n");
	printf("File Size: \t\t%ld bytes\n", fileStat.st_size);
	printf("File type: \n");
	switch (fileStat.st_mode & S_IFMT) {
	case S_IFBLK:  printf("block device\n");	break;
	case S_IFCHR:  printf("character device\n");	break;
	case S_IFDIR:  printf("directory\n");		break;
	case S_IFIFO:  printf("FIFO/pipe\n");		break;
	case S_IFLNK:  printf("symlink\n");		break;
	case S_IFREG:  printf("regular file\n");	break;
	case S_IFSOCK: printf("socket\n");		break;
	default: printf("unknown?\n");			break;
	}
	printf("Mode: %06o\n", fileStat.st_mode);
	printf("Space used: %llu\n", (unsigned long long)fileStat.st_blocks * BL_SIZE);
	printf("Last changed: %s", ctime(&fileStat.st_ctime));
	printf("Last access: %s", ctime(&fileStat.st_atime));
	printf("Last modification: %s", ctime(&fileStat.st_mtime));
	return 0;
}
