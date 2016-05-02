#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void die(const char *msg)
{
    perror(msg);
    exit(1);
}

int main()
{
    int shmid;
    key_t key;
    char *shm;

    key = ftok(getenv("HOME"), 42);

    if ((shmid = shmget(key, 6, IPC_CREAT | 0660)) < 0)
        die("shmget");

    if ((shm = shmat(shmid, NULL, 0)) == (void*)-1)
        die("shmat");

    printf("shm=\"%s\", hit enter...", shm); while(getchar()!=10);

    shm[0] = 0;
    printf("shm=\"%s\", exiting...\n", shm);

    if (shmdt(shm))
        die("shmdt");

    return 0;
}
