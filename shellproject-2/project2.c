/*
CS370 
Project 2 
Maira Soomro 
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h>
#include <termios.h> 
#include <string.h> 
#include <stdbool.h> 


void joinFiles(char* file1, char* file2, char* file3){ 
  FILE *inputFile1, *inputFile2; 
  FILE *outputFile; 
  char line; 

  inputFile1 = fopen(file1, "r");
  inputFile2 = fopen(file2, "r"); 

  outputFile = fopen(file3, "w"); 

  while((line = fgetc(inputFile1))!= EOF) { 
    fputc(line, outputFile);
  }
  fclose(inputFile1); 

  while((line = fgetc(inputFile2)) != EOF) { 
    fputc(line, outputFile); 
  } 
  fclose(inputFile2); 
  fclose(outputFile); 
} 

int main() {

  bool running = true; 
  int size = 100; // initial size, change later 


  // CHECK FOR MORE SPACE! 
  char *tokenArr[100];
  char *pipeArr[100]; 
  char *token; 
  int tkCnt = 0; 
 
  // get the original configuration 
  struct termios origConfig; 
  tcgetattr(0, &origConfig); 

  // create a copy of the original 
  struct termios newConfig = origConfig; 

  // adjust the new configuration 
  newConfig.c_lflag &= ~(ICANON|ECHO); 
  newConfig.c_cc[VMIN] = 1; 
  newConfig.c_cc[VTIME] = 2;

  // set the new configuration 
  tcsetattr(0, TCSANOW, &newConfig);   

  // EVERYTHING 
    while(running) { 
    
      char inputLn; 
    
      /* PRINT PROMPT */
      char *buffer = (char*)calloc(256,sizeof(char));
      getcwd(buffer,256);
      printf("%s> ", buffer); 

      // frees up memory in the buffer 
      free(buffer); 

      // get line of  input from user 
      /* char *getChar = (char*)calloc(100,sizeof(char)); 
	 fgets (getChar,100,stdin); */

      char c; 
      char endof = '\n'; 
    
      char message[size]; 
      int count = 0; // counts each character  

      //printf("%d\n", count); 
      do{
        // get character 
        c = getchar(); 

	/*      	if (c == 27) {   
	  char tra;
	  tra = getchar();
	  putchar(tra); 
	  tra = getchar();
	  putchar(tra); 
	  if (tra == 'A')
	    break;

	  else if (tra == 'B')
	    break; 

	  else if (tra == 'C') 
	    break; 

	  else if (tra == 'D')
	    break; 
	  
	    }*/  
	
	if (c == 27) { 
	  getchar(); 
	  switch(getchar()) { 
	  case 65: 
	    break; 
	  case 66:
	    break;
	  case 67: 
	    break; 
	  case 68:
	    break; 
	  }   
	}  
	else { 
	if((c == 127 || c==8) && (count>0)) {
	  printf("\b \b");
	  count--; 
	  message[count]=0;

	} 

	else if(c == 10) { 
	  printf("\n");
	  message[count] = 0; 
	} 
	else {
	  if (c != 127 && c!= 8) { 
	    // store it into an array 
	    message[count] = c; 
	    count++; 
	    //printf("backspace is entered");
	    // print it 
	    putchar(c);
	  } 
	}
	}
      } while(c != '\n');

      // reinitialize 
      count = 0; 
      // test
      //printf("user input %s\n", message);

      token = strtok(message, " \n"); 

      bool boolPipe = false; 
      int pipeNum; 

      while(token != NULL) 
	{ 
	  tokenArr[tkCnt] = token; 
	  if (tokenArr[tkCnt] != 0) 
	    { 
	      if(strcmp(tokenArr[tkCnt], "|") == 0) 
		{
		  pipeNum = tkCnt; 
		  boolPipe = true;
		  //	    printf("true"); 
		  tokenArr[tkCnt] = 0;
		}
	      else
		tkCnt++; 
	    }
	  

	// test 
	// printf("token: %s\n", token);
	token = strtok(NULL, " \n");
	} 

      int x, y; 
      int pipeCnt = 0; 
      int newTkCnt = 0; 

      if(boolPipe){ 
	for (x = 0; x < pipeNum; x++) { 
	  printf("cmd: %s\n", tokenArr[x]); 
	} 
	
	newTkCnt = pipeNum; 
	for (y = pipeNum; y<tkCnt; y++){ 
	  printf("2cmd: %s\n", tokenArr[y]);
	  pipeArr[pipeCnt] = tokenArr[y]; 
	  // clears these from 1st array
	  tokenArr[y] = 0; 
	  pipeCnt++; 
	} 

	// make it null terminated 
	tokenArr[newTkCnt] = 0; 
	pipeArr[pipeCnt] = 0; 
	printf("count: %d\n", pipeCnt); 
      }

      /*      
	while(token != NULL) { 
	
	tokenArr[tkCnt] = token; 
	tkCnt++; 
	// test
	// printf("token: %s\n", token);
	token = strtok(NULL, " \n"); 
	} */

      int j; 
      for (j = 0; j<newTkCnt; j++) { 
	printf("1st Array %s\n", tokenArr[j]); 
      }

      if(!boolPipe) { 
	tokenArr[tkCnt] = 0; 
      }

       
      //free(message); 
      // free(token); 
      // test 

      /*h
      printf("%s", tokenArr[0]); 
      printf("%s", tokenArr[2]);
      printf("%d", tkCnt); 
      */ 
      // re-initialize token count 
      tkCnt = 0; 
     
      if (tokenArr[0]!= 0) { 

      if(strcmp(tokenArr[0], "cd") == 0) {
	int path; 
	
	path = chdir(tokenArr[1]);
	if(path !=0) { 
	  perror(NULL);
	} 
      }

      
      else if(strcmp(tokenArr[0], "exit") == 0){ 
	char ans; 

	// fix this to include enter/linefield before 
	// executing 

	while ((ans != 'y' || ans != 'n') ) { 
	  printf("Are you sure you want to exit? y/n "); 

	  ans = getchar();
	  putchar(ans);

	  if (ans == 'y' ) {
	    printf("\n"); 
	    tcsetattr(0,TCSANOW, &origConfig); 
	    exit(0); 
	  }
	  else {
	    printf("\n"); 
	    break; 
	  } 
	}
      
      }

      // merge command


      else if((strcmp(tokenArr[0], "join") == 0) && (strcmp(tokenArr[3],">")==0)) {
     
	joinFiles(tokenArr[1], tokenArr[2], tokenArr[4]);
      }

	else 
	  {

	    if(boolPipe) { 
	      int fd[2]; // file descriptor
	      int pid, pid2;
	      int stat;

	      pipe(fd);
	      pid = fork();
	      if(!pid){
		close(fd[0]);
		close(1); // must put this otherwise errorr!
		dup(fd[1]);
		execvp(tokenArr[0], tokenArr); // first array of cmds
		perror("error"); 
	      } 

	      close(fd[1]); 
	      waitpid(pid, &stat, WUNTRACED); 
	      
	      pid2 = fork(); 
	      if(!pid2) {  
		dup2(fd[0], 0); // use dup2 here
		close(fd[1]);
		close(fd[0]); 
		execvp(pipeArr[0], pipeArr); // second array of commands
		perror("error");
	      } 
	       
	      waitpid(pid2, &stat, WUNTRACED); 

	      //if(fork()) #parent
	      //  if (fork()) #parent 
	      //  else # child2 
	      //else #child1

	     

	      
	      /*	      if(pid == 0) { 
		if(pid == 0 ) 
		close(0); 
		dup(fd[0]);
		close(fd[1]); 
		execvp(pipeArr[0], pipeArr);
		//		waitpid(pid, &stat, WUNTRACED);
	      } 
	      else if(pid>1) { 
		close(1); 
		dup(fd[1]);
		close(fd[0]);
		execvp(tokenArr[0], tokenArr);
		//		waitpid(pid, &stat, WUNTRACED);
	      } 
	      else { 
		perror(NULL);
		} */ 


	    } 
	    
	    else { 
	      int pid, status; 
	      pid = fork();
	      if (pid > 0) { // parent process
		waitpid(pid, &status, WUNTRACED);
	      }

	      else if (pid == 0) { // child process             
		execvp(tokenArr[0], tokenArr);
	      }
	      
	      else  {
		perror(NULL);
	      }
	    }
	  }

      }

    } 

  // restore the original configuration when done 
  tcsetattr(0, TCSANOW, &origConfig); 

  return 0; 
} 
