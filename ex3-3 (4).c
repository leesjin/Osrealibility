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

    fd = open("data.txt", O_RDONLY);
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
    
    close(fd);
    closedir(dp);

    return 0;
}