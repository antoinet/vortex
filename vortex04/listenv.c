#include <stdlib.h>
#include <stdio.h>

extern char **environ;

int main (int argc, char* argv[]) {
  char** p;

  for (p = environ; *p != NULL; p++) {
    printf("%p\t%s\n", p, *p);
  }
  return 0;
}
 
