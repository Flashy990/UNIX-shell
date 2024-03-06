
	pid_t child1_pid, child2_pid;
/*	int pipe_fd[2];*/
          /*  
      if (pipe(pipe_fd) < 0) {

	perror("pipe");

      } 
	*/
	if ((child1_pid = fork()) < 0) {

		perror("fork");

	} 
	
	if (child1_pid == 0) {
/*
	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
*/
		if (t->left != NULL) {

	  if (t->left->conjunction == NONE) {

	    execute_command(t->left);

	  } else {

	    execute_conjunction(t->left);

	  }

		}
	
	/*	exit(EXIT_SUCCESS);*/

	} 

waitpid(child1_pid, NULL, 0);


	if ((child2_pid = fork()) < 0) {

		perror("fork");

	} 

	if (child2_pid == 0) {
/*
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);		
*/
		if (t->right != NULL) {

	  if (t->right->conjunction == NONE) {

	    execute_command(t->right);

	  } else {

	    execute_conjunction(t->right);

	  }

		}
	/*	exit(EXIT_SUCCESS);*/

	}
 
/*	close(pipe_fd[0]);
	close(pipe_fd[1]);	     */
		waitpid(child2_pid, NULL, 0);



