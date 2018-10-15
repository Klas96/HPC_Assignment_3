#include<omp.h>
#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>

//float distence3D(float* x, float* y) {
int distence3D(float x1, float x2, float x3, float y1, float y2, float y3) {
  //float dist = (x[0]-y[0])*(x[0]-y[0]) + (x[1]-y[1])*(x[1]-y[1]) + (x[2]-y[2])*(x[2]-y[2]);
  float dist = (x1-y1)*(x1-y1) + (x2-y2)*(x2-y2) + (x3-y3)*(x3-y3);
  return ((int)(sqrt(dist)*100));
};

int main(int argc, char *argv[]) {

  // läser in variabler
  char * endpt;
  int tn = strtol(argv[1]+2, &endpt,10);
  //export OMP_NUM_THREADS=tn;
  omp_set_num_threads(tn);

  FILE * fp1 = fopen("cell_data/cell_e2","r");
  FILE * fp2 = fopen("cell_data/cell_e2","r");

  // allokerar minne till data

  int max_load = 100000/tn/sizeof(float); //max data memory


  //data
  //float* data_p1 = (float *)malloc(max_load/2);
  float data_p1[max_load/2];
  float data_p2[max_load/2];

  //resultat
  int dist_loc;
  int* dist = (int*)malloc((3464)*sizeof(int));
  int* Frequens = (int*)malloc((3464)*sizeof(int));

  for(int i = 0; i < 3464; i++){
    //dist[i] = i*0.01;//Float
    dist[i] = i;//Int
    Frequens[i] = 0;
  }

  int blocknr = 0;

  char* etc;

  //while(ftell(fp1) != EOF){
  while (fscanf(fp1, "%e",data_p1) != EOF) {
    //printf("Börjar med block %i \n", blocknr);
    fseek(fp1, 0, SEEK_SET);
    fseek(fp2, 0, SEEK_SET);

    blocknr++;
    // läser in data 1
    printf("Läser data 1\n");
    for(int i = 0; i<max_load/2; i+=3) {

      //if(ftell(fp1) != '-1L'){
      if(fscanf(fp1, "%c",data_p1) != EOF){
        fseek(fp1, -1, SEEK_CUR);
        //printf("read data 1");
        //fread(data_p1[i], sizeof(float), 3, fp1);
        //printf("%f %f %f\n",data_p1[i][0],data_p1[i][1],data_p1[i][2]);
        fscanf(fp1, "%f" ,&data_p1[i]);
        fscanf(fp1, "%f" ,&data_p1[i+1]);
        fscanf(fp1, "%f" ,&data_p1[i+2]);
        //printf("%f\n", data_p1[i]);
        printf("%f %f %f\n",data_p1[i+0],data_p1[i+1],data_p1[i+2]);

      }else{break;}
    }

    //läser in data 2
    //fseek(fp2, 0, SEEK_SET + blocknr*max_load/(sizeof(float)*2));
    printf("Läser data 2\n");
    for(int j = 0; j<max_load/(sizeof(float)*2); j+=3) {
      //if(ftell(fp2) != '-1L'){
      if(fscanf(fp2, "%c", data_p2) != EOF){
        fseek(fp2, -1, SEEK_CUR);
        //fread(data_p2[j], sizeof(float), 3, fp1);

        fscanf(fp2, "%f" ,&data_p2[j]);
        fscanf(fp2, "%f" ,&data_p2[j+1]);
        fscanf(fp2, "%f" ,&data_p2[j+2]);

        printf("%f %f %f\n",data_p2[j],data_p2[j+1],data_p2[j+2]);
      }else{break;printf("End of file data 2");}
    }

    //jämför data 1 o data 2
    #pragma omp parallel shared(Frequens)
    {
    //for(int k = 0; k < max_load/(sizeof(float)*2); k+=3) {
    for(int k = 0; k < max_load/(sizeof(float)*2); k++) {

      //for(int l = (k+1); l < max_load/(sizeof(float)*2); l+=3) {
      for(int l = (k+1); l < max_load/(sizeof(float)*2); l++) {

        //printf("X = (%f,%f,%f)\n Y = (%f,%f,%f)\n", data_p1[k*3 + 0] , data_p1[k*3 + 1], data_p1[k*3 + 2], data_p2[l*3 + 0], data_p2[l*3 + 1], data_p2[l*3 + 2]);

        dist_loc = distence3D(data_p1[k*3 + 0] , data_p1[k*3 + 1], data_p1[k*3 + 2], data_p2[l*3 + 0], data_p2[l*3 + 1], data_p2[l*3 + 2]);

        //dist_loc = distence3D(data_p1[k],data_p1[l]);

        //printf("dist_loc = %i \n",dist_loc);

        //Jämför dist_loc och ökar den frekvensen
        for(int m = 0; m < 3464; m++) {
          if(dist[m] == dist_loc) {
            Frequens[m]++;
            break;
          }
        }
      }
    }
    }
  }

  fclose(fp1);
  fclose(fp2);

  //calculate distances count frequency rouded to 2 didgets

  //Outbut (stdout) sorted list of distance eith freq.

  //Note: never use more that 1 GB memory

  for(int i = 0; i < 3464; i++){
    if(Frequens[i] != 0){
      //printf("R = %f ",dist[i]*0.01);
      //printf("F = %i \n",Frequens[i]);
    }
  }

  return 0;
}
