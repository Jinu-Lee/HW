#include <unistd.h>
#include <fcntl.h>

int line_number(int num, char* buf) {
    int len = 0;
    char temp;

    while (num) {
        buf[len++] = (num % 10) + '0';
        num /= 10;
    }

    for (int i = 0; i < len / 2; i++) {
        temp = buf[i];
        buf[i] = buf[len - 1 - i];
        buf[len - 1 - i] = temp;
    }

    buf[len++] = ' ';
    buf[len++] = '|';
    buf[len++] = ' ';

    return len;
}

int main() 
{
    char filename1[] = "Aladdin.txt";
    char filename2[] = "Aladdin_num.txt";
    char buf[20];
    
    int fd1, fd2, cnt = 1, retval;
    char temp = 0;

    if ((fd1 = open(filename1, O_RDONLY)) < 0) {
        perror("open");
        exit(1);
    }

    if ((fd2 = open(filename2, O_CREAT | O_WRONLY | O_TRUNC, 0755)) < 0) {
        perror("open");
        exit(1);
    }

    retval = line_number(cnt++, buf);
    write(fd2, buf, retval);

    while(read(fd1, &temp, 1) != 0) {
        write(fd2, &temp, 1);
        if (temp == '\n') {
            retval = line_number(cnt++, buf);
            write(fd2, buf, retval);
        }
    } 

    if ((retval = close(fd1)) < 0) {
        perror("close");
        exit(1);
    }

    if ((retval = close(fd2)) < 0) {
        perror("close");
        exit(1);
    }

    return 0;
}