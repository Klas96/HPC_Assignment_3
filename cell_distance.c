#include<omp.h>
#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

struct timespec start;
#define CHECKPOINT(fmt, ...) { \
struct timespec now; \
timespec_get(&now, TIME_UTC); \
float elapsed = (now.tv_sec + 1.0e-9 * now.tv_nsec) - (start.tv_sec + 1.0e-9 * start.tv_nsec); \
printf("\n%f: ", elapsed); \
printf(fmt, ##__VA_ARGS__); \
}

int main(int argc, char *argv[]) {
  timespec_get(&start, TIME_UTC);
  CHECKPOINT("Started\n");

  // läser in variabler
  char * endpt;
  int tn = strtol(argv[1]+2, &endpt,10);
  printf("Kör med %i threads \n",tn);

  // hittar antal rader indatan är
  FILE * fp = fopen("cell_data/cell_e5","r");
  fseek(fp, 0L, SEEK_END);
  int sz = ftell(fp);
  fseek(fp, 0L, SEEK_SET);
  int rows = sz/24;
  printf("Filstorlek: %i på %i rader\n",sz, rows);

  // allokerar minne till data
  float* data = (float *)malloc(rows*sizeof(float)*3);

  // läser in data
  CHECKPOINT("Loading data\n");
  for(size_t ix = 0; ix<rows*3; ix++)
    {
      fscanf(fp, "%f" ,&data[ix]);
      //printf("%f \n", data[ix]);
    }
  CHECKPOINT("Finished loading data\n");
  fclose(fp);
  
  //calculate distances count frequency rouded to 2 didgets

  //Outbut (stdout) sorted list of distance eith freq.

  //Note: never use more that 1 GB memory

  return 0;
}
