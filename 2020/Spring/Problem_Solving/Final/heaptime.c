#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define compare(a, b) (((a.tv_sec) <= (b.tv_sec)) && ((a.tv_nsec) < (b.tv_nsec)))

int printQuery = 0;
struct timespec begin, end;

////////////////////////////
// leetcode skeleton part //
////////////////////////////
typedef struct timespec Time;
typedef struct element {
    int key, value;
    Time access_time;
} Element;

typedef struct LRUCache {
    Element* heap;
    int count, capacity;
} LRUCache;

LRUCache* lRUCacheCreate(int capacity) {
    LRUCache* cache = malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->count = 0;
    cache->heap = malloc(sizeof(Element) * capacity);
    clock_gettime(CLOCK_MONOTONIC, &begin);
    return cache;
}

void swap_element(Element* e1, Element* e2) {
    int key1 = e1->key, key2 = e2->key, value1 = e1->value, value2 = e2->value;
    __time_t t1_sec, t2_sec; __syscall_slong_t t1_nsec, t2_nsec;
    t1_sec = e1->access_time.tv_sec, t1_nsec = e1->access_time.tv_nsec;
    t2_sec = e2->access_time.tv_sec, t2_nsec = e2->access_time.tv_nsec;

    e1->key = key2; e2->key = key1;
    e1->value = value2; e2->value = value1;
    e1->access_time.tv_sec = t2_sec; e2->access_time.tv_sec = t1_sec;
    e1->access_time.tv_nsec = t2_nsec; e2->access_time.tv_nsec = t1_nsec;
}

void heapify_bot_to_top(LRUCache* obj, int index) {
    int parent = (index - 1) / 2;
    if (compare(obj->heap[index].access_time, obj->heap[parent].access_time)) {
        swap_element(&(obj->heap[index]), &(obj->heap[parent]));
        heapify_bot_to_top(obj, parent);
    }
}

void heapify_top_to_bot(LRUCache* obj, int parent) {
    int l = parent * 2 + 1;
    int r = parent * 2 + 2;
    int min;
    if (l >= obj->count || l < 0)
        l = -1;
    if (r >= obj->count || r < 0)
        r = -1;
    if (l != -1 && compare(obj->heap[l].access_time, obj->heap[parent].access_time))
        min = l;
    else
        min = parent;
    if (r != -1 && compare(obj->heap[r].access_time, obj->heap[min].access_time))
        min = r;
    if (min != parent) {
        swap_element(&(obj->heap[min]), &(obj->heap[parent]));
        heapify_top_to_bot(obj, min);
    }
}

void pop_min(LRUCache* obj) {
    obj->heap[0].key = obj->heap[obj->count - 1].key;
    obj->heap[0].value = obj->heap[obj->count - 1].value;
    obj->heap[0].access_time.tv_sec = obj->heap[obj->count - 1].access_time.tv_sec;
    obj->heap[0].access_time.tv_nsec = obj->heap[obj->count - 1].access_time.tv_nsec;
    obj->count--;
    heapify_top_to_bot(obj, 0);
}

void insert(LRUCache* obj, int key, int value) {
    if (obj->count == obj->capacity) {
        pop_min(obj);
    }
    obj->heap[obj->count].key = key;
    obj->heap[obj->count].value = value;
    clock_gettime(CLOCK_MONOTONIC, &obj->heap[obj->count].access_time);
    heapify_bot_to_top(obj, obj->count);
    obj->count++;
}

int find(LRUCache* obj, int key) {
    Element* arr = obj->heap;
    for (int i = obj->count - 1; i >= 0; i--) {
        if (arr[i].key == key) {
            return i;
        }
    }
    return -1;
}

int lRUCacheGet(LRUCache* obj, int key) {
    int index = find(obj, key), value;
    if (index == -1) {
        return -1;
    }
    clock_gettime(CLOCK_MONOTONIC, &obj->heap[index].access_time);
    value = obj->heap[index].value;
    heapify_top_to_bot(obj, index);
    return value;
}

void lRUCachePut(LRUCache* obj, int key, int value) {
    int index = find(obj, key);
    if (index != -1) {
        clock_gettime(CLOCK_MONOTONIC, &obj->heap[index].access_time);
        heapify_top_to_bot(obj, index);
        return;
    }
    insert(obj, key, value);
}

void lRUCacheFree(LRUCache* obj) {
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("%fs\n", (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec) / 1000000000.0);
    free(obj->heap);
    free(obj);
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
        if(printQuery)
					GetLRUCache(key_as_num);
        int value = lRUCacheGet(myCache, key_as_num);

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
