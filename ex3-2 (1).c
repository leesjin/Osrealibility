#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


int main(void) {
    int fd,n ;
    char buf[256];
    mode_t mode;
    struct stat buff;
    int kind;

    mode = S_IRUSR | S_IWUSR ;
 
    fd = open("data.txt", O_CREAT | O_WRONLY | O_TRUNC , mode);
    if (fd == -1) {
        perror("Open data.txt");
        exit(1);
    }

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

    return 0;
} 
