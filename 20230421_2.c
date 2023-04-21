#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "USEway: %s file1 file2 N\n", argv[0]);
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        char *filename = argv[i];

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork error");
            exit(1);
        } else if (pid == 0) {
            execl("/usr/bin/wc", "wc", filename, (char *) NULL);
            perror("exec error");
            exit(1);
        }
    }

    for (int i = 1; i < argc; i++) {
        wait(NULL);
    }


    return 0;
}
