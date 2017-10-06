#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFSIZE 1024
#define DELIM " \t\r\n\a"

/*
* function tokenize(char *line)
* tokenizes the string with delimeter
*/
char **tokenize(char *line) {

  int n = 0; //number of strings

  char *line_cpy = (char*)malloc(strlen(line) * sizeof(char)+1);
  strcpy(line_cpy, line);
  line_cpy = strtok(line_cpy, DELIM);

  /*this loop gets the number of words*/
  while(line_cpy != NULL) {
    line_cpy = strtok(NULL, DELIM);
    n++;
  }
  //free(line_cpy);

  int ptr = 0;
  char **p = (char**)malloc(n+1 * sizeof(char*)+1);

  /*this loop tokenize the string*/
  line = strtok(line, DELIM);
  while(line != NULL) {
    p[ptr] = line;
    p[ptr][strlen(line)] = '\0'; //mark the end of each string
    line = strtok(NULL, DELIM);
    ptr++;
  }
  p[ptr] = '\0'; //mark the end of the array

  free(line_cpy);
  return p;
}

/*
* function *pwd(char *cmd)
* is executed if user inputs 'pwd',
* prints the current working directory
*/
char *pwd(char *cmd) {

  long size;
  char *buf;
  char *ptr;

  size = pathconf(".", _PC_PATH_MAX);

  if ((buf = (char *)malloc((size_t)size)) != NULL)
    ptr = getcwd(buf, (size_t)size);

  free(buf);
  return ptr;
}

/*
* function *cd(char *cmd, char *path)
* is executed if user inputs 'cd',
* then will generate a path based on 'path'
*/
char *cd(char *cmd, char *path) {

  long size;
  char *buf;
  char *ptr;

  size = pathconf(".", _PC_PATH_MAX);
  buf = (char *)malloc((size_t)size);

  if(path == NULL) {
    chdir(getenv("HOME"));
    ptr = getcwd(buf, (size_t)size);
    printf("cwd changed to %s\n", ptr);
    return ptr;

  } else if(chdir(path) == -1) {
      printf("no such directory: %s\n", path);
      return path;

  } else if(buf != NULL) {
      ptr = getcwd(buf, (size_t)size);
      printf("cwd changed to %s\n", ptr);
      return ptr;
  }
  free(buf);
  return 0;
}

/*
* function *isBuiltIn(char *cmd, char *path)
* checks if command equals any of the builtins
*/
char *isBuiltIn(char *cmd, char *path) {


  if(strncmp(cmd, "cd", strlen(cmd)) == 0)
    return cd(cmd, path);

  else if(strncmp(cmd, "pwd", strlen(cmd)) == 0) {
    printf("%s ", pwd(cmd));
    return pwd(cmd);
  }

  else if(strncmp(cmd, "exit", strlen(cmd)) == 0)
    {printf("Aborting...\n"); exit(1);}

  return 0;
}

/*
* function make_baby(char * argv[])
* fires the fork() system call, creating
* another process from the parent process
*/
int make_baby(char **args, char *path){

  pid_t pid;
  int status;
  char *route;

  pid = fork();

  if(pid < 0)
    perror("fork error");

  //child process
  //call exec function, execvp - path, vector
  else if(pid == 0) {

    //if(strncmp(args[0], "calc", strlen(args[0])) == 0)
    if(strcmp(args[0], "calculator") == 0) {
      route = (char*)malloc((strlen(path)+strlen("/calculator")) * sizeof(char)+1);
      strcpy(route, path);
      strcat(route, "/calculator");
      execvp(route, args);
    }
    //else if(strncmp(args[0], "listf", strlen(args[0]) == 0))
    else if(strcmp(args[0], "listf") == 0) {
      route = (char*)malloc((strlen(path)+strlen("/listf")) * sizeof(char)+1);
      strcpy(route, path);
      strcat(route, "/listf");
      execvp(route, args);
    }

    else if(execvp(args[0], args) == -1) {
      perror("error");
    }
    free(route);
    exit(EXIT_FAILURE);
  }

  //parent process
  //ensures child has not terminated & checks abnormal termination
  else {
    do {
      waitpid(pid, &status, WUNTRACED);
    } while(!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}

int main(int argc, char *argv[]) {

  char **args = { 0 };
  char line[BUFSIZE];

  long size;
  char *buf;
  char *ptr;

  size = pathconf(".", _PC_PATH_MAX);

  buf = (char *)malloc((size_t)size);
  ptr = getcwd(buf, (size_t)size);

  char *pathToCalc = (char*)malloc((size_t)size);
  char *pathToListf = (char*)malloc((size_t)size);

  strncpy(pathToCalc, ptr, size);

  strncpy(pathToListf, pathToCalc, size);
  printf("LISTF%s\n", pathToListf);

  while(1) {
    printf("$> ");

    if(fgets(line, BUFSIZE, stdin) == NULL) exit(1);

    line[strlen(line)-1] = '\0'; //replace newline with null terminator
    args = tokenize(line);

    if(args[0] == '\0') continue;

    if(isBuiltIn(args[0], args[1]))
      continue;

    /*special case for calculator*/
    else if(strncmp(args[0], "calculator", strlen(args[0])) == 0) {
    //else if(strcmp(args[0], "calc") == 0)
      make_baby(args, pathToCalc);
      continue;
    }

    /*special case for listf*/
    else if(strncmp(args[0], "listf", strlen(args[0])) == 0) {
    //else if(strcmp(args[0], "listf") == 0)
      make_baby(args, pathToListf);
      continue;
    }

    else
      make_baby(args, '\0');

    memset(line, 0, BUFSIZE);
    free(args);
    free(buf);
  }
  free(pathToCalc);
  free(pathToListf);
  return EXIT_SUCCESS;
}
