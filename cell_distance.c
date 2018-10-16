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

  // file pointers
  FILE * fp1 = fopen("cell_data/test","r");
  FILE * fp2 = fopen("cell_data/test","r");

  // allokerar minne till data
  int chunk_sz = 6;//1000/tn/6/sizeof(float); //max data memory per chunk

  //data
  //float* data_p1 = (float *)malloc(max_load/2);
  float data_p1[3*tn];
  float data_p2[chunk_sz*tn];

  //resultat

  int dist_loc;
  int dist[3464];
  int freq[3464];

  for(int i = 0; i < 3464; i++)
    {
      dist[i] = i;
      freq[i] = 0;
    }

  int blocknr = 0;


  while (fscanf(fp1, "%e",data_p1) != EOF)
    {
    //fseek(fp1, blocknr*chunk_sz, SEEK_SET);
    //fseek(fp2, blocknr*chunk_sz, SEEK_SET);
    //fseek(fp2, 0, SEEK_SET);
      blocknr++;

    // läser in data 1
      printf("Läser data 1\n");
      #pragma omp parallel shared(freq)
      for(int i = 0; ; )
        {
          //fseek(fp1,+1,SEEK_CUR);
          if(fscanf(fp1, "%f",data_p1) != EOF)
            {
              fseek(fp1, -15, SEEK_CUR);
        //printf("read data 1");
        //fread(data_p1[i], sizeof(float), 3, fp1);
        //printf("%f %f %f\n",data_p1[i][0],data_p1[i][1],data_p1[i][2]);
                fscanf(fp1, "%f" ,&data_p1[i]);
                fscanf(fp1, "%f" ,&data_p1[i+1]);
                fscanf(fp1, "%f" ,&data_p1[i+2]);
                //printf("%f\n", data_p1[i]);
                printf("%f %f %f\n",data_p1[i+0],data_p1[i+1],data_p1[i+2]);


                for(int j = 0; j<chunk_sz; j+=3)
                  {
          //if(ftell(fp2) != '-1L'){
                    if(fscanf(fp2, "%f", data_p2) != EOF)
                      {
                        printf("Läser data 2\n");
                        fseek(fp2, -7, SEEK_CUR);
            //fread(data_p2[j], sizeof(float), 3, fp1);

                        fscanf(fp2, "%f" ,&data_p2[j]);
                        fscanf(fp2, "%f" ,&data_p2[j+1]);
                        fscanf(fp2, "%f" ,&data_p2[j+2]);

                        printf("%f %f %f\n",data_p2[j],data_p2[j+1],data_p2[j+2]);

                        dist_loc = distence3D(data_p1[i + 0] , data_p1[i + 1], data_p1[i + 2], data_p2[j + 0], data_p2[j+ 1], data_p2[j + 2]);
                        for(int m = 0; m < 3464; m++)
                          {
                            if(dist[m] == dist_loc)
                              {
                                freq[m]++;
                                break;
                              }
                          }
                        }
                      else
                        {
                          break;
                          printf("End of file data 2");
                        }

                    }
              }
            else
              break;
    }
  }
  fclose(fp1);
  fclose(fp2);

  //calculate distances count frequency rouded to 2 didgets

  //Outbut (stdout) sorted list of distance eith freq.

  //Note: never use more that 1 GB memory

  for(int i = 0; i < 3464; i++)
    {
      if(freq[i] != 0)
        {
          printf("%2.2f ",dist[i]/100.0f);
          printf("%i \n",freq[i]);
        }
    }

  return 0;
}
