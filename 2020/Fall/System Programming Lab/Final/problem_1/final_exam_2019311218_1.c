#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
    int fd1, fd2, retval;
    char temp;

    if ((fd1 = open(argv[1], O_RDONLY)) < 0) exit(1);
    if ((fd2 = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0755)) < 0) exit(2);

    while(read(fd1, &temp, 1) != 0) {
        write(fd2, &temp, 1);
    }

    if ((retval = close(fd1)) < 0) exit(3);
    if ((retval = close(fd2)) < 0) exit(4);

    unlink(argv[1]);

    return 0;
}