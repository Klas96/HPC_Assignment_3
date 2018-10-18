
#include<omp.h>
#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
/*
// mäta tid
//struct timespec start;
//#define CHECKPOINT(fmt, ...) { \
struct timespec now; \
timespec_get(&now, TIME_UTC); \
float elapsed = (now.tv_sec + 1.0e-9 * now.tv_nsec) - (start.tv_sec + 1.0e-9 * start.tv_nsec); \
printf("\n%f: ", elapsed); \
printf(fmt, ##__VA_ARGS__); \
}
*/
int freq[3465];

void Comp_and_store(float* X, float* Y) {

  float dist_fun = (X[0]-Y[0])*(X[0]-Y[0]) + (X[1]-Y[1])*(X[1]-Y[1]) + (X[2]-Y[2])*(X[2]-Y[2]);
  int dist_loc = (int)(((sqrt(dist_fun)+0.005)*100));

  freq[dist_loc]++;
};



int main(int argc, char *argv[]) {

  // läser in variabler
//timespec_get(&start, TIME_UTC);
//CHECKPOINT("Start\n")

  char * endpt;
  int tn = strtol(argv[1]+2, &endpt,10);


  //export OMP_NUM_THREADS = tn;
  omp_set_num_threads(tn);


  //  FILE * fp1 = fopen("cell_data/cell_e5","r");
  //  FILE * fp2 = fopen("cell_data/cell_e5","r");

  //FILE * fp1 = fopen("cell_data/cell_e4","r");
  //FILE * fp2 = fopen("cell_data/cell_e4","r");

  FILE * fp1 = fopen("cell_data/cell_e5","r");
  FILE * fp2 = fopen("cell_data/cell_e5","r");


  //FILE * fp1 = fopen("cell_data/cell_web","r");//Testing
  //FILE * fp2 = fopen("cell_data/cell_web","r");//Testing

  fseek(fp1, 0, SEEK_END);
  long int end_Of_File = ftell(fp1);
  int tot_row = end_Of_File/24;

  fseek(fp1, 0, SEEK_SET);

  int max_load = 500000/tn/sizeof(float); //max data memory
  //int max_load = 40/sizeof(float); //TESTING
  //int lenData1 = 0;

  int chunkSize = max_load/6;

  //int chunkSize_ind = chunkSize/2;

  float data1[chunkSize][3];
  float data2[chunkSize][3];

  //float data1[chunkSize_ind][3];
  //float data2[chunkSize_ind][3];


  for(int i = 0; i < 3465; i++){
    freq[i] = 0;
  }

  int chunkNr = (int)tot_row/chunkSize;
  int restRows = (tot_row % chunkSize);

  int rows1;
  int rows2;

  printf("chunkNr = %i,restRows = %i, tot_row = %i, chunkSize = %i \n",chunkNr,restRows, tot_row, chunkSize );

  //Lopar över chunks
  #pragma omp parallel for reduction(+:freq)
  for(int v = 0; v < chunkNr+1; v++){

    int nx = omp_get_thread_num();
    printf("Thread %i: I take row \n",nx);

    if(v<chunkNr){
      rows1 = chunkSize;
    }else{
      rows1 = restRows;
    }


    //Läser Chunk 1
    //CHECKPOINT("chunk %i\n",v);
    for(int i = 0; i<rows1; i++){

      fscanf(fp1, "%f %f %f" ,&data1[i][0],&data1[i][1],&data1[i][2]);
      //printf("rad %i (%f, %f ,%f)\n",(v*chunkSize+i),data[i][0],data[i][1],data[i][2]);
    }

    fseek(fp2, v*24*chunkSize, SEEK_SET);

    //Jmf Chunk 1 med alla chunks
    for(int j = v; j <  chunkNr+1; j++){
      if(j<chunkNr){
        rows2 = chunkSize;
      }else{
        rows2 = restRows;
      }

      //Läser Chunk 2
      for(int i = 0; i<rows2; i++){
        fscanf(fp2, "%f %f %f" ,&data2[i][0],&data2[i][1],&data2[i][2]);
      }

      //compare Chunk1 och Chunk2
      for(int k = 0; k<rows1; k++){
        for(int l = k; l < rows2; l++){
          Comp_and_store(data1[k],data2[l]);

        }
      }

    }
  }

  fclose(fp1);
  fclose(fp2);

//CHECKPOINT("Printar resultat \n");
  for(int i = 0; i < 3464; i++){
    if(freq[i] != 0){
      printf("%2.2f ",i*0.01);
      printf("%i \n",freq[i]);
    }
  }
//CHECKPOINT("Slut\n");
  return 0;

}
