#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

void file1(char*);

int main(int argc, char* *argv){
	if(argc!=2) exit(0);
	file1(argv[1]);
}

void file1(char* filename){
	int fd = open(filename, O_RDWR|O_CREAT, 0644);
	char a[1024] = "sdfasfasdfasdfasdf";
	write(fd, a, 1024);
	close(fd);

	fd = open(filename, O_RDWR, 0644);
	char b[1024] = {};
	read(fd, b, 1024);
	close(fd);
	int count = 0;
	for(int i=0; i<1024; i++){
		if(b[i] == '\0'){
			break;
		}
		count++;
	}
	printf("%d\n", count);
}
