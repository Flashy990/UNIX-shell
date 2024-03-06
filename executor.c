/*
 * Name: Aryan Kakadia
 * Student ID: 117401103
 *
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "command.h"
#include "executor.h"

#define FILE_PERMS 0664

int status;
/*
 * Function to execute commands that don't have any conjuctions
 */
static void execute_command(struct tree *t) {

  /*int return_value = -1;*/
  if (t != NULL) {	
    int chdir_return_value = -1;
    /* implement the exit command functionality */
    if ((strcmp(t->argv[0], "exit") == 0)/* || (strcmp(t->argv[0], "hastalavista") == 0)*/) {
 
      exit(EXIT_SUCCESS);		
    /* implement the change directory functionality */  
    } else if (strcmp(t->argv[0], "cd") == 0) {

      chdir_return_value = chdir(t->argv[1]);
      /* in case an error occurs */
      if (chdir_return_value == -1) {
	
	perror(t->argv[1]);

      }
    /* if its a unix command */
    } else {
      pid_t child_pid;
      int exe_return_value = -1;
      /* fork() and create a child process */
      if ((child_pid = fork()) < 0) {

	perror("fork");
	
      } else if (child_pid) {
	/* parent code */
	/* wait for the child process */
	/* use status to get information about the child's exit status */
	wait(&status);

      } else {
	/* child process */		
	if (t->input != NULL) {
			
	  int fd;	
	  /* code to handle input/output redirection */	
	  /* open in a file in read-only mode using open() system call */
	  if ((fd = open(t->input, O_RDONLY)) < 0) {

	    perror(t->input);
				
	  } 
	  /* copy the open file table entry from fd to STDOUT_NO */
	  if ((dup2(fd, STDIN_FILENO)) < 0) {

	    perror("dup2");

	  }
	  close(fd);

	}
			
	if (t->output != NULL) {
			
	  int fd;	
	  /* open the file to send output */
	  if ((fd = open(t->output, O_WRONLY | O_CREAT | O_APPEND, FILE_PERMS)) < 0) {

	    perror(t->output);
				
	  }

	  /* copy the open file table entry from fd to STDIN_NO */ 
	  if ((dup2(fd, STDOUT_FILENO)) < 0) {

	    perror("dup2");

	  }
	  close(fd);

	}
	/* use execvp to execute the UNIX command input by the user */
	exe_return_value = execvp(t->argv[0], t->argv);
	/* print error message if the command is invalid */
	if (exe_return_value == -1) {

	  exit(EX_OSERR);

	}

	fflush(stdout);
	exit(EXIT_SUCCESS);

      }	
    }
  } 
}

/*
 * function to handle commands with conjunctions
 */
static void execute_conjunction(struct tree *t) {
  /* make sure the tree is not NULL */
  if(t != NULL) {
    /* code to handle the PIPE conjunction */
    if (t->conjunction == PIPE) {
        /* code to handle ambiguous input/output redirects */
      if (t->left->output != NULL) {

        printf("Ambiguous output redirect.\n");
        status = -1;

      } else if (t->right->input != NULL) {
        
        printf("Ambiguous input redirect.\n");
        status = -1;	

      } else {

        int pipe_fd[2], close1_return_value, close2_return_value;
        pid_t child1_pid, child2_pid;
     	/* open up a pipe */ 
        if (pipe(pipe_fd) < 0) {

	  perror("pipe");

        } 
	/* create a child process */
        if ((child1_pid = fork()) < 0) {

	  perror("fork");

        } 
  
        if (child1_pid == 0) {
	  /* child1 process */
	  /* close the read end of the pipe */
	  close(pipe_fd[0]);
	  dup2(pipe_fd[1], STDOUT_FILENO);
	  /* close the write end of the pipe */
	  close(pipe_fd[1]);

	  if (t->left != NULL) {
	    /*
             * execute either execute_command() or execute_conjunction() 
	     * depending on the command on the left leaf of the tree
	     */
	    if (t->left->conjunction == NONE) {

	      execute_command(t->left);

	    } else {

	      execute_conjunction(t->left);

	    }

	  }  
	  		
	  exit(EXIT_SUCCESS);

        }  
 	/* creat another child process */
        if ((child2_pid = fork()) < 0)  {

	  perror("fork");

        } 

        if (child2_pid == 0) {
	  /* inside child2 process */
	  /* close the write end of the pipe */
	  close(pipe_fd[1]);
	  dup2(pipe_fd[0], STDIN_FILENO);
	  /* close the read end of the pipe */
	  close(pipe_fd[0]);	
	
	  if (t->right != NULL) {
            /*
             * execute either execute_command() or execute_conjunction() 
	     * depending on the command on the right leaf of the tree
	     */
	    if (t->right->conjunction == NONE) {

	      execute_command(t->right);

	    } else {

	      execute_conjunction(t->right);

	    }

	  }
	    
	  exit(EXIT_SUCCESS);

        }
	/* close read and write ends of the pipe */
        close1_return_value = close(pipe_fd[0]);
        if (close1_return_value == -1) {
	
	  perror("close()");
          exit(EXIT_FAILURE);

        }
        close2_return_value = close(pipe_fd[1]);	
        if (close2_return_value == -1) {
	
	  perror("close()");
          exit(EXIT_FAILURE);

        }
        /* wait for child1 and child2 */
        waitpid(child1_pid, NULL, 0);
        waitpid(child2_pid, NULL, 0);

      }

    } else if (t->conjunction == AND) {
      /* if we have an AND conjunction in the command */
      pid_t child1_pid;

      /* create a new child process */
      /* catch forking error */
      if ((child1_pid = fork()) < 0) {

        perror("fork");

      } 
      if (child1_pid == 0) {
	/* child process */
        if (t->left != NULL) {
            /*
             * execute either execute_command() or execute_conjunction() 
	     * depending on the command on the left leaf of the tree
	     */
          if (t->left->conjunction == NONE) {

            execute_command(t->left);

          } else {

            execute_conjunction(t->left);

          }	
	
        }
	/*
         *  use the exit status of command1 to determine if command2 should
         *  be executed or not
         */ 
        if ((WIFEXITED(status)) && (WEXITSTATUS(status) == 0)) {
	
          if (t->right != NULL) {
            /*
             * execute either execute_command() or execute_conjunction() 
	     * depending on the command on the right leaf of the tree
	     */
            if (t->right->conjunction == NONE) {

              execute_command(t->right);

            } else {

              execute_conjunction(t->right);

            }
          }
        }

        exit(EXIT_SUCCESS);

      } else {
          /* parent process */
          /* wait for the child to finish execution */
          waitpid(child1_pid, NULL, 0);
	
      }

    } else if (t->conjunction == SUBSHELL) {
      /* if we have a subshell within the command */
      pid_t child1_pid;
      /* create a child process to execute the commands within the subshell */
      /* catch fork error */	
      if ((child1_pid = fork()) < 0) {

        perror("fork");

      } 

      if (child1_pid == 0) {
        /* child process */
        execute(t->left);
        exit(EXIT_SUCCESS);

      } else {
        /* wait for the child process to finish execution */
        waitpid(child1_pid, NULL, 0);

      }

    }
  }
}

int execute(struct tree *t) {

  /* call either execute_command() if the conjunction is the command is NONE
   * otherwise call execute_conjunction() 
   */
  if (t->conjunction == NONE) {

    execute_command(t);

  } else {

    execute_conjunction(t);

  }
  return 0;

}

