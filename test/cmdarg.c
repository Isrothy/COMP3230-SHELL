#include <stdio.h>

int main(int argc, char **argv)
{
  int i = 1;
  
  printf("You have entered %d arguments\n", argc);
  printf("They are:\n");
  printf("CMD: %s\n", argv[0]);
  while (i < argc) {
    printf("arg%d: %s\n", i, argv[i]);
    i++;
  }

  return 0;
}