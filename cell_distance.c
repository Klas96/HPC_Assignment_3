#include<omp.h>
#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>

//float distence3D(float* x, float* y) {
int distence3D(float x1, float x2, float x3, float y1, float y2, float y3) {
  //int dist = (x[0]-y[0])*(x[0]-y[0]) + (x[1]-y[1])*(x[1]-y[1]) + (x[2]-y[2])*(x[2]-y[2]);
  int dist = (x1-y1)*(x1-y1) + (x2-y2)*(x2-y2) + (x3-y3)*(x3-y3);
  return ((int)sqrt(dist)*100);
  //Avrunda??
}

/*
void distAll() {

  float * distance = (float *)malloc(rows*rows*sizeof(float));

  for(int i = 0; i < rows; i++){
    for(int j = (i+1); j < rows; j++){
      distance[i+j-1] = distence3D(data_p[i*3 + 0] , data_p[i*3 + 1], data_p[i*3 + 2], data_p[j*3 + 0], data_p[j*3 + 1], data_p[j*3 + 2]);
    }
  }
}
*/

void sort(/* arguments */) {
  /* code */
}

int main(int argc, char *argv[]) {

  // läser in variabler
  char * endpt;
  int tn = strtol(argv[1]+2, &endpt,10);
  //export OMP_NUM_THREADS=tn;
  omp_set_num_threads(tn);
  printf("Kör med %i threads \n",tn);

  // hittar antal rader indatan är
  FILE * fp = fopen("cell_data/cell_e2","r");

  fseek(fp, 0L, SEEK_END);
  int sz = ftell(fp);
  fseek(fp, 0L, SEEK_SET);
  int rows = sz/24;
  printf("Filstorlek: %i på %i rader\n",sz, rows);

  	//rows*4 byte Storlek på inläsning.

  // allokerar minne till data

  //float** data_pp = (float **)malloc(rows);
  float* data_p = (float *)malloc(rows*sizeof(float)*3);//tidigare

  // läser in data


  for(size_t ix = 0; ix<rows*3; ix++){
        fscanf(fp, "%f" ,&data_p[ix]);
        //printf("%f \n", data[ix]);
    }

  fclose(fp);

  //calculate distances count frequency rouded to 2 didgets

  int dist_loc;

  int* dist = (int*)malloc((1733)*sizeof(int));
  int* Frequens = (int*)malloc((1733)*sizeof(int));

  for(int i = 0; i < 1733; i++){
    //dist[i] = i*0.01;//Float
    dist[i] = i;//Int
    Frequens[i] = 0;
  }


  #pragma omp parallel shared(Frequens)
  {
  for(int i = 0; i < rows; i++){
    //dont want to comapre with it self therfor j=(i+1)
    int nx = omp_get_thread_num();
    printf("Thread %i: I take row %i \n",nx,i);
    for(int j = (i+1); j < rows; j++){
      dist_loc = distence3D(data_p[i*3 + 0] , data_p[i*3 + 1], data_p[i*3 + 2], data_p[j*3 + 0], data_p[j*3 + 1], data_p[j*3 + 2]);
      for(int k = 0; k < 1733; k++){
        if(dist[k] == dist_loc){
          Frequens[k]++;
          break;
        }
      }
    }
  }
  }
  //Outbut (stdout) sorted list of distance eith freq.

  //Note: never use more that 1 GB memory

  for(int i = 0; i < 1733; i++){
    if(Frequens[i] != 0){
      printf("R = %i ",dist[i]);
      printf("F = %i \n",Frequens[i]);
    }
  }

  return 0;
}
