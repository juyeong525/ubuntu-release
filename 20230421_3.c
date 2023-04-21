#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    char *envlist = "MYDATA=5";
    execle("my_script","my_script", (char *)0, envlist);


    return 0;
}
