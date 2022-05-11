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
    caddr_t addr2;
    caddr_t str;
    //void* addr;
    struct stat statbuf;
    int i=0; 
    char* buf;
    const char *p_temp_str;
    int cut =0;

    printf("%s %s\n",argv[0], argv[1]);

    if(argc != 3){
        fprintf(stderr, "Usage : %s filename\n", argv[0]);
        exit(1);
    }

    if(stat(argv[1] , &statbuf) == -1) {
        perror("stat");
        exit(1);
    }

    if((fd = open(argv[1], O_RDWR)) == -1) {
        perror("open");
        exit(1);
    }

    addr = mmap(NULL, statbuf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, (off_t)0);

    if(addr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    close(fd);

    const char *p_str_list[statbuf.st_size];
    
    //printf("%s", addr);
    addr2 = (char *)malloc(sizeof(char) * 256);
    strcpy(addr2, addr);
    //printf("\n\n");
    //printf("%s", addr2);
    char *ptr = strtok(addr2, "\n");

    while(ptr != NULL) {
        //p_str_list[cut] = ptr;
        printf("split : %s\n", ptr);
        p_str_list[cut] = ptr;
        //p_str_list[i] = ptr;
        printf("p_str_list[%d] :%s\n",cut, p_str_list[cut]);
        ptr = strtok(NULL, "\n");
        cut++;
    }

    printf("after cut >>\n");
    for(int p =0; p<cut; p++) {
        printf("%s\n", p_str_list[p]);
    }
    
    for(int j=0; j<cut -1 ; j++) {
        for(int k=0; k<cut -1; k++) {
            if(strcmp(p_str_list[k], p_str_list[k+1]) >0 ) {
                p_temp_str = p_str_list[k];
                p_str_list[k] = p_str_list[k+1];
                p_str_list[k+1] = p_temp_str;
            }
        }
    }
    printf("after sort >>\n\n");
    for(int p =0; p<cut; p++) {
        printf("%s\n", p_str_list[p]);
    }
    str = (char *)malloc(sizeof(char) * 256);
    for(int p =0; p<cut; p++) {
        strcat(str, p_str_list[p]);
        strcat(str, "\n");
    }
    //printf("\n\n");
    //printf("gule\n");
    //printf("%s\n", str);
    
    strcpy(addr, str);
    printf("now addr>>>\n");
    printf("%s", addr);
 
    return 0;
}