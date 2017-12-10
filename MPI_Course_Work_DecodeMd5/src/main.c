#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>
#include "md5.h"

#define BUFFERSIZE 256

char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
int alphabetSize = sizeof(alphabet) - 1;
char* keyword = NULL;
char* keywordhash = NULL;
int root = 0;

void iteration_brute(int rank, int commsize);
void hash(char *input, char **output);

int main(int argc, char** argv) 
{
  int rankm, commsizem;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rankm);
  MPI_Comm_size(MPI_COMM_WORLD, &commsizem);

  keyword = (char *)malloc(BUFFERSIZE * sizeof(char));
  if (argc > 1) {
    strcpy(keyword, argv[1]);
  } else {
    strcpy(keyword, "keyword");
  }
  hash(keyword,&keywordhash);

  iteration_brute(rankm, commsizem);

  free(keyword);
  free(keywordhash);

  MPI_Finalize();
  return 0;
}

void iteration_brute(int rank, int commsize)
{
  if (!strcmp(keyword, "")) return;
  int lenght = 1;
  //long iterationCount = pow(alphabetSize, lenght);
  long i = 0, j = 0, k = 0;
  //int breakflag = 0;
  char* wordhash = NULL;
  char* word = (char*)malloc(lenght + 1 * sizeof(char));
  word[lenght] = 0;
  int* numsell = (int*)malloc(lenght * sizeof(int));
  
  if (rank == root) {
    printf("Keyword: %s\nKeywordHash: %s\n\n", keyword, keywordhash);
  }

  double time = MPI_Wtime();

  while (1) {
   // breakflag = 0;
    for (i = 0; i < lenght; i++) {
    	  numsell[i] = 0;
    }
    numsell[0] = rank;
    for (i = 0; ; i++) {
      if (numsell[lenght - 1] >= alphabetSize)break;

      for (j = 0; j < lenght - 1; j++) {
        if (numsell[j] >= alphabetSize) 
        { 
	  numsell[j + 1] += numsell[j] / alphabetSize;
          numsell[j] = numsell[j] % alphabetSize;
  	}
      }
      for (j = 0, k = lenght - 1; j < lenght, k >= 0; j++, k--) {
    	word[k] = alphabet[numsell[j]];
      }
//    printf("Word: %s\n", word);
#if 0
  MPI_Recv(NULL, 0, MPI_INT, (rank > 0) ? rank - 1 : MPI_PROC_NULL, 0, MPI_COMM_WORLD,
  MPI_STATUS_IGNORE);  
    printf("Word: %s\n", word);
  MPI_Ssend(NULL, 0, MPI_INT, rank != commsize - 1 ? rank + 1 : MPI_PROC_NULL, 0,
  MPI_COMM_WORLD);
#endif  

      hash(word, &wordhash);
      if (!strcmp(word, keyword))
      {
        printf("Word: %s\nWordHash: %s\nTime: %lf\n", word, wordhash, MPI_Wtime() - time);
        MPI_Abort(MPI_COMM_WORLD,0);
      }
      free(wordhash);
      numsell[0]+=commsize;    
      //usleep(25000);
      //printf("\nword %s\n", word);
    }
    lenght++;
    word = (char*)realloc(word, lenght + 1 * sizeof(char));
    word[lenght] = 0;
    numsell = (int*)realloc(numsell, lenght * sizeof(int));
    //iterationCount = pow(alphabetSize, lenght);
  }

  time = MPI_Wtime() - time;

#if 0
  MPI_Recv(NULL, 0, MPI_INT, (rank > 0) ? rank - 1 : MPI_PROC_NULL, 0, MPI_COMM_WORLD,
  MPI_STATUS_IGNORE);
  printf("Proc %d: %lf\n", rank, time);
  MPI_Ssend(NULL, 0, MPI_INT, rank != commsize - 1 ? rank + 1 : MPI_PROC_NULL, 0,
  MPI_COMM_WORLD);
#endif  
  free(word);
  free(numsell);
  return;
}

void hash(char *input, char **output)
{
  md5_state_t HASH;
  md5_byte_t digest[16];
  char *hex_output = NULL;
  hex_output = malloc((16 * 2 + 1) * sizeof(char));
  int di;

  md5_init(&HASH);
  md5_append(&HASH, (const md5_byte_t *) input, strlen(input));
  md5_finish(&HASH, digest);

  for (di = 0; di < 16; ++di) {
    sprintf(hex_output + di * 2, "%02x", digest[di]);
  }
  *output = hex_output;
}

/*
alfsize -> 10
alf     -> 0 1 2 3 4 5 6 7 8 9


15      ->  1 5

l -> 2


 1 2 9
 1 2 11
 1 3 1


0 1
0 2
0 3
0 9
1 0

*/
