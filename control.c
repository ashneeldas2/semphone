#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#define KEY 6430
#define SHMKEY 1111

int create() {
    int sd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
    if (sd == -1) {
        printf("The semaphore already exists!\n");
    }
    else {
        union semun su; 
        su.val = 1;
        semctl(sd, 0, SETVAL, su);
        printf("semaphore created: %d\n", KEY);
        printf("value set: %d\n", su.val);
    }
    int fd = open("story.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
    close(fd);
    printf("story.txt file created\n");
    int sm = shmget(SHMKEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0644); 
    if (sm == -1) {
        printf("Shared memory already exists!\n");
    }
    else {
        printf("shared memory created: %d\n", SHMKEY);
    }
    return 0;
}

int my_remove() {
    int fd = open("story.txt", O_RDONLY, 0644);
    char story[1000]; 
    read(fd, &story, 1000);
    printf("%s\n", story);
    remove("story.txt");
    printf("story.txt removed\n");
    int sd = semget(KEY, 1, 0);
    printf("semaphore removed: %d\n", semctl(sd, 0, IPC_RMID));
    int sm = shmget(SHMKEY, sizeof(int), 0);
    printf("shared memory removed: %d\n", shmctl(sm, 0, IPC_RMID));
    return 0;
}

int view() {
    int fd = open("story.txt", O_RDONLY, 0644);
    char story[1000]; 
    read(fd, &story, 1000);
    close(fd);
    printf("%s\n", story);
    return 0;
}

int main(int argc, char ** argv) {
    if (argc < 2) {
        printf("Not enough arguments!\n");
    }
    else if (!strcmp(argv[1], "-c")) {
        create();
    }
    else if (!strcmp(argv[1], "-r")) {
        my_remove();
    }
    else if (!strcmp(argv[1], "-v")) {
        view();
    }
}