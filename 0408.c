#include<stdio.h>
#include<unistd.h>

int main(){
	if(!access("b.txt", F_OK))
		printf("OK");
	else
		printf("sorry");
}
