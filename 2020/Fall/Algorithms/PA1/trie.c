#include <stdio.h>
#include <stdlib.h>

typedef struct ll {
    struct ll* next;
    int index;
} LLNode;

typedef struct trie {
    struct trie* child[62];
    LLNode* head, *tail;
    int isEnd;
} TrieNode;

int char_to_idx(char c) {
    if (c <= '9') return ((int) c - 48);
    else if (c <= 'Z') return ((int) c - 55);
    return ((int) c - 61);
}

void insertLL(TrieNode* trie, int index) {
    LLNode* newNode = (LLNode*) malloc(sizeof(LLNode));
    newNode->index = index;
    newNode->next = NULL;
    if (trie->head == NULL && trie->tail == NULL) {
        trie->head = trie->tail = newNode;
    }
    else {
        trie->tail->next = newNode;
        trie->tail = newNode;
    }
}

void printLL(TrieNode* trie, FILE* output, char* obj[]) {
    LLNode* node = trie->head, *temp;
    while (node != NULL) {
        fputs(obj[node->index], output);
        fputc('\n', output);
        temp = node->next;
        free(node);
        node = temp;
    }
    trie->head = NULL;
}

TrieNode* newTrieNode() {
    TrieNode* newNode = NULL;
    newNode = (TrieNode*) malloc(sizeof(TrieNode));
    if (newNode != NULL) {
        for (int i = 0; i < 62; i++) {
            newNode->child[i] = NULL;
        }
        newNode->head = NULL;
        newNode->tail = NULL;
        newNode->isEnd = 0;
    }
    return newNode;
}

void insertTrie(TrieNode* root, char* word, int len, int index) {
    TrieNode* TrieNode = root;
    for (int i = 0; i < len; i++) {
        int idx = char_to_idx(word[i]);
        if (TrieNode->child[idx] == NULL) TrieNode->child[idx] = newTrieNode();
        TrieNode = TrieNode->child[idx]; 
    }
    TrieNode->isEnd = 1;
    insertLL(TrieNode, index);
}

void printTrie(TrieNode* root, FILE* output, char* obj[]) {
    if (root == NULL) return;

    for (int i = 0; i < 62; i++) {
        if (root->child[i] != NULL) {
            if (root->child[i]->isEnd) {
                printLL(root->child[i], output, obj);
            }
            printTrie(root->child[i], output, obj);
        }
    }
}

void deleteTrie(TrieNode* root) {
    if (root != NULL && root->isEnd == 0) {
        for (int i = 0; i < 62; i++) {
            if (root->child[i] != NULL) {
                deleteTrie(root->child[i]);
                root->child[i] = NULL;
            }
        }
    }

    if (root != NULL) {
        free(root);
    }
}

int compare(char* w1, char* w2) {
    int i;
    for (i = 0; (w1[i] == w2[i]) && w2[i] != '\0'; i++) {}

    char l1 = w1[i], l2 = w2[i];
    if (l1 < l2) 
        return -1;
    else if (l1 > l2)
        return 1;
    return 0;
}

int main(int argc, char* argv[])
{
    //FILE* input = fopen("hw1_input.txt", "r");
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen("hw1_output.txt", "w");

    char** obj, buf[165];
    int n, key, t = 1;

    //read n
    fscanf(input, "%d ", &n);
    fgetc(input); fgetc(input);

    //determine which attribute is the key
    fgets(buf, 165, input);
    for (int i = 0; i <= 160; i++) {
        char temp = buf[i];
        if ((temp == '(') && (buf[i + 1] == 'K') && (buf[i + 2] == 'e') && (buf[i + 3] == 'y') && (buf[i + 4] == ')')) {
            key = t;
        }
        if (temp == ':') {
            t++;
        }
    }

    TrieNode* root = newTrieNode();
    obj = (char**) malloc(sizeof(char*) * n);

    //read object one by one
    fgetc(input); fgetc(input);
    for (int i = 0; i < n; i++) {
        int size = 16 * t + 1, prev_key, curr_key = -1, cnt = 0;
        char* buf = (char*) malloc(size);
        obj[i] = buf;
        fgets(buf, size, input);
        
        for (char* p = buf; *p != '\0'; p++) {
            if (*p == '\n') *p = '\0';
        }

        for (int j = 0; j < size; j++) {
            if (buf[j] == ':' || buf[j] == '\0') {
                prev_key = curr_key;
                curr_key = j;
                cnt++;
            }
            if (key == cnt) {
                int len = curr_key - prev_key - 1;
                insertTrie(root, buf + prev_key + 1, len, i);
                break;
            }
        }
    }

    printTrie(root, output, obj);
    deleteTrie(root);

    for (int i = 0; i < n; i++) {
        free(obj[i]);
    }
    free(obj);

    fclose(input);
    fclose(output);

    return 0;
}