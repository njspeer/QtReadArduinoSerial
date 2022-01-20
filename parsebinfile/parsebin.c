#include<stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <stdint.h>
#include <inttypes.h>

int main()
{

  char* fpth = "../data.bin";
  struct stat st;
  stat(fpth, &st);

  long Nst_size    = st.st_size;

  printf("Nst_size = %ld\n", Nst_size);

  int Nbytes = 4; // number of bytes per element
  int Nel = Nst_size/(8*Nbytes);

  printf("Nel = %d\n", Nel);

  int ncol = 3;
  int nrows = Nel/ncol;

  printf("Nrows = %d\n", nrows);

  int Nchar = Nel*Nbytes;



  char Vchar[Nchar];
  FILE *fp;
  fp = fopen(fpth,"r");
  for(int i = 0;i < Nchar; ++i){Vchar[i] = fgetc(fp);}
  fclose(fp);

  printf("Nchar = %d\n", Nchar);

  int32_t Vx[Nel];
  for(int i = 0; i < Nel; ++i)
  {
    Vx[i] = *(int32_t *)(&Vchar[0] + i*Nbytes);
  }

  FILE *fpnt;
  fpnt = fopen("fout.csv","w+");

  for(int i = 0; i < Nel; ++i)
  {
    fprintf(fpnt, "%ld,", (long)Vx[i]);
    if(i && !((i+1) % ncol))
    {
      fprintf(fpnt, "\n");
    }
  }

  return 0;
}