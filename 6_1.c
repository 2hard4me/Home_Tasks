#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define handle_error(message) \
	do { perror(message); exit(EXIT_FAILURE); } while(0);

int main(int argc, char *argv[]) {
	if (argc > 2) {
		printf("Usage: %s", argv[1]);
		return 1;
	}

	DIR *dir;
	struct dirent *sd;
	struct stat fileStat;
	
	const char *dirname;
	
	if (argc == 2) {
		dirname = argv[1];
	} else {
		dirname = ".";
	}

	dir = opendir(dirname);

	if (dir == NULL) {
		handle_error("Unable to open directory\n");
	}
	
	while ((sd = readdir(dir)) != NULL) {
		printf("File type:\t");
		if ((sd->d_type) == DT_UNKNOWN) {
			if (lstat(sd->d_name, &fileStat) < 0) {
				handle_error("Failed to stat\n");
			}
			switch (fileStat.st_mode & S_IFMT) {
				case S_IFBLK:
				      printf("Block device");
				      break;
				case S_IFCHR:
				      printf("Character device");
				      break;
				case S_IFDIR:
				      printf("Directory");
				      break;
				case S_IFIFO:
				      printf("Fifo/pipe");
				      break;
				case S_IFLNK:
				      printf("Symbolyc link");
				      break;
				case S_IFREG:
				      printf("Regular file");
				      break;
				case S_IFSOCK:
				      printf("Socket");
				      break;
				default:
				      printf("Unknown?");
				      break;
			}

		} else {
			switch (sd->d_type) {
				case DT_BLK: 
					printf("Block device");
					break;
				case DT_DIR:
					printf("Directory");
					break;
				case DT_CHR:
					printf("Character device");
					break;
				case DT_FIFO:
					printf("Fifo/pipe");
					break;
				case DT_LNK:
					printf("Symbolic link");
					break;
				case DT_REG:
					printf("Regular file");
					break;
				case DT_SOCK:
					printf("Socket");
			}
	
		}

		printf("Name: %s \n", sd->d_name);
	}

	if (closedir(dir) < 0) {
		handle_error("Failed to close directory\n");
	}
	
	return 0;
}
