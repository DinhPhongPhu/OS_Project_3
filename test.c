#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>

int main(){
	int fd;
	char buf[50]="Viet_Phu_Son\n";
	fd = open("in.txt",O_WRONLY | O_CREAT);
	printf("Wrote to file: %d\n",fd);
	if(write(fd, buf, strlen(buf)) == 13) {
    	printf("Successfully!\n");
	}
	else{
    	printf("Failed!\n");
	}
	close(fd);
	return 0;
}