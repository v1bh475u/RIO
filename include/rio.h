#include <unistd.h>
#include <errno.h>
#include <string.h>
// Direct(unbuffered) transfer of data between memory and file
// returns number of bytes if OK,0 on EOF(only rio_readn) -1 on error
ssize_t rio_readn(int fd, void *usrbuf, size_t size);
ssize_t rio_writen(int fd, void *usrbuf, size_t size);

#define RIO_BUFSIZE 8192

typedef struct
{
    int rio_fd;
    ssize_t rio_cnt;
    char *rio_bufp;
    char rio_buf[RIO_BUFSIZE];
} rio_t;

// Initializes the read buffer by using open fd
void rio_readinitb(rio_t *rp, int fd);

// Returns number of bytes read if OK,0 on EOF, -1 on error
// Both functions use same read buffer
// Readline reads the next text line from file rp( including newline character)
// copies it to usrbuf and terminates it using NULL character
// It can read at most maxlen-1 bytes
// If a line is longer than maxlen, it is truncanted and read
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t size);