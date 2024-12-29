#include "rio.h"

ssize_t rio_readn(int fd, void *usrbuf, size_t size)
{
    size_t left = size;
    ssize_t readn = 0;
    char *bufp = usrbuf;
    while (left > 0)
    {
        if ((readn = read(fd, bufp, left)) < 0)
        {
            if (errno == EINTR)
                readn = 0;
            else
                return -1;
        }
        else if (readn == 0)
            break;
        left -= readn;
        bufp += readn;
    }
    return (size - left);
}

ssize_t rio_writen(int fd, void *usrbuf, size_t size)
{
    size_t left = size;
    ssize_t written = 0;
    char *bufp = usrbuf;
    while (left > 0)
    {
        if ((written = write(fd, usrbuf, left) < 0))
        {
            if (errno == EINTR)
                written = 0;
            else
                return -1;
        }
        left -= written;
        bufp += written;
    }
    return size;
}

void rio_readinitb(rio_t *rp, int fd)
{
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufp = rp->rio_buf;
}

// rio_readlineb and rio_readnb internally use rio_read
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t size)
{
    ssize_t cnt;
    while (rp->rio_cnt <= 0)
    {
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
        if (rp->rio_cnt < 0)
        {
            if (errno != EINTR)
                return -1;
        }
        else if (rp->rio_cnt == 0)
        {
            return 0;
        }
        else
        {
            rp->rio_bufp = rp->rio_buf;
        }
    }
    cnt = ((ssize_t)size < rp->rio_cnt) ? (ssize_t)size : rp->rio_cnt;
    memcpy(usrbuf, rp->rio_bufp, cnt);
    rp->rio_cnt -= cnt;
    rp->rio_bufp += cnt;
    return cnt;
}

ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
    ssize_t n, rcnt;
    char c, *bufptr = usrbuf;
    for (n = 0; n <= (ssize_t)maxlen; n++)
    {
        if ((rcnt = rio_read(rp, &c, 1)) == 1)
        {
            *bufptr++ = c;
            if (c == '\n')
            {
                n++;
                break;
            }
        }
        else if (rcnt == 0)
        {
            if (n == 0)
                return 0;
            else
                break;
        }
        else
        {
            return -1;
        }
    }
    *bufptr = 0;
    return n;
}

ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t size)
{
    size_t left = size;
    ssize_t readn = 0;
    char *bufp = usrbuf;
    while (left > 0)
    {
        if ((readn = rio_read(rp, bufp, left)) < 0)
            return -1;
        else if (readn == 0)
            break;
        left -= readn;
        bufp += readn;
    }
    return (size - left);
}