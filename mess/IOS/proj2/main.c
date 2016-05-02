/**
 * @file		readerWriter.c
 * @brief		Problem synchronizace ctenar - pisar
 * 				(projekt c. 2, predmet IOS)
 *
 * @date		2012/04/20
 * @version		0.8a
 * @author		Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

#define UNUSED(p){(p) = (p);}

/** @info: Enumeration of possible errors */
enum errorCodes{
    E_OK = 0,
    E_PARAM,
    E_CONVERSION,
    E_OUTPUT,
    E_FORK,
    E_ABORT,

    E_SHM_FTOK,
    E_SHM_GET,
    E_SHM_AT,
    E_SHM_DT,
    E_SHM_CTL

};

/** @info Array of error messages */
const char* ERROR_MESSAGES[] = {
    [E_PARAM]       = "Invalid parametres",
    [E_CONVERSION]  = "Invalid conversion",
    [E_OUTPUT]      = "Cannot set output",
    [E_FORK]        = "Cannot fork",
    [E_ABORT]       = "Aborting..",

    [E_SHM_FTOK]    = "Failed function ftok",
    [E_SHM_GET]     = "Failed function shmget",
    [E_SHM_AT]      = "Failed function shmat",
    [E_SHM_DT]      = "Failed function shmdt",
    [E_SHM_CTL]     = "Failed function shmctl"
};

/** @info: Structure for storage parametres */
typedef struct params{
    int error;

    int writers;
    int readers;
    int cycles;

    int intervalWriters;
    int intervalReaders;

    char* outputFile;
} PARAMS;

/** @info: Structure for storage resources */
typedef struct resources{
    int error;

    pid_t master;

    FILE* output;

    int shmId;
    int* actionCounter;
    int* workSpace;

    int* activeWriters;
    int* activeReaders;

    int shmSemId;
    sem_t* lockWrite;
    sem_t* lockRead;
    sem_t* lockActiveWriters;
    sem_t* lockActiveReaders;
    sem_t* lockAtomic;
    sem_t* lockOutput;

} RESOURCES;


/** @info Global container of pointers */
RESOURCES RESOURCE = {
    .error = E_OK,

    .master = 0,

    .output = NULL,

    .shmId = -1,
    .actionCounter = NULL,
    .workSpace = NULL,
    .activeWriters = NULL,
    .activeReaders = NULL,

    .shmSemId = -1,
    .lockWrite = NULL,
    .lockRead = NULL,
    .lockActiveWriters = NULL,
    .lockActiveReaders = NULL,
    .lockAtomic = NULL
};

/**
 * @info	load params from command line
 * @argv	int - count of parametres
 * @argv	char*[] - parametres
 * @return	PARAMS
 */
PARAMS loadParams(int, char*[]);

/**
 * @info	load resources
 * @argv	PARAMS - structure with parametres
 * @return	RESOURCES
 */
RESOURCES loadResources(PARAMS);

/**
 * @info free all loaded and mapped resources
 * @argv RESOURCE - structure with pointers
 */
void freeResources(RESOURCES);

/**
 * @info main function for writers
 * @argv int - unique id of writer
 * @argv PARAMS - structure with parametres
 */
void processWriter(int, PARAMS);

/**
 * @info main function for readers
 * @argv int - unique id of reader
 * @argv PARAMS - structure with parametres
 */
void processReader(int, PARAMS);

void programAbort(int);

/**
 * @info	parses integer
 * @argv	char* - input string
 * @argv	int* - error flag
 * @return	int
 */
int parseInt(char*, int*);

/**
 * @info	printError to stderr
 * @argv	int - errorCode
 */
void printError(int);

/**
 * @info	printStatus to handler
 * @argv	char*[] - format
 */
void printStatus(char*, ...);

