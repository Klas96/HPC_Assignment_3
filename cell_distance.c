#include <omp.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

  //Läs inn argumnet

  int n_threads = 0;


/*
  for(int i = 1; i < argc; i++){

    str = argv[i];

    ret = strcmp(*str[1], "t");

    printf("%s",str[1]);

    if(0 == ret){
      printf("hittat n_threads\n");
      n_threads = (int)strtol(argv[i]+2, (char **)NULL,10);
    }
  }

  printf("Kör med %i threads \n",n_threads);
*/
  //Read coordinates -10 to 10 from cells.txt

  FILE * coordinates = fopen("cell_data/cell_50","wb+");

  char cordStr[24];

  fread(cordStr, strlen(cordStr) + 1, 1,coordinates);

  printf("%s\n", cordStr);

  //calculate distances count frequency rouded to 2 didgets

  //Outbut (stdout) sorted list of distance eith freq.

  //Note: never use more that 1 GB memory

  return 0;
}
