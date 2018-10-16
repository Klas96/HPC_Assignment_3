#include<omp.h>
#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>

//parametrar

int* Frequens;
int* dist;

float y_loc[3];


float data;

//float distence3D(float* x, float* y) {
int distence3D(float* X, float* Y) {
  //float dist = (x[0]-y[0])*(x[0]-y[0]) + (x[1]-y[1])*(x[1]-y[1]) + (x[2]-y[2])*(x[2]-y[2]);
  float dist_fun = (X[0]-Y[0])*(X[0]-Y[0]) + (X[1]-Y[1])*(X[1]-Y[1]) + (X[2]-Y[2])*(X[2]-Y[2]);
  return ((int)((sqrt(dist_fun)+0.005)*100));

};

void Comp_and_store(float* X, float* Y){

  int dist_loc = distence3D(X,Y);

  for(int m = 0; m < 3464; m++) {
    if(dist[m] == dist_loc) {
      Frequens[m]++;
      //printf("Freq++ => %i  dist_loc = %i dist[m] = %i \n",Frequens[m],dist_loc, dist[m]);
      break;
    }
  }
};


int main(int argc, char *argv[]) {

  // läser in variabler
  char * endpt;
  int tn = strtol(argv[1]+2, &endpt,10);
  //export OMP_NUM_THREADS=tn;
  omp_set_num_threads(tn);
  FILE * fp1 = fopen("cell_data/cell_web","r");
  FILE * fp2 = fopen("cell_data/cell_web","r");

  // allokerar minne till data

  int max_load = 30/tn/sizeof(float); //max data memory

  int max_row = max_load/3;

  float data[max_row][3];

  //resultat
  int dist_loc;

  dist = (int*)malloc((3464)*sizeof(int));
  Frequens = (int*)malloc((3464)*sizeof(int));

  for(int i = 0; i < 3464; i++){
    //dist[i] = i*0.01;//Float
    dist[i] = i;//Int
    Frequens[i] = 0;
  }

  int lenData1 = 0;

  char* etc;

  //#pragma omp parallel shared(Frequens) {
  //while(ftell(fp1) != EOF){
  while (fscanf(fp1, "%c",data) != EOF) {
    fseek(fp1, -1, SEEK_CUR);
    //fseek(fp1, 0, SEEK_SET);
    //fseek(fp2, 0, SEEK_SET);
    long int poss = ftell(fp1);

    printf("Poss = %i\n",poss);
    fseek(fp2, poss, SEEK_SET);

    // läser in data 1
    printf("Data 1:\n");
    for(int i = 0; i<max_row; i++) {

      fseek(fp1, +1, SEEK_CUR);
      if(fscanf(fp1, "%c",data) != EOF){
        fseek(fp1, -2, SEEK_CUR);

        fscanf(fp1, "%f" ,&data[i][0]);
        fscanf(fp1, "%f" ,&data[i][1]);
        fscanf(fp1, "%f" ,&data[i][2]);

        printf("(%f, %f ,%f)\n",data[i][0],data[i][1],data[i][2]);

        lenData1 = i+1;
      }else{printf("Klar data 1\n");break;}
    }

    //Lopar över Data 1
    for(int k = 0; k < lenData1; k++) {

      fseek(fp2, poss+1, SEEK_SET);

      //fseek(fp2, k*24+24, SEEK_SET);
      fseek(fp2, k*24+24, SEEK_CUR);

      printf("Data 2: (%f %f %f) k = %i lenData1 = %i\n",data[k][0],data[k][1],data[k][2],k,lenData1);

      while (fscanf(fp2, "%c",data) != EOF) {
        fseek(fp2, -2, SEEK_CUR);

        fscanf(fp2, "%f" ,&y_loc[0]);
        fscanf(fp2, "%f" ,&y_loc[1]);
        fscanf(fp2, "%f" ,&y_loc[2]);

        printf("(%f, %f ,%f)\n",y_loc[0],y_loc[1],y_loc[2]);

        Comp_and_store(data[k], y_loc);

        fseek(fp2, 1, SEEK_CUR);
      }
    }
  }

  fclose(fp1);
  fclose(fp2);

  //printar resultat
  for(int i = 0; i < 3464; i++){
    if(Frequens[i] != 0){
      printf("R = %f ",dist[i]*0.01);
      printf("F = %i \n",Frequens[i]);
    }
  }

  return 0;
}
