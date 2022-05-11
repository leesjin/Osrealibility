#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    DIR *dp;
    struct dirent *dent;
    char name[10] = "data.txt";
    int a = 1;

    if ((dp = opendir(".")) == NULL) {
        perror("opendir: .");
        exit(1);
    }
    while(a) {
        while((dent = readdir(dp))) {
        if(strcmp(dent->d_name, name)==0) {
            a = 0;
            break;
        }
        else {
            write(1, "no data.txt\n", 12);
            continue;
        }
    }
    rewinddir(dp);

    }
    
    int fd,n ;
    char buf[256];
    mode_t mode;
    mode = S_IRUSR | S_IWUSR ;
    struct stat buff;

    stat("data.txt", &buff);

    if(S_ISREG(buff.st_mode)) {
        //write(1, "Regular file", 12);
    }
    else {
        write(1, "Not Regular File\n", 17);
        exit(1);
    }

    if((buff.st_mode & S_IROTH) != 0 || (buff.st_mode & S_IWOTH) != 0 || (buff.st_mode & S_IRGRP) != 0 || (buff.st_mode & S_IWGRP) != 0) {
        write(1, "data.txt must be protected\n", 27);
        exit(1);
    }
    
    int size;

    while(1) {
        stat("data.txt", &buff);
        size = (int)buff.st_size;
        if(size == 0) {
            fd = open("data.txt", O_RDONLY, mode);
            if (fd == -1) {
            perror("Open data.txt");
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
    close(fd);
    closedir(dp);

    return 0;
}