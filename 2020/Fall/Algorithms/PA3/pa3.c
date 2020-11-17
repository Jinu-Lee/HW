#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define isOutofIndex(x) (((x) < 0) || ((x) >= tree_size))
#define parent(x)       (((x) - 1) / 2)
#define leftChild(x)    (2 * (x) + 1)
#define rightChild(x)   (2 * (x) + 2)
#define islower(x)      (((x) >= 'a') && ((x) <= 'z'))
#define isupper(x)      (((x) >= 'A') && ((x) <= 'Z'))

typedef struct {
    char* word;
    int cnt;
} Node;

Node tree[256];
int frequency[256];
int n, tree_size;
char table[26][10];

void swap(Node* a, Node* b) {
    Node temp;
    temp.word = a->word;
    temp.cnt = a->cnt;
    a->word = b->word;
    a->cnt = b->cnt;
    b->word = temp.word;
    b->cnt = temp.cnt;
}

void heapify_bot_top(int index) {
    int parent = parent(index);
    if (tree[parent].cnt > tree[index].cnt) {
        swap(&tree[index], &tree[parent]);
        heapify_bot_top(parent);
    }
}

void heapify_top_bot(int parent) {
    int left = leftChild(parent), right = rightChild(parent), min;
    if (isOutofIndex(left)) left = -1;
    if (isOutofIndex(right)) right = -1;

    if (left != -1 && tree[left].cnt < tree[parent].cnt)
        min = left;
    else
        min = parent;

    if (right != -1 && tree[right].cnt < tree[min].cnt)
        min = right;
    
    if (min != parent) {
        swap(&tree[min], &tree[parent]);
        heapify_top_bot(min);
    }
}

void popHeap(Node* min) {
    min->word = tree[0].word;
    min->cnt = tree[0].cnt;
    tree_size--;
    tree[0].word = tree[tree_size].word;
    tree[0].cnt = tree[tree_size].cnt;
    heapify_top_bot(0);
}

void insertHeap(Node* node) {
    tree[tree_size].word = node->word;
    tree[tree_size].cnt = node->cnt;
    heapify_bot_top(tree_size);
    tree_size++;
}

char* concat(Node* n1, Node* n2) {
    int len = strlen(n1->word) + strlen(n2->word) + 4;
    char* new_word = malloc(sizeof(char) * len);
    sprintf(new_word, "(%s,%s)", n1->word, n2->word);
    free(n1->word);
    free(n2->word);
    return new_word;
}

int find(char c, char* result, int pos, int left, int right) {
    int mid, cnt[2] = {0, }, flag = 0;
    char temp;

    for (int i = left + 1; i < right; i++) {
        temp = tree[0].word[i];
        if (temp == '(') {
            cnt[0]++;
            flag = 1;
        } else if (temp == ')') {
            cnt[1]++;
            flag = 1;
        } else if (temp == ',' && !flag) {
            mid = i;
            break;
        }

        if (flag && cnt[0] == cnt[1]) {
            mid = i + 1;
            break;
        }
    }

    if (left + 2 == mid && tree[0].word[left + 1] == c) {
        result[pos] = '0';
        result[pos + 1] = '\0';
        return 1;
    }
    if (mid + 2 == right && tree[0].word[mid + 1] == c) {
        result[pos] = '1';
        result[pos + 1] = '\0';
        return 1;
    }
    if (left + 2 != mid && find(c, result, pos + 1, left + 1, mid - 1)) {
        result[pos] = '0';
        return 1;
    }
    if (mid + 2 != right && find(c, result, pos + 1, mid + 1, right - 1)) {
        result[pos] = '1';
        return 1;
    }
    return 0;
}

void insertTree(char* word, int pos, char ch, int parent) {
    char* temp;
    if (word[pos] == '0') {
        if (word[pos + 1] == '\0') {
            temp = malloc(sizeof(char) * 2);
            temp[0] = ch;
            temp[1] = '\0';
            tree[leftChild(parent)].word = temp;
        }
        else {
            insertTree(word, pos + 1, ch, leftChild(parent));
        }
    }
    else if (word[pos] == '1') {
        if (word[pos + 1] == '\0') {
            temp = malloc(sizeof(char) * 2);
            temp[0] = ch;
            temp[1] = '\0';
            tree[rightChild(parent)].word = temp;
        }
        else {
            insertTree(word, pos + 1, ch, rightChild(parent));
        }
    }
}

int main(int argc, char* argv[])
{
    FILE* input, *output;

    if (strcmp(argv[1], "compress") == 0) {
        char input_file[] = "hw3_input.txt";
        char output_file[] = "hw3_output1.txt";

        input = fopen(input_file, "r");
        output = fopen(output_file, "w");

        char c, *word;
        while ((c = fgetc(input)) != EOF) {
            frequency[c]++;
        }  

        Node temp;
        for (int i = 0; i < 26; i++) {
            frequency['a' + i] += frequency['A' + i];
            if (frequency['a' + i]) {
                n++;
                word = malloc(sizeof(char) * 2);
                word[0] = 'a' + i;
                word[1] = '\0';
                temp.word = word;
                temp.cnt = frequency['a' + i];
                insertHeap(&temp);
            }
        }

        while (tree_size > 1) {
            Node n1, n2;
            popHeap(&n1);
            popHeap(&n2);

            n1.word = concat(&n1, &n2);
            n1.cnt += n2.cnt;

            insertHeap(&n1);
        }

        for (int i = 0; i < 26; i++) {
            if (frequency['a' + i]) {
                find('a' + i, table[i], 0, 0, strlen(tree[0].word) - 1);
            }
        }

        fprintf(output, "%sHEADEREND", tree[0].word);
        free(tree[0].word);

        fseek(input, 0, SEEK_SET);
        while ((c = fgetc(input)) != EOF) {
            if (!islower(c) && !isupper(c))
                continue;
            if (isupper(c)) c = c - 'A' + 'a';
            fprintf(output, "%s", table[c - 'a']);
        }

    } else if (strcmp(argv[1], "uncompress") == 0) {
        char input_file[] = "hw3_output1.txt";
        char output_file[] = "hw3_output2.txt";

        input = fopen(input_file, "r");
        output = fopen(output_file, "w");

        int idx = 0;
        char c, *tree_struct, temp[6];
        while ((c = fgetc(input)) != 'H') {
            frequency[c]++;
            idx++;
        }
        
        fseek(input, 0, SEEK_SET);
        tree_struct = malloc(sizeof(char) * (idx + 1));
        idx = 0;
        while ((c = fgetc(input)) != 'H') {
            tree_struct[idx++] = c;
        }
        tree_struct[idx] = '\0';
        tree[0].word = tree_struct;
        fseek(input, 8, SEEK_CUR);

        for (int i = 0; i < 26; i++) {
            if (frequency['a' + i]) {
                find('a' + i, table[i], 0, 0, strlen(tree[0].word) - 1);
                insertTree(table[i], 0, 'a' + i, 0);
                fprintf(output, "%c:%s\n", 'a' + i, table[i]);
            }
        }
        free(tree_struct);
        tree[0].word = NULL;
        
        idx = 0;
        while ((c = fgetc(input)) != EOF) {
            idx = (c == '0') ? leftChild(idx) : rightChild(idx);
            if (tree[idx].word != NULL) {
                fprintf(output, "%s", tree[idx].word);
                idx = 0;
            }
        }

        for (int i = 0; i < 127; i++) {
            if (tree[i].word != NULL)
                free(tree[i].word);
        }
    }   

    fclose(input);
    fclose(output);

    return 0;
}