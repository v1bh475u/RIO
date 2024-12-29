#include "rio.h"
#include <fcntl.h>
#include <time.h>
#include <stdio.h>

int main()
{
    // reading char by char
    rio_t rio;
    char c;
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    int fd = open("examples/file", O_RDONLY);
    if (fd == -1)
    {
        printf("Error opening file\n");
        return 1;
    }
    rio_readinitb(&rio, fd);
    while (rio_readnb(&rio, &c, 1) == 1)
    {
        rio_writen(STDOUT_FILENO, &c, 1);
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f\n", cpu_time_used);
    // reading line by line
    start = clock();
    char buf[8192];
    rio_readinitb(&rio, fd);
    while (rio_readlineb(&rio, buf, 8192) > 0)
    {
        rio_writen(STDOUT_FILENO, buf, strlen(buf));
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f\n", cpu_time_used);
    close(fd);
    return 0;
}