int main(int argc, char* argv[]){
    //signal handling
    signal(SIGTERM, programAbort);
    signal(SIGINT, programAbort);

    //loading params
    PARAMS param = loadParams(argc, argv);

    if(param.error != E_OK){
        printError(param.error);
        return param.error;
    }

    //loading resources
    RESOURCE = loadResources(param);

    if(RESOURCE.error != E_OK){
        printError(RESOURCE.error);
        freeResources(RESOURCE);

        return RESOURCE.error;
    }

    //set random seed
    srand(time(NULL));

    int iterator;
    pid_t processId = -1;
    pid_t readers[param.readers];
    pid_t writers[param.writers];

    //Creating writers
    for(iterator = 0; iterator < param.writers; iterator++){
        //printStatus("#M creating writer: %d\n", iterator+1);

        processId = fork();

        //Fork error
        if(processId == -1){
            printError(E_FORK);

            //Kill all remaining children
            for(int i = 0; i < iterator; i++)
                kill(writers[i], SIGTERM);

            //Wait for them
            for(int i = 0; i < iterator; i++)
                waitpid(writers[i], NULL, 0);

            //programAbort(SIGTERM);
            freeResources(RESOURCE);

            return E_FORK;
        }
        //Created writer
        else if(processId == 0) processWriter(iterator+1, param);
        //Main process
        else writers[iterator] = processId;
    }

    //Creating readers
    for(iterator = 0; iterator < param.readers; iterator++){
        //printStatus("#M creating reader: %d\n", iterator+1);

        processId = fork();

        //Fork error
        if(processId == -1){
            printError(E_FORK);

            //Kill all remaining children
            for(int i = 0; i < param.writers; i++)
                kill(writers[i], SIGTERM);

            for(int i = 0; i < iterator; i++)
                kill(readers[i], SIGTERM);

            //Wait for them
            for(int i = 0; i < param.writers; i++)
                waitpid(writers[i], NULL, 0);

            for(int i = 0; i < iterator; i++)
                waitpid(readers[i], NULL, 0);

            //programAbort(SIGTERM);
            freeResources(RESOURCE);

            return E_FORK;
        }
        //Created reader
        else if(processId == 0) processReader(iterator+1, param);
        //Main process
        else readers[iterator] = processId;
    }

    //All children successfully created! Congratulation!

    //Waiting for all writers
    for(iterator = 0; iterator < param.writers; iterator++)
        waitpid(writers[iterator], NULL, 0);

    //LOCK
    sem_wait(RESOURCE.lockRead);
        //printStatus("#M Setted 0 to workSpace\n");
        *(RESOURCE.workSpace) = 0;
    sem_post(RESOURCE.lockRead);
    //UNLOCK

    //Waiting for all readers
    for(iterator = 0; iterator < param.readers; iterator++)
        waitpid(readers[iterator], NULL, 0);

    //Free all loaded resources
    freeResources(RESOURCE);

    return RESOURCE.error;
}

PARAMS loadParams(int argc, char* argv[]){
    PARAMS param = { .error = E_OK};

    if(argc == 7){
        int flag = E_OK;

        //Parse writers
        param.writers = parseInt(argv[1], &flag);
        if(flag != E_OK){ param.error = E_CONVERSION; return param; }

        //Parse readers
		param.readers = parseInt(argv[2], &flag);
		if(flag != E_OK){ param.error = E_CONVERSION; return param; }

        //Parse cycles
		param.cycles = parseInt(argv[3], &flag);
		if(flag != E_OK){ param.error = E_CONVERSION; return param; }

        //Parse intervalWriters
		param.intervalWriters = parseInt(argv[4], &flag);
		if(flag != E_OK){ param.error = E_CONVERSION; return param; }

        //Parse intervalReaders
		param.intervalReaders = parseInt(argv[5], &flag);
		if(flag != E_OK){ param.error = E_CONVERSION; return param; }

        //Setting output file name
        param.outputFile = argv[6];
    }
    else
        param.error = E_PARAM;

    if(param.readers < 0 || param.writers < 0 || param.cycles < 0 || param.intervalWriters < 0 || param.intervalReaders < 0)
        param.error = E_PARAM;

    return param;
}

