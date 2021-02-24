#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int printQuery = 0;
int count, cache_count, capacity;
struct timespec begin, end;

////////////////////////////
// leetcode skeleton part //
////////////////////////////
typedef struct node
{
    int key, value, count;
    struct node* leftChild;
    struct node* rightChild;
}Node;

Node* find_min_count(Node* root)
{
    if (root->leftChild == NULL && root->rightChild == NULL) {
        return root;
    }
    if (root->leftChild == NULL && root->rightChild != NULL) {
        Node* right = find_min_count(root->rightChild);
        int right_val = right->count, root_val = root->count;
        return (root_val > right_val) ? right : root;
    }
    if (root->leftChild != NULL && root->rightChild == NULL) {
        Node* left = find_min_count(root->leftChild);
        int left_val = left->count, root_val = root->count;
        return (root_val > left_val) ? left : root;
    }
    Node* left = find_min_count(root->leftChild), *right = find_min_count(root->rightChild);
    int left_val = left->count, right_val = right->count, root_val = root->count;
    if (root_val <= left_val && root_val <= right_val) {
        return root;
    }
    if (left_val <= right_val && left_val <= root_val) {
        return left;
    }
    return right;
}

Node* findMinNode(Node* root)
{
    Node* tmp = root;
    while(tmp->leftChild != NULL)
        tmp = tmp->leftChild;
    return tmp;
}

Node* delete(Node* root, int key)
{
    Node* tNode = NULL;
    if(root == NULL)
        return NULL;

    if(root->key > key)
        root->leftChild = delete(root->leftChild, key);
    else if(root->key < key)
        root->rightChild = delete(root->rightChild, key);
    else
    {
        if(root->rightChild != NULL && root->leftChild != NULL)
        {
            tNode = findMinNode(root->rightChild);
            root->key = tNode->key;
            root->count = tNode->count;
            root->value = tNode->value;
            root->rightChild = delete(root->rightChild, tNode->key);
        }
        else
        {
            tNode = (root->leftChild == NULL) ? root->rightChild : root->leftChild;
            free(root);
            return tNode;
        }
    }

    return root;
}

Node* insert(Node* root, int key, int value)
{
    if(root == NULL)
    {
        root = (Node*)malloc(sizeof(Node));
        root->leftChild = root->rightChild = NULL;
        root->key = key;
        root->value = value;
        root->count = count++;
        cache_count++;
        return root;
    }
    else
    {
        if (cache_count == capacity) {
            Node* to_delete = find_min_count(root);
            root = delete(root, to_delete->key);
            cache_count--;
        }
        if(root->key > key)
            root->leftChild = insert(root->leftChild, key, value);
        else
            root->rightChild = insert(root->rightChild, key, value);
    }
    return root;
}

Node* search(Node* root, int key)
{
    if(root == NULL)
        return NULL;

    if(root->key == key)
        return root;
    else if(root->key > key)
        return search(root->leftChild, key);
    else
        return search(root->rightChild, key);
}

void BST_print(Node* root, int* cnt)
{
    if(root == NULL)
        return;

    printf("%d %d\n", (*cnt)++, root->key);
    BST_print(root->leftChild, cnt);
    BST_print(root->rightChild, cnt);
}

typedef struct LRUCache {
    Node* root;
} LRUCache;

LRUCache* lRUCacheCreate(int cap) {
    LRUCache* cache = malloc(sizeof(LRUCache));
    cache->root = NULL;
    count = 1 << 31;
    capacity = cap;
    clock_gettime(CLOCK_MONOTONIC, &begin);
    return cache;
}

int lRUCacheGet(LRUCache* obj, int key) {
    Node* node = search(obj->root, key);
    if (node == NULL) {
        return -1;
    } else {
        node->count = count++;
        return node->value;
    }
}

void lRUCachePut(LRUCache* obj, int key, int value) {
    Node* node = search(obj->root, key);
    if (node != NULL) {
        node->count = count++;
    } else {
        obj->root = insert(obj->root, key, value);
    }
}

void clear(Node *root)
{
	if (root == NULL)
	{
		return;
	}
	clear(root->leftChild);
	clear(root->rightChild);
	free(root);
}

void lRUCacheFree(LRUCache* obj) {
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("%fs\n", (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec) / 1000000000.0);
    clear(obj->root);
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
