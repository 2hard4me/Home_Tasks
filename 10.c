#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>

#define EVENT_SIZE ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN ( 1024 * (EVENT_SIZE + 16) )
#define handle_error(message) \
	do { perror(message); exit(EXIT_FAILURE); } while(0)


int main(int argc, char *argv[]) {
	if (argc > 2) {
		fprintf(stderr, "Usage: %s too many arguments\n");
		return 1;
	}
	int length, i = 0;
	int filedesc;
	int watchdesc;
	char buf[EVENT_BUF_LEN];

	filedesc = inotify_init();

	if (filedesc < 0) {
		handle_error("Error: inotify init");
	}

	watchdesc = inotify_add_watch(filedesc, ".", IN_MODIFY | IN_CREATE | IN_DELETE);
	while (1) {
		i = 0;
		length = read(filedesc, buf, EVENT_BUF_LEN);
		if (length < 0) {
			close(filedesc);
			close(watchdesc);
			handle_error("Error: read");
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
		handle_error("Error: inotify_rm_watch");
	}
	if (close(filedesc) < 0) {
		handle_error("Error: close");
	}
	if (close(watchdesc) < 0){
		handle_error("Error: close");
	}
	return 0;
}
