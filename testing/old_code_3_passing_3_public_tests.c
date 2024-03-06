


  int chdir_return_value = -1;
  /*
    printf("You must implement me :)\n");
    print_tree(t);*/ 

	
   if (t != NULL) {
    
    print_tree(t->left);

    if (t->conjunction == NONE) {
      /*         printf("NONE: %s, %s", t->argv[0], t->argv[1]);*/


      if ((strcmp(t->argv[0], "exit") == 0) || (strcmp(t->argv[0], "hastalavista") == 0)) {
 
/*	printf("See you\n");*/
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
/*      printf("%s, left: %s, right: %s\n", conj[t->conjunction], t->left->argv[0], t->right->argv[0]);
*/	
	int fd[2], child1_pid, child2_pid;
	if (pipe(fd) < 0) {

		perror("pipe");

	} 
	
	if ((child1_pid = fork()) < 0) {

		perror("fork");

	} 
  	
	if (child1_pid == 0) {

		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		execvp(t->left->argv[0], t->left->argv);

	}
 
	if ((child2_pid = fork()) < 0)  {

		perror("fork");

	} 

	if (child2_pid == 0) {

		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		execvp(t->right->argv[0], t->right->argv);

	}

	close(fd[0]);
	close(fd[1]);	
 	waitpid(child1_pid, NULL, 0);
	waitpid(child2_pid, NULL, 0);

      /*	printf("\nt->argv[0]: %s\n", t->argv[0]);*/

    }
/*    printf("IR: %s, ", t->input);
    printf("OR: %s\n", t->output);
*/
    print_tree(t->right);
      
  }
 
