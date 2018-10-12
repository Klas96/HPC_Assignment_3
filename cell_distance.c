#include <omp.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

float distence3D(int* x, int* y){

  int dist = (x[0]-y[0])*(x[0]-y[0]) + (x[1]-y[1]])*(x[1]]-y[1]]) + (x[2]-y[2]])*(x[2]]-y[2]]);
  return sqrt((float)dist)/1000;
  //Avrunda

}

int main(int argc, char *argv[]) {

  //Läs inn argumnet

  int n_threads = 0;

  //Read coordinates -10 to 10 from cells.txt

  FILE * fp = fopen("cell_data/cell_50","r");

  char buffer[100];

  (void) fread(buffer, 7*sizeof(char)+1, 10, fp);

  printf("%s\n", buffer);

  //calculate distances count frequency rouded to 2 didgets


  for(int i = 0; i < len(cell_Cord); i++){
    //dont want to comapre with it self therfor j=(i+1)
    for(int j = (i+1); j < len(cell_Cord); j++){
      distance[i] = distence3D(cell_Cord[i],cell_Cord[j]);
    }
  }

  int* dist;
  int* Frequens;

  for(int i = 0; len(distance[1]); i++)){
    for(int j = 0; len(distance[1]); j++){
      //Dont want to comapre
      //if in list increase freq
      if(distFrequens[0][j] == distance[i]){
        Frequens[j]++;
        break;
      }
      //else append to bottom of list
      //Can se if we got to bottom by checking for null or something
      if(distFrequens[0][j] == -1){
        dist[j] = distance[i];
        Frequens[j] = 1;
      }
    }
  }

  //Outbut (stdout) sorted list of distance eith freq.

  //Note: never use more that 1 GB memory

  return 0;
}
