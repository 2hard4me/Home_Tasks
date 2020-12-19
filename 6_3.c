#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define handle_error(message) \
	do { perror(message); exit(EXIT_FAILURE); } while (0);

#define N 1024




struct linux_dirent {
	long           d_ino;
	off_t          d_off;
	unsigned short d_reclen;
	char           d_name[];
};


const char *PRINT_TYPE(mode_t file_mode) {
	switch (file_mode & S_IFMT) {
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
}


const char *PRINT_DIR_TYPE(__uint8_t type) {
	switch (type) {
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


int main(int argc, char *argv[]) {
	if (argc > 2) {
		printf("Usage: %s", argv[1]);
		return 1;
	}
	int filedesc, nread;
	char buf[N];
	struct linux_dirent *d;
	int bpos;
	char d_type;
	struct stat fileStat;

	const char *dirname;
	if (argc == 2) {
		dirname = argv[1];
	} else {
		dirname = ".";
	}

	filedesc = open(dirname, O_RDONLY | O_DIRECTORY);
	
	if (filedesc < 0) {
		handle_error("Failed to open directory\n");
	}

	for ( ; ; ) {
		nread = syscall(SYS_getdents, filedesc, buf, N);
		if (nread < 0) {
			handle_error("Error: getdents\n");
		}

		if (nread == 0) {
			break;
		}

		bpos = 0;
		while (bpos < nread) {
			d = (struct linux_dirent *) (buf + bpos);

			d_type = *(buf + bpos + d->d_reclen - 1);

			printf("File type:\t");
			if (d_type == DT_UNKNOWN) {
				if (lstat(d->d_name, &fileStat) < 0) {
					handle_error("Failed to stat\n");
				}
				PRINT_TYPE(fileStat.st_mode);

			} else {
				PRINT_DIR_TYPE(d_type);					
			}
			printf("Name: %s \n", d->d_name);

			bpos += d->d_reclen;
		}
	}
	
	return 0;
}
