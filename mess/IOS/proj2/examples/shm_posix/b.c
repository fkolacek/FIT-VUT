#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

const char *shmname = "/somename";

void die(const char *msg)
{
    perror(msg);
    exit(1);
}

int main()
{
    int *shared;

    int fd = shm_open(shmname, O_RDWR, 0);
    if (fd < 0)
        die("shm_open");

    shared = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE,
            MAP_SHARED, fd, 0);
    if (shared == MAP_FAILED)
        die("mmap");

    printf("*shared = %d, hit enter...", *shared);
    while(getchar()!=10);

    printf("writing 4 to *shared\n");
    *shared = 4;

    printf("*shared = %d, exiting\n", *shared);
    close(fd);
    return 0;
}
