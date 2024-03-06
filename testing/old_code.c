/*****************************************************************************/

    /* exit/hastalavista command 
    if ((strcmp(t->argv[0], "exit") == 0) || (strcmp(t->argv[0], "hastalavista") == 0)) {
 
      printf("See you\n");
      exit(EXIT_SUCCESS);
		
      
    } else if (strcmp(t->argv[0], "cd") == 0) {

      chdir_return_value = chdir(t->argv[1]);
     
      if (chdir_return_value == -1) {
	
	printf("Cannot change to directory %s\n", t->argv[1]);

      }
     
    } else if (0) {*/
			
      /* declare a variable to store the return status of the execvp command */	
      int exe_return_value = -1;
      /* to store the process id */
      pid_t process_id;
      char *arg_exec[3];
      /* set the arg_exec values */
      arg_exec[0] = t->argv[0];
      arg_exec[1] = t->argv[1];
      arg_exec[2] = NULL;
      process_id = fork();

      if (process_id < 0) {

	exit(EX_OSERR);	
	/* make the parent process wait until the child process has finished execution */		
      } else if (process_id) {

	wait(NULL);	
	/* child process: executes the command */
      } else {
	/* use execvp to execute the UNIX command input by the user */
	exe_return_value = execvp(arg_exec[0], arg_exec);
	/* print error message if the command is invalid */
	if (exe_return_value == -1) {

	  printf("Failed to execute %s\n", t->argv[0]);
	  fflush(stdout);
	  exit(EX_OSERR);

	}

	fflush(stdout);
	exit(EXIT_SUCCESS);

      }

  /*  }		
  }

  return 0; 
}
*/


/*****************************************************************************/
 
