#include<omp.h>
#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>

//float distence3D(float* x, float* y) {
float distence3D(float x1, float x2, float x3, float y1, float y2, float y3) {
  //int dist = (x[0]-y[0])*(x[0]-y[0]) + (x[1]-y[1])*(x[1]-y[1]) + (x[2]-y[2])*(x[2]-y[2]);
  int dist = (x1-y1)*(x1-y1) + (x2-y2)*(x2-y2) + (x3-y3)*(x3-y3);
  return sqrt(dist);
  //Avrunda??
}

int main(int argc, char *argv[]) {

  // läser in variabler
  char * endpt;
  int tn = strtol(argv[1]+2, &endpt,10);
  printf("Kör med %i threads \n",tn);

  // hittar antal rader indatan är
  FILE * fp = fopen("cell_data/cell_e2","r");
  fseek(fp, 0L, SEEK_END);
  int sz = ftell(fp);
  fseek(fp, 0L, SEEK_SET);
  int rows = sz/24;
  printf("Filstorlek: %i på %i rader\n",sz, rows);

  // allokerar minne till data

  //float** data_pp = (float **)malloc(rows);
  float* data_p = (float *)malloc(rows*sizeof(float)*3);//tidigare

  printf("ROWS = %i \n",rows);

  //for ( size_t ix = 0, jx = 0; ix < rows; ++ix, jx+=3 ){
    //printf("ix = %zu \n",ix ,jx);
    //data_pp[ix] = data_p + jx;
  //}

  // läser in data
  for(size_t ix = 0; ix<rows*3; ix++){
        fscanf(fp, "%f" ,&data_p[ix]);
        //printf("%f \n", data[ix]);
    }

  fclose(fp);

  //printf("%f",data_pp[2][2]);

  //calculate distances count frequency rouded to 2 didgets

  float * distance = (float *)malloc(rows*rows*sizeof(float));

  for(int i = 0; i < rows; i++){
    //dont want to comapre with it self therfor j=(i+1)
    //X = {data_p+i*3 + 0, data_p+i*3 + 1, data_p+i*3 + 2};
    for(int j = (i+1); j < rows; j++){
      //Y = {data_p+j*3 + 0, data_p+j*3 + 1, data_p+j*3 + 2};
      //distance[i] = distence3D(data[i],data[j]);
      distance[i+j-1] = distence3D(data_p[i*3 + 0] , data_p[i*3 + 1], data_p[i*3 + 2], data_p[j*3 + 0], data_p[j*3 + 1], data_p[j*3 + 2]);
    }
  }

  //printf("x1 = %f, x2 = %f, x3 = %f\n",data_p[0],data_p[1],data_p[2]);
  //printf("x1 = %f, x2 = %f, x3 = %f\n",data_p[3],data_p[4],data_p[5]);

  //float test = distence3D(data_p[0] , data_p[1], data_p[2], data_p[3], data_p[4], data_p[5]);

  //printf("Avstånd de första punkterna: %f \n",test);
  //printf("Avstånd de första punkterna: %f \n",distance[0]);

  int* dist = (float *)malloc(rows*rows*sizeof(float));
  for(i=0; i<row*row; i++){

  }
  int* Frequens;

  for(int i = 0; i < rows; i++){
    for(int j = 0; j < rows; j++){
      //Dont want to comapre
      //if in list increase freq
      if(dist[j] == distance[i]){
        Frequens[j]++;
        break;
      }
      //else append to bottom of list
      //Can se if we got to bottom by checking for null or something
      if(dist[j] == -1){
        dist[j] = distance[i];
        Frequens[j] = 1;
      }
    }
  }

  //Outbut (stdout) sorted list of distance eith freq.

  //Note: never use more that 1 GB memory

  return 0;
}
