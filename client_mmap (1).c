#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int fd;
    caddr_t addr;
    struct stat statbuf;


    printf("\nHere Client!!\n");
    printf("%s %s\n",argv[0], argv[1]);
    //printf("%d", argc);

    if(argc != 3){
        fprintf(stderr, "Usage : %s filename\n", argv[0]);
        exit(1);
    }

    if(stat(argv[0] , &statbuf) == -1) {
        perror("stat");
        exit(1);
    }

    if((fd = open(argv[0], O_RDWR)) == -1) {
        perror("open");
        exit(1);
    }

    addr = mmap(NULL, statbuf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, (off_t)0);

    if(addr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    close(fd);

    printf("%s", addr);

    while(1) {
        printf("we wait sorter\n");
        sleep(3);
        msync(addr, statbuf.st_size, MS_SYNC);
        if(addr[0] == '%') {
            break;
        }
    }

    printf("%s", addr);

    if(munmap(addr, statbuf.st_size)== -1) {
        perror("munmap");
        exit(1);
    }
    printf("finish!!");

    return 0;
}