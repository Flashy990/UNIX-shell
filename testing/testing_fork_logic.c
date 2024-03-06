#include <stdio.h>
#include <sysexits.h> 
#include <err.h> 
#include <unistd.h>    /* Required by fork(), getpid, getppid */
#include <sys/types.h> /* Required by pid_t */
#include <sys/wait.h>  /* Required by wait() */
#include <stdlib.h>

int main() {
   pid_t child1, child2;

	if ((child2 = fork()) || (child1 = fork())) {
		if (child1 == 0) {
			printf("Child1\n");
			exit(0);
		} else {

		printf("Gigachad Parent\n");
		waitpid(child2, NULL, 0);

		waitpid(child1, NULL, 0);

		}
		} else {
		/*if (child1 == 0) {
			printf("Child1\n");
		}*/
		if (child2 == 0) {
			printf("Child2\n");
			exit(0);
		}
	}

   return 0;
}
