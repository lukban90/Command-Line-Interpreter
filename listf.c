#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//#include <fcntl.h>
#include <sys/dir.h>
//#include <sys/stat.h>
#include <sys/param.h>

#define DELIM " \t\r\n\a"

//Inbuilt sorting 
extern int alphasort();
char pathname[MAXPATHLEN];

void terminate(char *msg) {
  perror(msg);
  exit(0);
}

/*
* function cmpfunc(const void *a, const void *b)
* compare function for qsort(),
* strcasecmp will ignore case strings
*/
int cmpfunc(const void *a, const void *b) {

  return strcasecmp((*(struct dirent **) a)->d_name,
                    (*(struct dirent **) b)->d_name);
}

/*
* function file_select(const struct dirent *entry)
* ignores the following files with '.' or '..' tags
*/
int file_select(const struct dirent *entry) {
    if ((strncmp(entry->d_name, ".", strlen(entry->d_name)) == 0)
    || (strncmp(entry->d_name, "..", strlen(entry->d_name)) == 0))
        return (0);
    else
        return (1);
}

int linkedIn(char tag) {

  switch(tag) {
    case 'l' : return tag;
  }

  return 0;
}

int main(int argc, char *argv[]) {

  int count;
  struct direct **files;

  if(!getcwd(pathname, sizeof(pathname)))
    terminate("Error getting pathname\n");

  count = scandir(pathname, &files, file_select, alphasort);

  /*natural sort the files*/
  qsort(files, count, sizeof(*files), cmpfunc);

  if(count <= 0)
    terminate("No files in this directory\n");


  printf("%d entries found\n",count);
  if(argc == 1) {
    for (int i=1; i<count+1; ++i) {
      printf("%s ",files[i-1]->d_name);
      free(files[i]);
    } printf("\n");
  }

  if(argv[1][0] == '-') printf("MEE\n");

  free(files);
  return 0;
}
