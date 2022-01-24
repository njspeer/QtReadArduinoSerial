#include<stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <stdint.h>
#include <inttypes.h>

int main()
{

  /* open file and get size */
  char* fpth = "../data.bin";
  struct stat st;
  stat(fpth, &st);
  long Nst_size    = st.st_size;
  printf("Nst_size = %ld\n", Nst_size);

  /* get the number of 16-bit elements */
  int Nbytes = 2; // number of bytes per element
  int Nel = Nst_size/(Nbytes);

  printf("Nel = %d\n", Nel);

  /* get the number of rows and columns */
  int ncol = 100;
  int nrows = Nel/ncol;

  printf("Nrows = %d\n", nrows);

  /* read-in file as char array */
  char Vchar[Nst_size];
  FILE *fp;
  fp = fopen(fpth,"r");
  for(int i = 0;i < Nst_size; ++i){Vchar[i] = fgetc(fp);}
  fclose(fp);

  /* convert to int16 */
  int16_t Vx[Nel];
  for(int i = 0; i < Nel; ++i)
  {
    Vx[i] = *(int16_t *)(&Vchar[0] + i*Nbytes);
  }
return 0;

  /* write to file */
  FILE *fpnt;
  fpnt = fopen("fout.csv","w+");


  for(int i = 0; i < Nel; ++i)
  {
    if((i+1)%100==0)
    {
      fprintf(fpnt, "%d\n", Vx[i]);
    }
    else
    {
      fprintf(fpnt, "%d,", Vx[i]);
    }
  }
  return 0;
}