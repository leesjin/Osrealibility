#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int fd,n ;
    char buf[256];
    mode_t mode;
    struct stat buff;
    int size;
    int start = 0;
    pid_t pid;
    //pid_t wait;
    int nn;
    extern char *optarg;
    extern int optind;
    char *val;
    char *val2;
    char *val3;

    val2 = getenv("SORT_COMM");
    val3 = getenv("SORT_COMM_BUF");

    while((nn = getopt(argc, argv, "t:f:s:")) != -1) {
        switch (nn)
        {
        case 't':
            val = optarg;
            break;
        case 'f':
            val2 = optarg;
            if(val2 == NULL){
                val2 = "data.mmap";
            }
            break;
        case 's':
            val3 = optarg;
            if(val3 == NULL){
                val3 = "4096";
            }
            break;
        }
    }


    mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    fd = open(val2, O_CREAT | O_WRONLY | O_TRUNC, mode);
    if (fd == -1) {
        perror("Open data.mmap");
        exit(1);
    }
    
    while(1) {
        stat(val2, &buff);
        size = (int)buff.st_size;
        write(1, ">> ", 3);
        n = read(0, buf, 255);
        buf[n] = '\0';
        if( n > 0 ) {
            if( write(fd, buf, n) != n ) {
                perror("Write error");
            }
        } else if (n==-1) {
            perror("Read error");
        }
        if(n == 5 && buf[0] == '%' && buf[1] == 's' && buf[2] == 'o' && buf[3] == 'r' && buf[4] == 't') {
            start = 1;
            write(1,"sort!!",6);
            write(1, "Terminate\n", 10);
            break;
        }
        write(1, buf, n);
    }

    //printf("finish\n");    
    //printf("%d\n", start);

    stat(val2, &buff);
    int txtsize = (int)buff.st_size;
    int status;

    if(start = 1) {  //%sort input 
       switch(pid = fork()) {
            case -1 : 
                perror("fork");
                exit(1);
                break;
            case 0 :
                execl("c_mmap", val2, val3);
                printf("Child Process\n");
                exit(2);
                break;
            default :
                while(wait(&status) != pid) {
                    continue;
                }
                printf("Parent Process!\n");
                break; 
       }
    }

    close(fd);

    return 0;
}