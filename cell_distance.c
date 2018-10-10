#include <omp.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  
  int n_threads = 0;

  char* str;
  
  //Ta inn agrument
  int ret;

  int i;

  for(i = 1; i < argc; i++){

    str = argv[i];
    
    ret = strcmp(*str[1], "t");

    printf("%s",str[1]);
    
    if(0 == ret){
      printf("hittat n_threads\n");
      n_threads = (int)strtol(argv[i]+2, (char **)NULL,10);
    }
  }
  
  printf("Kör med %i threads \n",n_threads);
  

  return 0;
}


