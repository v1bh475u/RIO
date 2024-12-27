#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
int main()
{
    // reading char by char
    char c;
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    int fd = open("examples/file", O_RDONLY);
    while (read(fd, &c, 1) == 1)
    {
        write(STDOUT_FILENO, &c, 1);
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f\n", cpu_time_used);
    // reading line by line
    start = clock();
    char buf[8192], *p = buf;
    int n;
    while ((n = read(fd, p, 1)) == 1)
    {
        if (*p == '\n')
        {
            *p = '\0';
            write(STDOUT_FILENO, buf, p - buf + 1);
            p = buf;
        }
        else
        {
            p++;
        }
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f\n", cpu_time_used);
    close(fd);
    return 0;
}