	int command1_return_status = -1, command2_return_status = -1;
	pid_t child1_pid, child2_pid;		
 /*
        if (pipe(pipe_fd) < 0) {

		perror("pipe");

        } 
*/
        if ((child1_pid = fork()) < 0) {

		perror("fork");

        } 
	 	
      if (child1_pid == 0) {

/*	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
*/     
	if (t->left != NULL) {

	  if (t->left->conjunction == NONE) {
	printf("t-left, no conj, child1\n");
	    command1_return_status = execute_command(t->left);

		if ((WIFEXITED(command1_return_status)) && (WEXITSTATUS(command1_return_status) == 0)) {		
		
		if ((child2_pid = fork()) < 0)  {

			perror("fork");

      		} 

		if (child2_pid == 0) {

		if (t->right != NULL) {
	
		  if (t->right->conjunction == NONE) {
			printf("t-right arg: %s\n", t->right->argv[0]);
		    command2_return_status = execute_command(t->right);

		  } else {

		    execute_conjunction(t->right);

		  }
		
		}
	    
		exit(EXIT_SUCCESS);

	      } else {

			waitpid(child2_pid, NULL, 0);
		
		}	

	}


	  } else {

	    execute_conjunction(t->left);

	  }

/*          read(1, buffer, 1024);*/

	}
	    		
	exit(EXIT_SUCCESS);

      }

	/*printf("status value: %d\n", command1_return_status);	
	if ((WIFEXITED(command1_return_status)) && (WEXITSTATUS(command1_return_status) == 0)) {		
		printf("going in :)\n"); 
		if ((child2_pid = fork()) < 0)  {

			perror("fork");

      		} 

		if (child2_pid == 0) {

		if (t->right != NULL) {
	
		  if (t->right->conjunction == NONE) {

		    command2_return_status = execute_command(t->right);

		  } else {

		    execute_conjunction(t->right);

		  }
		
		}
	    
		exit(EXIT_SUCCESS);

	      }

	}*/

/*      close(pipe_fd[1]);*/

/*	if ((WEXITSTATUS(command1_return_status) == 0) && (WEXITSTATUS(command2_return_status) == 0)) {

		
	}	*/
/*      close(pipe_fd[0]);*/
      waitpid(child1_pid, NULL, 0);
     /* waitpid(child2_pid, NULL, 0);*/
	
