#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#define max(a, b) (((a) > (b)) ? (a) : (b))

int printQuery = 0;
int count, cache_count, capacity;
struct timespec begin, end;

////////////////////////////
// leetcode skeleton part //
////////////////////////////
typedef struct node
{
    int key, value, count, height;
    struct node* child[2];
} Node;
Node dummy = {0, 0, INT_MAX, 0, {&dummy, &dummy}}, *dummy_ptr = &dummy;

Node *new_node(int key, int value)
{
    Node *node = malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    node->count = count++;
    node->height = 1;
    node->child[0] = dummy_ptr;
    node->child[1] = dummy_ptr;
    return node;
}

void set_height(Node *node)
{
    node->height = 1 + max(node->child[0]->height, node->child[1]->height);
}

int get_BF(Node *node)
{
    return node->child[0]->height - node->child[1]->height;
}

Node *rotate(Node **rootp, int dir)
{
    Node *old_r = *rootp, *new_r = old_r->child[dir];

    if (dummy_ptr == (*rootp = new_r))
        free(old_r);
    else
    {
        old_r->child[dir] = new_r->child[!dir];
        set_height(old_r);
        new_r->child[!dir] = old_r;
    }
    return new_r;
}

void adjust_balance(Node **rootp)
{
    Node *root = *rootp;
    int b = get_BF(root) / 2;
    if (b)
    {
        int dir = (1 - b) / 2;
        if (get_BF(root->child[dir]) == -b)
            rotate(&root->child[dir], !dir);
        root = rotate(rootp, dir);
    }
    if (root != dummy_ptr)
        set_height(root);
}

Node *search(Node *root, int key)
{
    return root == dummy_ptr
               ? 0
               : root->key == key
                     ? root
                     : search(root->child[key > root->key], key);
}

void insert(Node **rootp, int key, int value)
{
    Node *root = *rootp;

    if (root == dummy_ptr)
    {
        *rootp = new_node(key, value);
        cache_count++;
    }
    else if (key != root->key)
    { // don't allow dup keys
        insert(&root->child[key > root->key], key, value);
        adjust_balance(rootp);
    }
    else if (key == root->key)
    {
        root->count = count++;
    }
}

void delete (Node **rootp, int key)
{
    Node *root = *rootp;
    if (root == dummy_ptr)
        return; // not found

    // if this is the node we want, rotate until off the tree
    if (root->key == key)
        if (dummy_ptr == (root = rotate(rootp, get_BF(root) < 0)))
            return;

    delete (&root->child[key > root->key], key);
    adjust_balance(rootp);
}

Node* find_min_count_node(Node* root) {
    if (root == dummy_ptr || root == NULL) {
        return dummy_ptr;
    }
    int left = find_min_count_node(root->child[0])->count;
    int right = find_min_count_node(root->child[1])->count;
    Node* min = (left < right) ? root->child[0] : root->child[1];
    if (min == dummy_ptr) {
        return root;
    } else {
        return (root->count < min->count) ? root : min;
    }
}

void clear_node(Node* root) {
    if (root == dummy_ptr || root == NULL) {
        return;
    }
    clear_node(root->child[0]);
    clear_node(root->child[1]);
    free(root);
}

typedef struct LRUCache
{
    Node *root;
} LRUCache;

LRUCache *lRUCacheCreate(int cap)
{
    LRUCache *cache = malloc(sizeof(LRUCache));
    cache->root = dummy_ptr;
    cache_count = 0;
    count = INT_MIN;
    capacity = cap;
    clock_gettime(CLOCK_MONOTONIC, &begin);
    return cache;
}

int lRUCacheGet(LRUCache *obj, int key)
{
    Node* node = search(obj->root, key);
    if (node == NULL) {
        return -1;
    }
    node->count = count++;
    return node->value;
}

void lRUCachePut(LRUCache *obj, int key, int value)
{
    insert(&obj->root, key, value);
    if (cache_count == capacity + 1) {
        Node* del = find_min_count_node(obj->root);
        delete(&obj->root, del->key);
        cache_count--;
    }
}

void lRUCacheFree(LRUCache *obj)
{
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("%fs\n", (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec) / 1000000000.0);
    clear_node(obj->root);
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
