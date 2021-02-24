#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int printQuery = 1;

////////////////////////////
// leetcode skeleton part //
////////////////////////////
typedef struct LRUCache {

} LRUCache;

LRUCache* lRUCacheCreate(int capacity) {

}

int lRUCacheGet(LRUCache* obj, int key) {
  
}

void lRUCachePut(LRUCache* obj, int key, int value) {

}

void lRUCacheFree(LRUCache* obj) {

}

///////////////
// Upto here //
///////////////

void GetLRUCache(int key)
{
  printf("GET (%d)\n", key);
}

void PutLRUCache(int key, int value)
{
  printf("PUT (%d,%d)\n", key, value);
}

// Operation type
// 0 size      --> Setting cache size
// 1 key value --> Put (key, value)
// 2 key       --> Get (key)

int main(int argc, char** argv)
{
  if(argc != 2)
    return 0;
  
  char filename_out[25] = {};
  char filename_kv[25] = {};
  strncpy(filename_out, argv[1], strlen(argv[1]));
  strncpy(filename_kv, argv[1], strlen(argv[1]));
  strcat(filename_out, ".out");
  strcat(filename_kv, ".kv");

  FILE* fop = fopen(filename_out, "r");
  FILE* fkv = fopen(filename_kv, "r");
  int* keys = 0;
  int* values = 0;
  int num_pairs = -1;

  char* operation = (char*) malloc(sizeof(char)*100);
  LRUCache* myCache;
  
  while(fgets(operation, 100, fop) != NULL) {
    char* op = strtok(operation, " ");
    int func = atoi(op);
    switch(func) {
      case 0:
      {
        char* c_size = strtok(NULL, " ");
        myCache = lRUCacheCreate(atoi(c_size));
        // Initialize k,v table
        fscanf(fkv, "%d", &num_pairs); 
        keys  = malloc(sizeof(int) * num_pairs);
        values = malloc(sizeof(int) * num_pairs);
        for(int i=0; i<num_pairs; i++) {
          fscanf(fkv, "%d %d", &keys[i], &values[i]); 
        }
        break;
      }
      case 1:
      {
        char* key = strtok(NULL, " ");
        char* value = strtok(NULL, " ");

				if(printQuery)
					PutLRUCache(atoi(key), atoi(value));

        lRUCachePut(myCache, atoi(key), atoi(value));
        break;
      }
      case 2:
      {
        char* key = strtok(NULL, " ");
        int key_as_num = atoi(key);
        int value = lRUCacheGet(myCache, key_as_num);

				if(printQuery)
					GetLRUCache(key_as_num);

        for(int i=0; i<num_pairs; i++) {
          if(keys[i] == key_as_num) {
            // Check for correct return. Naive implementation
            if(value != -1 && value != values[i]) {
              fprintf(stderr, "WRONG ANSWER %d %d %d\n", value, atoi(key), values[i]);
              return 0;
            }
          }
        }

        break;
      }
      default:
        fprintf(stderr, "ERROR: WRONG OP\n");
        return 1;
    }
  }
  fclose(fop);
  fclose(fkv);
  free(keys);
  free(values);
  lRUCacheFree(myCache);
  printf("FINISH\n");
  return 0;
}
