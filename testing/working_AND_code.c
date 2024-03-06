pid_t child1_pid, child2_pid;

	if ((child1_pid = fork()) < 0) {

		perror("fork");

	} 
	if (child1_pid == 0) {

		/*if ((child2_pid = fork()) < 0) {

			perror("fork");

		}*/

/*		if (child2_pid == 0) {*/

			if (t->left != NULL) {

			  if (t->left->conjunction == NONE) {

			    execute_command(t->left);

			  } else {

			    execute_conjunction(t->left);

			  }		

			}/*
			exit(EXIT_SUCCESS);
		}*/
		

		if (t->right != NULL) {

	  if (t->right->conjunction == NONE) {

	    execute_command(t->right);

	  } else {

	    execute_conjunction(t->right);

	  }

	}

/*		waitpid(child2_pid, NULL, 0);   */
		exit(EXIT_SUCCESS);

		

	} else {

	/* parent */
	waitpid(child1_pid, NULL, 0);

	}


/***************************************************************************/

	pid_t child1_pid, child2_pid;
	
	if ((child1_pid = fork()) && (child2_pid = fork())) {

		waitpid(child2_pid, NULL, 0);
		waitpid(child1_pid, NULL, 0);

	} else {

		if (child1_pid == 0) {

		if (t->left != NULL) {

			  if (t->left->conjunction == NONE) {
/*				printf("%s\n", t->left->argv[0]);*/
			    execute_command(t->left);

			  } else {

			    execute_conjunction(t->left);

			  }		

			}
			exit(EXIT_SUCCESS);

		} 
		
		if (child2_pid == 0) {
	
		if (t->right != NULL) {

		  if (t->right->conjunction == NONE) {
/*			printf("%s\n", t->right->argv[0]);*/
		    execute_command(t->right);

		  } else {

		    execute_conjunction(t->right);

		  }

		} 
		exit(EXIT_SUCCESS);
	
		}

	}


