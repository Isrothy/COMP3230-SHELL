#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int letter;
  int delay = 0;
  
  if (argc > 2) {
    printf("Usage: ./relay [millisecond]\n");
    exit(0);
  } else if (argc == 2) {
    delay = atoi(argv[1]);
  }
    
  while ((letter = getchar()) != EOF) {
    putchar(letter);
    if (delay) usleep(delay*1000);
  }
        
  return 0;
}