RESOURCES loadResources(PARAMS param){
    RESOURCES resource = {
		.error = E_OK,

		.master = 0,

		.output = NULL,

		.shmId = -1,
		.actionCounter = NULL,
		.workSpace = NULL,
		.activeWriters = NULL,
		.activeReaders = NULL,

		.shmSemId = -1,
		.lockWrite = NULL,
		.lockRead = NULL,
		.lockActiveWriters = NULL,
		.lockActiveReaders = NULL,
		.lockAtomic = NULL,
		.lockOutput = NULL
	};

	resource.master = getpid();

    //Loading output

    //Stdout
    if(strcmp(param.outputFile, "-") == 0)
        resource.output = stdout;
    //Regular file
    else{
        resource.output = fopen(param.outputFile, "w");

        if(!resource.output){
            resource.error = E_OUTPUT;
            return resource;
        }

        setbuf(resource.output, NULL);
    }

	//Generate unique key
	//Answer to the Ultimate Question of Life, the Universe, and Everything
	key_t key = ftok(getenv("HOME"), 42);
	key_t keySem = ftok(getenv("HOME"), 24);

	if(key == -1 || keySem == -1){ resource.error = E_SHM_FTOK; return resource; }

    //Get ID of shared memmory
	resource.shmId = shmget(key, 4 * sizeof(int*), IPC_CREAT | 0660);
	resource.shmSemId = shmget(keySem, 6 * sizeof(sem_t*), IPC_CREAT | 0660);

    //Shmget failed - cannot get share memmory id
    if(resource.shmId < 0 || resource.shmSemId < 0){ resource.error = E_SHM_GET; return resource; }

    //Attach shared memmory
    resource.workSpace = shmat(resource.shmId, NULL, 0);
    resource.lockWrite = shmat(resource.shmSemId, NULL, 0);

    //Shmat failed - cannot atach shared memmory
    if( (resource.workSpace == (void*)-1) || (resource.lockWrite == (void*)-1) ){
        resource.workSpace = NULL;
        resource.lockWrite = NULL;

        //Free shared memmory
        if(shmctl(resource.shmId, IPC_RMID, NULL) || shmctl(resource.shmSemId, IPC_RMID, NULL)){
            resource.error = E_SHM_CTL;
            return resource;
        }

        resource.shmId = -1;
        resource.shmSemId = -1;
        resource.error = E_SHM_AT;

        return resource;
    }

    //Attach shared actionCounter, activeWriters, activeReaders
    resource.actionCounter = resource.workSpace + sizeof(int*);
    resource.activeWriters = resource.workSpace + (2 * sizeof(int*));
    resource.activeReaders = resource.workSpace + (3 * sizeof(int*));

    //Attach shared lockRead, lockActiveWriters, lockActiveReaders
    resource.lockRead = resource.lockWrite + sizeof(sem_t*);
    resource.lockActiveWriters = resource.lockWrite + (2 * sizeof(sem_t*));
    resource.lockActiveReaders = resource.lockWrite + (3 * sizeof(sem_t*));
    resource.lockAtomic = resource.lockWrite + (4 * sizeof(sem_t*));
    resource.lockOutput = resource.lockWrite + (5 * sizeof(sem_t*));

    //Set default value of workSpace, actionCounter, activeWriters/activeReaders
    *(resource.workSpace) = -1;
    *(resource.actionCounter) = 1;
    *(resource.activeWriters) = 0;
    *(resource.activeReaders) = 0;

    //Init semaphores
    sem_init(resource.lockWrite, 1, 1);
    sem_init(resource.lockRead, 1, 1);
    sem_init(resource.lockActiveWriters, 1, 1);
    sem_init(resource.lockActiveReaders, 1, 1);
    sem_init(resource.lockAtomic, 1, 1);
    sem_init(resource.lockOutput, 1, 1);

    return resource;
}

void freeResources(RESOURCES resource){
    //Free output
    if(resource.output && resource.output != stdout){
        fclose(resource.output);

        resource.output = NULL;
    }

    //Free shared memmory
    if(resource.workSpace){
        if(shmdt(resource.workSpace))
            resource.error = E_SHM_DT;

        if(shmctl(resource.shmId, IPC_RMID, NULL))
            resource.error = E_SHM_CTL;

        resource.shmId = -1;
        resource.workSpace = NULL;
        resource.actionCounter = NULL;
        resource.activeWriters = NULL;
        resource.activeReaders = NULL;
    }

    if(resource.lockWrite){
        //Only master can do this
        if(getpid() == resource.master){
            sem_destroy(RESOURCE.lockWrite);
            sem_destroy(RESOURCE.lockRead);
            sem_destroy(RESOURCE.lockActiveWriters);
            sem_destroy(RESOURCE.lockActiveReaders);
            sem_destroy(RESOURCE.lockAtomic);
            sem_destroy(RESOURCE.lockOutput);
        }

        if(shmdt(resource.lockWrite))
            resource.error = E_SHM_DT;

        if(shmctl(resource.shmSemId, IPC_RMID, NULL))
            resource.error = E_SHM_CTL;

        resource.shmSemId = -1;
        resource.lockWrite = NULL;
        resource.lockRead = NULL;
        resource.lockActiveWriters = NULL;
        resource.lockActiveReaders = NULL;
        resource.lockAtomic = NULL;
        resource.lockOutput = NULL;
    }

}

