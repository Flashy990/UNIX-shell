
  do {

    struct tree *t1 = t->left, *t2 = t->right;

    if (t->conjunction == NONE) {
      int chdir_return_value = -1;
      	printf("t = %s\n", t->argv[0]);
      /*if (t->input == NULL) {

	printf("input is NULL\n");
			
	} */


      /*         printf("NONE: %s, %s", t->argv[0], t->argv[1]);*/


      if ((strcmp(t->argv[0], "exit") == 0) || (strcmp(t->argv[0], "hastalavista") == 0)) {
 
	exit(EXIT_SUCCESS);
		
      
      } else if (strcmp(t->argv[0], "cd") == 0) {

	chdir_return_value = chdir(t->argv[1]);
     
	if (chdir_return_value == -1) {
	
	  perror(t->argv[1]);

	}
     
      } else {
	pid_t child_pid;
	int exe_return_value = -1;

	

	if ((child_pid = fork()) < 0) {

	  perror("fork");

	} else if (child_pid) {

	  wait(NULL);
		/*wait(&status);
		return status;*/

	} else {
	
		
	  if (t->input != NULL) {
			
	    int fd;		
	    if ((fd = open(t->input, O_RDONLY)) < 0) {

	      perror(t->input);
				
	    } 
	    if ((dup2(fd, STDIN_FILENO)) < 0) {

	      perror("dup2");

	    }
	    close(fd);

	  }
			
	  if (t->output != NULL) {
			
	    int fd;	
	    if ((fd = open(t->output, O_WRONLY | O_CREAT | O_APPEND, FILE_PERMS)) < 0) {

	      perror(t->output);
				
	    } 
	    if ((dup2(fd, STDOUT_FILENO)) < 0) {

	      perror("dup2");

	    }
	    close(fd);

	  }

	  /* use execvp to execute the UNIX command input by the user */
	  exe_return_value = execvp(t->argv[0], t->argv);
	  /* print error message if the command is invalid */
	  if (exe_return_value == -1) {

	    perror(t->argv[0]);
	    fflush(stdout);
	    exit(EX_OSERR);

	  }

	  fflush(stdout);
	  exit(EXIT_SUCCESS);

	}
 	

	
      }
    } else {

	if (t->conjunction == PIPE/*strcmp(conj[t->conjunction], "|") == 0*/) {

			
	int pipe_fd[2], child1_pid, child2_pid;
	
        if (t->input != NULL) {
			
	    int fd;		
	    if ((fd = open(t->input, O_RDONLY)) < 0) {

	      perror(t->input);
				
	    } 
	    if ((dup2(fd, STDIN_FILENO)) < 0) {

	      perror("dup2");

	    }
	    close(fd);

	  }
			
	  if (t->output != NULL) {
			
	    int fd;	
	    if ((fd = open(t->output, O_WRONLY | O_CREAT | O_APPEND, FILE_PERMS)) < 0) {

	      perror(t->output);
				
	    } 
	    if ((dup2(fd, STDOUT_FILENO)) < 0) {

	      perror("dup2");

	    }
	    close(fd);

	  }

        if (pipe(pipe_fd) < 0) {

		perror("pipe");

	} 
	
	if ((child1_pid = fork()) < 0) {

		perror("fork");

	} 
  	
	if (child1_pid == 0) {

		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		
		execvp(t->left->argv[0], t->left->argv);
		close(pipe_fd[1]);
	}
 
	if ((child2_pid = fork()) < 0)  {

		perror("fork");

	} 

	if (child2_pid == 0) {

		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		execvp(t->right->argv[0], t->right->argv);
                close(pipe_fd[0]);

	}

	close(pipe_fd[0]);
	close(pipe_fd[1]);	
 	waitpid(child1_pid, NULL, 0);
	waitpid(child2_pid, NULL, 0);



	}
			
	/*		
      printf("t = %s\n", conj[t->conjunction]);
      if (t1->conjunction == NONE) {

	printf("t1 = %s\n", t1->argv[0]);

      } else {

	printf("t1 = %s\n", conj[t1->conjunction]);

      }

      if (t2->conjunction == NONE) {

	printf("t2 = %s\n", t2->argv[0]);

      } else {

	printf("t2 = %s\n", conj[t2->conjunction]);

      }*/

				
    } 
		
    if ((t1 != NULL) && (t1->right == NULL) && (t1->left == NULL)) {

      t = t2;

    } else {

      t = t1;
			
    }
		
  } while (t !=NULL/*(t->left != NULL) && (t->right != NULL)*/);




