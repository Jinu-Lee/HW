#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct pair {
  int key;
  int value;
} PAIR;

int main(int argc, char** argv)
{
  if(argc != 5) {
    fprintf(stderr, "Usage: <executable name> num_pairs cache_size num_ops output_filename\n");
    return 0;
  }

  srand(time(0));
  int num_pairs, cache_size, num_ops;
  char filename_out[25] = {};
  char filename_kv[25] = {};

  num_pairs = atoi(argv[1]);
  cache_size = atoi(argv[2]);
  num_ops = atoi(argv[3]);
  strncpy(filename_out, argv[4], strlen(argv[4]));
  strncpy(filename_kv, argv[4], strlen(argv[4]));
  strcat(filename_out, ".out");
  strcat(filename_kv, ".kv");

  FILE* fout = fopen(filename_out, "w");
  FILE* fkv= fopen(filename_kv, "w");

  int* keys  = malloc(sizeof(int) * num_pairs);
  int* values = malloc(sizeof(int) * num_pairs);

  int key_num = 0;
  for(int i=0; i<num_pairs; i++) {
    keys[i] = (key_num += rand() % 10 + 1);
    values[i] = rand();
  }

  fprintf(fkv, "%d\n", num_pairs);
  for(int i=0; i<num_pairs; i++) {
    fprintf(fkv, "%d %d\n", keys[i], values[i]);
  }

  int initial_put_size = cache_size * 0.9;
  fprintf(fout, "0 %d\n", cache_size);
  for(int i=0; i<num_ops; i++) {
    int op = rand() % cache_size;
    initial_put_size--;
    if(initial_put_size < cache_size * 0.5)
      initial_put_size = cache_size * 0.5;
    if(op < initial_put_size)
      op = 1;
    else
      op = 2;
    int k = rand() % num_pairs;
    if(op == 1)
      fprintf(fout, "1 %d %d\n", keys[k], values[k]);
    else
      fprintf(fout, "2 %d\n", keys[k]);
  }
  
  free(keys);
  free(values);
  fclose(fout);
  fclose(fkv);
  
}
