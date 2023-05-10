#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
	char *filename = "test.txt";

	if(access(filename, R_OK) == -1) {
		fprintf(stderr, "User");
		//printf("user cannot read file %s]n",
	}
}
