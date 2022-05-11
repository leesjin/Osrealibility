#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
void regular(char *newname);
void fifo(char *newname);

int main(int argc, char *argv[]) {
    int kind;
    int nn;
    extern char *optarg;
    extern int optind;
    char *val;
    int mmode = 1;

    val = getenv("COM_FILE");

    while((nn = getopt(argc, argv, "t:f:")) != -1) {
        switch (nn)
        {
        case 't':
            if(strcmp(optarg, "f") ==0 ) {
                printf("fifo mode\n");
                mmode = 2;
            }
            else if(strcmp(optarg, "r") == 0){
                printf("regular mode\n");
                mmode = 1;
            }
            break;
        case 'f':
            val = optarg;
            break;
        }
    }

    if(mmode == 1){ //regular
        if(val == NULL){
            val = "data.txt";
        }
        regular(val);
    }
    else if(mmode == 2){ //fifo
        if(val == NULL){
            val = "data.fifo";
        }
        fifo(val);
    }

    return 0;
}

void regular(char *newname) {
    int fd,n ;
    char buf[256];
    mode_t mode;
    struct stat buff;
    DIR *dp;
    struct dirent *dent;
    
    if ((dp = opendir(".")) == NULL) {
        perror("opendir : current dir");
        exit (1);
    }

    while((dent = readdir(dp) )) {
        if(strcmp(dent -> d_name , newname) == 0) {
            stat(newname , &buff);
            if(S_ISREG(buff.st_mode)) {
                //write(1, "Regular file", 12);
            }
            else {
                write(1, "Not Regular File\n", 17);
                 exit(1);
            }
            if((buff.st_mode & S_IROTH) != 0 || (buff.st_mode & S_IWOTH) != 0 || (buff.st_mode & S_IRGRP) != 0 || (buff.st_mode & S_IWGRP) != 0) {
                write(1, "This must be protected\n", 27);
                exit(1);
            }
            break;
        }
    }

    mode = S_IRUSR | S_IWUSR ;

    fd = open(newname, O_CREAT | O_WRONLY | O_TRUNC , mode);
    if (fd == -1) {
        perror("Open file");
        exit(1);
    }
    while(1) {
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
        if(n == 1 && buf[0] == 'q') {
            write(1, "Terminate\n", 10);
            break;
        }
        write(1, buf, n);
    }     
    close(fd);
} 

void fifo(char *newname) {
    int pd, n;
    mode_t mode;
    char buf[256];
    struct stat buff;
    DIR *dp;
    struct dirent *dent;
    int exist = -1;
    
    if ((dp = opendir(".")) == NULL) {
        perror("opendir : current dir");
        exit (1);
    }

    while((dent = readdir(dp) )) {
        if(strcmp(dent -> d_name , newname) == 0) {
            exist = 1;
            stat(newname , &buff);
            if(S_ISFIFO(buff.st_mode)) {
                //write(1, "FIFO file", 12);
            }
            else {
                write(1, "Not FIFO File\n", 15);
                 exit(1);
            }
            if((buff.st_mode & S_IROTH) != 0 || (buff.st_mode & S_IWOTH) != 0 || (buff.st_mode & S_IRGRP) != 0 || (buff.st_mode & S_IWGRP) != 0) {
                write(1, "This must be protected\n", 27);
                exit(1);
            }
            break;
        }
    }

    printf("Server =====\n");
    mode = S_IRUSR | S_IWUSR ;

    if (exist != 1){
        if (mkfifo(newname, mode) == -1) {
            perror("mkfifo");
            exit(1);
        }
    }

    pd = open(newname, O_WRONLY, mode);
    if (pd == -1) {
        perror("Open error!");
        exit(1);
    }

    printf("To Client : \n");

    while(1) {
        write(1, ">> ", 3);
        n = read(0, buf, 255);
        buf[n] = '\0';
        if( n > 0 ) {
            if( write(pd, buf, n) != n ) {
                perror("Write error");
            }
        } else if (n==-1) {
            perror("Read error");
        }
        if(n == 1 && buf[0] == 'q') {
            write(1, "Terminate\n", 10);
            break;
        }
        write(1, buf, n);
    }     
    close(pd);
}
