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

int main() {
    int sd = semget(KEY, 1, 0);
    int sm = shmget(SHMKEY, sizeof(int), 0);
    struct sembuf point;
    point.sem_num = 0;
    point.sem_flg = SEM_UNDO;
    point.sem_op = -1;
    semop(sd, &point, 1);
    int fd = open("story.txt", O_APPEND | O_RDWR);
    char story[1000];
    int *size = shmat(sm, 0, 0);
    lseek(fd, -*size, SEEK_END);
    read(fd, story, *size);
    printf("The last line of the story:\n\n");
    printf("%s\n", story);
    char line[1024];
    printf("Enter the next line of the story: ");
    fgets(line, sizeof(line), stdin);
    *size = strlen(line);
    lseek(fd, 0, SEEK_END);
    write(fd, line, strlen(line));
    shmdt(size);
    point.sem_op = 1;
    semop(sd, &point, 1 );
    return 0; 
}