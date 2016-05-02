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

    strcpy(shm, "Hello");

    printf("shm=\"%s\", hit enter...", shm); while(getchar()!=10);

    while (shm[0])
    {
        printf("shm=\"%s\", waiting...\n", shm);
        sleep(1);
    }

    printf("shm=\"%s\", exiting...\n", shm);
    
    if (shmdt(shm))
        die("shmdt");

    if (shmctl(shmid, IPC_RMID, NULL))
        die("shmctl");

    return 0;
}
