#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int printQuery = 0;
int hash_size, prime;
struct timespec begin, end;

////////////////////////////
// leetcode skeleton part //
////////////////////////////
typedef struct node
{
  struct node *prev, *next;
  int value;
} Node;

typedef struct element
{
  Node *node;
  int key;
} Element;

typedef struct LRUCache
{
  int capacity, cache_count;
  Element *hash;
  Node *front, *end;
} LRUCache;

void getSize(int capacity)
{
  if (capacity <= 2) {
    hash_size = 3;
    prime = 2;
    return;
  }
  int size = 3 * capacity;
  char *prime_arr = malloc(sizeof(char) * (size + 1));
  for (int i = 2; i <= size; i++)
  {
    prime_arr[i] = 1;
  }
  for (int i = 2; i * i <= size; i++)
    if (prime_arr[i])
      for (int j = i * i; j <= size; j += i)
        prime_arr[j] = 0;
  for (int i = capacity - 1; i >= 2; i--)
    if (prime_arr[i])
    {
      prime = i;
      break;
    }
  for (int i = (capacity * 4) / 3; i <= size; i++)
    if (prime_arr[i])
    {
      hash_size = i;
      break;
    }
}

LRUCache *lRUCacheCreate(int capacity)
{
  getSize(capacity);
  LRUCache *cache = malloc(sizeof(LRUCache));
  cache->hash = malloc(hash_size * sizeof(Element));
  for (int i = 0; i < hash_size; i++)
  {
    cache->hash[i].key = -1;
    cache->hash[i].node = NULL;
  }
  cache->front = NULL;
  cache->end = NULL;
  cache->cache_count = 0;
  cache->capacity = capacity;
  clock_gettime(CLOCK_MONOTONIC, &begin);
  return cache;
}

int func1(int key)
{
  int res = key % hash_size;
  if (res < 0) res += hash_size;
  return res;
}

int func2(int key)
{
  int res = key % prime;
  if (res < 0) res += prime;
  return prime - res;
}

int search(Element *hash, int key)
{
  int index1 = func1(key);
  int index2 = func2(key);
  if (hash[index1].key == key) {
    if (!(hash[index1].node->next == NULL && hash[index1].node->prev == NULL)) {
      return index1;
    } else {
      return -1;
    }
  }
  for (int i = 1;; i++)
  {
    int index = (index1 + i * index2) % hash_size;
    if (index < 0) {
      index += hash_size;
    }
    if (index == index1)
      return -1;
    if (hash[index].key == key && !(hash[index].node->next == NULL && hash[index].node->prev == NULL))
      return index;
  }
}

int insert(Element *hash, int key)
{
  int index1 = func1(key);
  int index2 = func2(key);
  if (hash[index1].key == -1 || (hash[index1].node->next == NULL && hash[index1].node->prev == NULL)) {
    return index1;
  }
  for (int i = 1;; i++)
  {
    int index = (index1 + i * index2) % hash_size;
    if (index < 0) {
      index += hash_size;
    }
    if (index == index1)
      return -1;
    if (hash[index].key == -1 || (hash[index].node->next == NULL && hash[index].node->prev == NULL))
      return index;
  }
}

void del_end(LRUCache *obj)
{
  obj->end = obj->end->prev;
  obj->end->next->prev = NULL;
  obj->end->next = NULL;
}

void add_front(LRUCache *obj, Node *node, int key)
{
  int len = obj->cache_count;
  if (len == 0)
  {
    obj->front = obj->end = node;
    node->next = node->prev = NULL;
    obj->cache_count++;
  }
  else
  {
    if (len == obj->capacity)
      del_end(obj);
    else
      obj->cache_count++;
    node->next = obj->front;
    node->prev = NULL;
    obj->front->prev = node;
    obj->front = node;
  }
}

void move_front(LRUCache *obj, Node *node)
{
  if (obj->front == node)
    return;
  if (obj->end == node)
  {
    node->prev->next = NULL;
    obj->end = node->prev;
  }
  else
  {
    node->prev->next = node->next;
    node->next->prev = node->prev;
  }
  node->next = obj->front;
  node->prev = NULL;
  obj->front->prev = node;
  obj->front = node;
}

int lRUCacheGet(LRUCache *obj, int key)
{
  int index;
  if (obj->cache_count == 1) {
    index = func1(key);
    if (obj->hash[index].key != key) {
      index = -1;
    }
  }
  else {
    index = search(obj->hash, key);
  }
  if (index == -1)
    return -1;
  Node *node = obj->hash[index].node;
  move_front(obj, node);
  return node->value;
}

void lRUCachePut(LRUCache *obj, int key, int value)
{
  int index;
  if (obj->cache_count == 1) {
    index = func1(key);
    if (obj->hash[index].key != key) {
      index = -1;
    }
  } else {
    index = search(obj->hash, key);
  }
  if (index == -1)
  {
    index = insert(obj->hash, key);
    if (obj->hash[index].node != NULL)  {
      free(obj->hash[index].node);
    }
    Node *node = malloc(sizeof(Node));
    node->value = value;
    obj->hash[index].key = key;
    obj->hash[index].node = node;
    add_front(obj, node, key);
  }
  else
  {
    Node *node = obj->hash[index].node;
    move_front(obj, node);
  }
}

void lRUCacheFree(LRUCache *obj)
{
  clock_gettime(CLOCK_MONOTONIC, &end);
  printf("%fs\n", (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec) / 1000000000.0);
  free(obj->hash);
  Node *node = obj->front, *temp;
  while (node != NULL)
  {
    temp = node;
    node = node->next;
    free(temp);
  }
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

int main(int argc, char **argv)
{
  if (argc != 2)
    return 0;

  char filename_out[25] = {};
  char filename_kv[25] = {};
  strncpy(filename_out, argv[1], strlen(argv[1]));
  strncpy(filename_kv, argv[1], strlen(argv[1]));
  strcat(filename_out, ".out");
  strcat(filename_kv, ".kv");

  FILE *fop = fopen(filename_out, "r");
  FILE *fkv = fopen(filename_kv, "r");
  int *keys = 0;
  int *values = 0;
  int num_pairs = -1;

  char *operation = (char *)malloc(sizeof(char) * 100);
  LRUCache *myCache;

  while (fgets(operation, 100, fop) != NULL)
  {
    char *op = strtok(operation, " ");
    int func = atoi(op);
    switch (func)
    {
    case 0:
    {
      char *c_size = strtok(NULL, " ");
      myCache = lRUCacheCreate(atoi(c_size));
      // Initialize k,v table
      fscanf(fkv, "%d", &num_pairs);
      keys = malloc(sizeof(int) * num_pairs);
      values = malloc(sizeof(int) * num_pairs);
      for (int i = 0; i < num_pairs; i++)
      {
        fscanf(fkv, "%d %d", &keys[i], &values[i]);
      }
      break;
    }
    case 1:
    {
      char *key = strtok(NULL, " ");
      char *value = strtok(NULL, " ");

      if (printQuery)
        PutLRUCache(atoi(key), atoi(value));

      lRUCachePut(myCache, atoi(key), atoi(value));
      break;
    }
    case 2:
    {
      char *key = strtok(NULL, " ");
      int key_as_num = atoi(key);
      
      int value = lRUCacheGet(myCache, key_as_num);

      if (printQuery)
        GetLRUCache(key_as_num);

      for (int i = 0; i < num_pairs; i++)
      {
        if (keys[i] == key_as_num)
        {
          // Check for correct return. Naive implementation
          if (value != -1 && value != values[i])
          {
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
