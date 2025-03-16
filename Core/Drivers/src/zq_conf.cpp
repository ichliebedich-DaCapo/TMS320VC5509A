//
// Created by fairy on 2025/3/1 16:11.
//
// 头文件
#include<zq_conf.h>

// 为了兼容GCC
#ifdef __ARM__
extern "C"{
#include <reent.h>
#include <sys/stat.h>

/* Variables */
extern int __io_putchar(int ch) __attribute__((weak));

extern int __io_getchar(void) __attribute__((weak));


/* Functions */
void initialise_monitor_handles()
{
}

 int _getpid(void)
{
    return 1;
}

 int _kill(int pid, int sig)
{
    return -1;
}

void _exit(int status)
{
    while (1) {}
}

__attribute__((weak)) int _read(int file, char *ptr, int len)
{
    return len;
}

__attribute__((weak)) int _write(int file, char *ptr, int len)
{
    return len;
}

int _close(int file)
{
    return -1;
}


int _fstat(int file, struct stat *st)
{
    return 0;
}

int _isatty(int file)
{
    return 1;
}

int _lseek(int file, int ptr, int dir)
{
    return 0;
}

int _open(char *path, int flags, ...)
{
    return -1;
}

int _wait(int *status)
{
    return -1;
}

int _unlink(char *name)
{
    return -1;
}

int _times(struct tms *buf)
{
    return -1;
}

int _stat(char *file, struct stat *st)
{
    return 0;
}

int _link(char *old, char *new_)
{
    return -1;
}

int _fork(void)
{
    return -1;
}

int _execve(char *name, char **argv, char **env)
{
    return -1;
}

void *_sbrk(ptrdiff_t incr)
{

    return (void *)0;
}
}

#endif
