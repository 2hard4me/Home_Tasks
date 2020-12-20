#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>

#define EVENT_SIZE ( sizeof (struct inotify_event) + NAME_MAX + 1 )
#define EVENT_BUF_LEN ( 1024 * (EVENT_SIZE + 16) )


int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s [dir]\n", argv[0]);
		return 1;
	}
	ssize_t length, i;
	int filedesc;
	int watchdesc;
	char *buf = malloc(EVENT_BUF_LEN);

	filedesc = inotify_init();

	if (filedesc < 0) {
		perror("Error: inotify init");
		return 1;
	}

	watchdesc = inotify_add_watch(filedesc, ".", IN_MODIFY | IN_CREATE | IN_DELETE);
	
	if (watchdesc < 0) {
		perror("Error: inotify_add_watch");
		close(filedesc);
		return 1;
	}
	
	while (1) {
		i = 0;
		length = read(filedesc, buf, EVENT_BUF_LEN);
		if (length < 0) {
			perror("Error: read");
			close(filedesc);
			close(watchdesc);
			return 1;
		}

		while (i < length) {
			struct inotify_event *event = (struct inotify_event *) &buf[i];
			if (event->len) {
				if (event->mask & IN_CREATE) {
					if (event->mask & IN_ISDIR) {
						printf("Directory %s was created\n", event->name);
					} else {
						printf("The file %s was created\n", event->name);
					}
				} else if (event->mask & IN_DELETE) {
					if (event->mask & IN_ISDIR) {
						printf("Directory %s was deleted\n", event->name);	
					} else {
						printf("The file %s was deleted\n", event->name);
					}
				} else if (event->mask & IN_MODIFY) {
					if (event->mask & IN_ISDIR) {
						printf("Directory %s was modified\n", event->name);
					} else {
						printf("The file %s was modified\n", event->name);
					}
				}
			}
			i += EVENT_SIZE + event->len;
		}
	}
	if (inotify_rm_watch (filedesc, watchdesc) < 0) {
		perror("Error: inotify_rm_watch");
		close(filedesc);
		close(watchdesc);
		return 1;
	}
	if (close(filedesc) < 0) {
		perror("Error: close");
		close(watchdesc);
		return 1;
	}
	if (close(watchdesc) < 0){
		perror("Error: close");
		return 1;
	}
	return 0;
}
