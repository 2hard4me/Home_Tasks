#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

#define handle_error(message) \
	do { perror(message); exit(EXIT_FAILURE); } while (0)


int main(int argc, char *argv[]) {
	pid_t process_id;
	pid_t p_process_id;
	process_id = getpid(); 			// return process id of calling function
	p_process_id = getppid();		// return process id of parent function
	
	printf("Process id: %d\n", process_id);
	printf("Process parent id: %d\n", p_process_id);

	struct passwd *pwd;

	if ((pwd = getpwuid(getuid())) == 0) {
		handle_error("Error");
	}

	uid_t user_id = pwd->pw_uid;
	char *user_name = pwd->pw_name;
	gid_t group_id = pwd->pw_gid;

	printf("User id: %d\n", user_id);
	printf("User name: %s\n", user_name);
	printf("Group id: %d\n", group_id);

	struct group *grp;

	if ((grp = getgrgid(pwd->pw_gid)) == 0) {
		handle_error("Error");
	}
	
	char *group_name = grp->gr_name;

	printf("Group name: %s\n", group_name);
	
	return 0;
	
}
