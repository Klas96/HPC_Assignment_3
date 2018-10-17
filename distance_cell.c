#include<omp.h>
#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>

// mäta tid
struct timespec start;
#define CHECKPOINT(fmt, ...) { \
struct timespec now; \
timespec_get(&now, TIME_UTC); \
float elapsed = (now.tv_sec + 1.0e-9 * now.tv_nsec) - (start.tv_sec + 1.0e-9 * start.tv_nsec); \
printf("\n%f: ", elapsed); \
printf(fmt, ##__VA_ARGS__); \
}

int freq[3465];

void Comp_and_store(float* X, float* Y) {
  //printf("X = %f %f %f\n",X[0],X[1],X[2]);
  //printf("Y = %f %f %f\n\n",Y[0],Y[1],Y[2]);

  float dist_fun = (X[0]-Y[0])*(X[0]-Y[0]) + (X[1]-Y[1])*(X[1]-Y[1]) + (X[2]-Y[2])*(X[2]-Y[2]);
  int dist_loc = (int)(((sqrt(dist_fun)+0.005)*100));

  freq[dist_loc]++;
};

int main(int argc, char *argv[]) {

  // läser in variabler
timespec_get(&start, TIME_UTC);
CHECKPOINT("Start\n")

  char * endpt;
  int tn = strtol(argv[1]+2, &endpt,10);

  omp_set_num_threads(tn);

  FILE * fp1 = fopen("cell_data/cell_e4","r");
  FILE * fp2 = fopen("cell_data/cell_e4","r");

  //FILE * fp1 = fopen("cell_data/cell_web","r");//Testing
  //FILE * fp2 = fopen("cell_data/cell_web","r");//Testing

  fseek(fp1, 0, SEEK_END);
  long int end_Of_File = ftell(fp1);
  int tot_row = end_Of_File/24;

  fseek(fp1, 0, SEEK_SET);

  int max_load = 10000/tn/sizeof(float); //max data memory
  //int max_load = 40/sizeof(float); //TESTING
  //int lenData1 = 0;

  int max_row = max_load/3;

  float data[max_row][3];

  for(int i = 0; i < 3465; i++){
    freq[i] = 0;
  }

  int chunkNr = (int)tot_row/max_row;
  int restRows = (tot_row % max_row);

  printf("chunkNr = %i,restRows = %i, tot_row = %i, max_row = %i \n",chunkNr,restRows, tot_row, max_row );

  //Lopar över chunks
  for(int v = 0; v < chunkNr; v++){

    fseek(fp2, v*24*max_row, SEEK_SET);

    printf("chunk %i\n",v);
    for(int i = 0; i<max_row; i++){
      fscanf(fp1, "%f %f %f" ,&data[i][0],&data[i][1],&data[i][2]);
      //printf("rad %i (%f, %f ,%f)\n",(v*max_row+i),data[i][0],data[i][1],data[i][2]);
    }

    //loop genom
    //printf("Testar main:\n");
    float y_loc[3];
    for(int k = 0; k<max_row; k++){
      //printf("Data 2: %f %f %f",);
      //printf("Testar: (%f, %f ,%f)\n",data[k][0],data[k][1],data[k][2]);

      fseek(fp2, v*max_row*24, SEEK_SET);
      fseek(fp2, k*24, SEEK_CUR);

      for(int j = v*max_row+k; j < tot_row; j++ ){
        fscanf(fp2, "%f %f %f" ,&y_loc[0],&y_loc[1],&y_loc[2]);
        Comp_and_store(data[k], y_loc);
      }
    }

  }

  printf("Testar Rest:\n");
  float y_loc[3];
  for(int i = 0; i < restRows; i++){

    fscanf(fp1, "%f %f %f" ,&data[i][0],&data[i][1],&data[i][2]);
    fseek(fp2, 0, SEEK_CUR);

    for(int j=0; j < tot_row; j++){
      fscanf(fp2, "%f %f %f" ,&y_loc[0],&y_loc[1],&y_loc[2]);
      Comp_and_store(data[i], y_loc);
    }
  }

  fclose(fp1);
  fclose(fp2);

  for(int i = 0; i < 3464; i++){
    if(freq[i] != 0){
      //printf("%2.2f ",i*0.01);
      //printf("%i \n",freq[i]);
    }
  }

  return 0;

}