void processWriter(int id, PARAMS param){
    //printStatus("#W: %d ready\n", id);

    int interval;
    for(int i = 0; i < param.cycles; i++){
        interval = ((param.intervalWriters == 0)? 0 : (rand()%(param.intervalWriters + 1)));

        sem_wait(RESOURCE.lockActiveWriters);
            (*RESOURCE.activeWriters)++;

            //printStatus("\x1b[31;1mA: writer: %d: new value\x1b[0m\n", id);
            printStatus("writer: %d: new value\n", id);
            usleep(interval * 1000);
            //printStatus("\x1b[31;1mA: writer: %d: ready\x1b[0m\n", id);
            printStatus("writer: %d: ready\n", id);

            if(*(RESOURCE.activeWriters) == 1)
                sem_wait(RESOURCE.lockRead);

        sem_post(RESOURCE.lockActiveWriters);

        //Lock writing
        sem_wait(RESOURCE.lockWrite);

            //printStatus("\x1b[31;1mA: writer: %d: writes a value\x1b[0m\n", id);
            printStatus("writer: %d: writes a value\n", id);
            *(RESOURCE.workSpace) = id;
            //printStatus("\x1b[31;1mA: writer: %d: written\x1b[0m\n", id);
            printStatus("writer: %d: written\n", id);

        sem_post(RESOURCE.lockWrite);
        //Unlock writing

        //Lock
        sem_wait(RESOURCE.lockActiveWriters);
            (*RESOURCE.activeWriters)--;

            if(*(RESOURCE.activeWriters) == 0)
                sem_post(RESOURCE.lockRead);

        sem_post(RESOURCE.lockActiveWriters);
        //Unlock

    }

    //_exit(0);
    freeResources(RESOURCE);
    exit(0);
}

void processReader(int id, PARAMS param){
    //printStatus("#R: %d ready\n", id);

    int interval, readed;
    while(1){


        //Lock
        sem_wait(RESOURCE.lockAtomic);
            //Lock
            sem_wait(RESOURCE.lockRead);
                //Lock
                sem_wait(RESOURCE.lockActiveReaders);
                    (*RESOURCE.activeReaders)++;

                    //printStatus("\x1b[32;1mA: reader: %d: ready\x1b[0m\n", id);
                    printStatus("reader: %d: ready\n", id);

                    if(*(RESOURCE.activeReaders) == 1)
                        sem_wait(RESOURCE.lockWrite);

                sem_post(RESOURCE.lockActiveReaders);
                //Unlock
            sem_post(RESOURCE.lockRead);
            //Unlock
        sem_post(RESOURCE.lockAtomic);
        //Unlock

        //READING
        //printStatus("\x1b[32;1mA: reader: %d: reads a value\x1b[0m\n", id);
        printStatus("reader: %d: reads a value\n", id);
        readed = *(RESOURCE.workSpace);
        //printStatus("\x1b[32;1mA: reader: %d: read: %d\x1b[0m\n", id, readed);
        printStatus("reader: %d: read: %d\n", id, readed);

        interval = ((param.intervalReaders == 0)? 0 : (rand()%(param.intervalReaders + 1)));
        usleep(interval * 1000);

        //Lock
        sem_wait(RESOURCE.lockActiveReaders);
            (*RESOURCE.activeReaders)--;

            if(*(RESOURCE.activeReaders) == 0)
                sem_post(RESOURCE.lockWrite);

        sem_post(RESOURCE.lockActiveReaders);
        //Unlock

        //Read 0 from share memmory.. exiting
        if(readed == 0)
            break;
    }

    //_exit(0);
    freeResources(RESOURCE);
    exit(0);

}

void programAbort(int signal){
    UNUSED(signal);

    //Free all resources
    freeResources(RESOURCE);

    //Kill parent if exists
    //kill(getppid(), SIGTERM);

    printError(E_ABORT);
    //Child _exit(1);
    exit(1);
}

int parseInt(char* argv, int* flag){
	char* endPointer = NULL;

	int result = strtoul(argv, &endPointer, 10);

	if(endPointer == argv || *endPointer != '\0')
		if(flag)
			*flag = E_CONVERSION;

    return result;
}

void printError(int errorCode){
    if(errorCode > E_OK && errorCode <= E_SHM_CTL)
        fprintf(stderr, "%s\n", ERROR_MESSAGES[errorCode]);
}

void printStatus(char* format, ...){
    //LOCK
    sem_wait(RESOURCE.lockOutput);

    if(RESOURCE.output){
        va_list args;
        va_start(args, format);

        fprintf(RESOURCE.output, "%d: ", *(RESOURCE.actionCounter));

        (*RESOURCE.actionCounter)++;
        vfprintf(RESOURCE.output, format, args);
        fflush(RESOURCE.output);
        va_end(args);
    }

    //UNLOCK
    sem_post(RESOURCE.lockOutput);
}
