#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void fifo(char *newname);
void regular(char *newname);

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

void regular(char *newname){
    DIR *dp;
    struct dirent *dent;
    int fd,n ;
    char buf[256];
    mode_t mode;
    mode = S_IRUSR | S_IWUSR ;
    struct stat buff;
    
    int size;

    if ((dp = opendir(".")) == NULL) {
        perror("opendir: .");
        exit(1);
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
                write(1, "This file must be protected\n", 27);
                exit(1);
            }
            break;
        }
    }

    while(1) {
        stat(newname, &buff);
        size = (int)buff.st_size;
        if(size == 0) {
            fd = open(newname, O_RDONLY);
            if (fd == -1) {
            perror("Open error!");
            exit(1);
        }
        while(1) {
            n = read(fd, buf, 255);
            buf[n] = '\0';
            if (n==-1) {
                perror("Read error");
            } else if (n == 0) continue;
            write(1, "Recv>> ", 7);
            write(1, buf, n);
            if(n == 1 && buf[0] == 'q') {
                write(1, "Terminate\n", 10);
                break;
            }
        } 
    }
    if(n == 1 && buf[0] == 'q') {
        write(1, "Terminate\n", 10);
        break;
        } 
    }    

}

void fifo(char *newname){
    DIR *dp;
    struct dirent *dent;
    int fd,n ;
    char buf[256];
    mode_t mode;
    mode = S_IRUSR | S_IWUSR ;
    struct stat buff;
    
    int size;

    if ((dp = opendir(".")) == NULL) {
        perror("opendir: .");
        exit(1);
    }
    
   while((dent = readdir(dp) )) {
        if(strcmp(dent -> d_name , newname) == 0) {
            stat(newname , &buff);
            if(S_ISFIFO(buff.st_mode)) {
                //write(1, "FIFO file", 12);
            }
            else {
                write(1, "Not FIFO File\n", 17);
                 exit(1);
            }
            if((buff.st_mode & S_IROTH) != 0 || (buff.st_mode & S_IWOTH) != 0 || (buff.st_mode & S_IRGRP) != 0 || (buff.st_mode & S_IWGRP) != 0) {
                write(1, "This file is must be protected\n", 27);
                exit(1);
            }
            break;
        }
    }

    while(1) {
        stat(newname, &buff);
        size = (int)buff.st_size;
        if(size == 0) {
            fd = open(newname, O_RDONLY, mode);
            if (fd == -1) {
            perror("Open error!");
            exit(1);
        }
        while(1) {
            n = read(fd, buf, 255);
            buf[n] = '\0';
            if (n==-1) {
                perror("Read error");
            } else if (n == 0) continue;
            write(1, "Recv>> ", 7);
            write(1, buf, n);
            if(n == 1 && buf[0] == 'q') {
                write(1, "Terminate\n", 10);
                break;
            }
        } 
    }
    if(n == 1 && buf[0] == 'q') {
        write(1, "Terminate\n", 10);
        break;
        } 
    }    


}

