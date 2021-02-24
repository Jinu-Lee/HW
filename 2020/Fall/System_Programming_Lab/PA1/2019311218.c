#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

char colon = ':', space = ' ', new_line = '\n';

void print_number(int num) {
    int len = 0;
    char buf[100];
    char temp;

    if (num == 0) {
        temp = '0';
        write(1, &temp, 1);
        return;
    }

    while (num) {
        buf[len++] = (num % 10) + '0';
        num /= 10;
    }

    for (int i = 0; i < len / 2; i++) {
        temp = buf[i];
        buf[i] = buf[len - 1 - i];
        buf[len - 1 - i] = temp;
    }

    write(1, buf, len);
}

void to_upper(char* buffer, int n) {
    for (int i = 0; i < n; i++) {
        if ((buffer[i] >= 'a') && (buffer[i] <= 'z')) {
            buffer[i] -= 32;
        }
    }
}

int check_line(int fd) {
    int n, cnt = 1;
    char buffer[100];

    while ((n = read(fd, buffer, 100)) != 0) {
        for (int i = 0; i < n; i++) {
            if (buffer[i] == '\n') {
                cnt++;
            }
        }
    }
    lseek(fd, 0, SEEK_SET);
    return cnt;
}

int read_word(int fd, char* buffer) {
    int n, len;
    char temp;

    //nothing left
    if ((n = read(fd, buffer, 50)) == 0) {
        return -1;
    }

    for (len = 0; len < n; len++) {
        temp = buffer[len];
        if (temp == ' ' || temp == '\t' || temp == '\n' || temp == -1) {
            break;
        }
    }

    lseek(fd, len - 49, SEEK_CUR);

    return len;
}

int compare(char* w1, char* w2, int n) {
    for (int i = 0; i < n; i++) {
        if (w1[i] != w2[i]) {
            return 0;
        }
    }
    return 1;
}

int check_buffer(char* buffer_ptr, int cnt, char* word, int n2, int* len) {
    int bias = 0;
    for (int i = 0; i < cnt; i++) {
        if (len[i] == n2 && compare(buffer_ptr + bias, word, n2)) {
            return i;
        }
        bias += len[i] + 1;
    }
    return -1;
}

int main(int argc, char* argv[])
{
    char *buffer, *buffer_ptr, word[50], temp;
    int fd, flag, n1, n2, end, line, idx;

    if((fd = open(argv[1], O_RDONLY, 0777)) < 0) {
        exit(1);
    }

    end = check_line(fd);

    while (1)
    {
        buffer = malloc(100);
        n1 = temp = 0;
        while(temp != '\n') {
            read(0, &temp, 1);
            buffer[n1++] = temp;
            if (n1 % 100 == 0) {
                buffer = realloc(buffer, n1 + 100);
            }
        }
        buffer_ptr = buffer; n1--;

        flag = 0;
        if (buffer[0] == '[' && buffer[n1 - 1] == ']') {
            flag = 3;
            buffer_ptr++;
            n1 -= 2;
        }
        else {
            for(int i = 0; i < n1; i++) {
                if (buffer[i] == ' ') {
                    flag = 2;
                    break;
                }
                if (buffer[i] == '*') {
                    flag = 4;
                    break;
                }
            }
            if (flag == 0) {
                flag = 1;
            }
        }
        
        to_upper(buffer_ptr, n1);
        line = 1, idx = 0;
        if (flag == 1) {
            while (1) {
                n2 = read_word(fd, word);
                if ((line == end + 1) || (n2 == -1)) {
                    break;
                }

                if (n2 != 0){
                    to_upper(word, n2);
                    if (n1 == n2 && compare(buffer_ptr, word, n1)) {
                        print_number(line);
                        write(1, &colon, 1);
                        print_number(idx);
                        write(1, &space, 1);
                    }
                    else {
                        idx += n2 + 1;
                    }
                }

                if (word[n2] == '\n') {
                    line++;
                    idx = 0;
                }
            }
        }
        else if (flag == 2) {
            idx = 1;
            for (int i = 0; i < n1; i++) {
                if (buffer_ptr[i] == ' ') {
                    idx++;
                }
            }
            int* flag = calloc(idx, sizeof(int));
            int* len = calloc(idx, sizeof(int));

            temp = idx = 0;
            for (int i = 0; i < n1; i++) {
                if (buffer_ptr[i] == ' ') {
                    len[idx++] = temp;
                    temp = 0;
                }
                else {
                    temp++;
                }
            }
            len[idx++] = temp;
            n1 = idx;

            while (1) {
                n2 = read_word(fd, word);
                if ((line == end + 1) || (n2 == -1)) {
                    break;
                }

                if (n2 != 0) {
                    to_upper(word, n2);
                    idx = check_buffer(buffer_ptr, n1, word, n2, len);
                    if (idx >= 0) {
                        flag[idx] = 1;
                    }
                }

                if (word[n2] == '\n') {
                    temp = 1;
                    for (int i = 0; i < n1; i++) {
                        temp &= flag[i];
                    }
                    if (temp) {
                        print_number(line);
                        write(1, &space, 1);
                    }

                    line++;
                    for (int i = 0; i < n1; i++) {
                        flag[i] = 0;
                    }
                }
            }
        }
        else if (flag == 3) {
            int bias = 0, saved_line, saved_idx;
            flag = 0;
            while (1) {
                n2 = read_word(fd, word);
                if ((line == end + 1) || (n2 == -1)) {
                    break;
                }

                if (n2 != 0) {
                    to_upper(word, n2);
                    if (compare(buffer_ptr + bias, word, n2)) {
                        if (bias == 0) {
                            saved_line = line;
                            saved_idx = idx;
                        }
                        bias += n2 + 1;
                        flag = 1;
                    }
                    else {
                        flag = bias = 0;
                    }
                    idx += n2 + 1;
                }

                if (bias == n1 + 1 && flag == 1) {
                    print_number(saved_line);
                    write(1, &colon, 1);
                    print_number(saved_idx);
                    write(1, &space, 1);
                }

                if (word[n2] == '\n') {
                    line++;
                    idx = 0;
                    flag = bias = 0;
                }
            }
        }
        else {
            int len[2];
            for (int i = 0; i < n1; i++) {
                if (buffer_ptr[i] == '*') {
                    len[0] = i;
                    len[1] = n1 - i - 1;
                    break;
                }
            }

            idx = flag = 0;
            while (1) {
                n2 = read_word(fd, word);
                if ((line == end + 1) || (n2 == -1)) {
                    break;
                }

                if (n2 != 0) {
                    to_upper(word, n2);
                    if (flag == 0) {
                        if (len[0] == n2 && compare(buffer_ptr, word, n2)) {
                            flag = 1;
                        }
                    }
                    else if (flag == 1) {
                        if (len[1] == n2 && compare(buffer_ptr + len[0] + 1, word, n2)) {
                            flag = 2;
                        }
                        else {
                            idx++;
                        }
                    }
                }

                if (word[n2] == '\n') {
                    if (flag == 2 && idx >= 1) {
                        print_number(line);
                        write(1, &space, 1);
                    }
                    line++;
                    idx = flag = 0;
                }
            }
        }
        write(1, &new_line, 1);
        lseek(fd, 0, SEEK_SET);
    }
}