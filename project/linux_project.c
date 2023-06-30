#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include "trim.h"

#define MAX_PROCESSES 10
#define MAX_LINE_LENGTH 100
#define MAX_PARAMS 3

typedef struct {
  time_t lastRestartTime;
  char name[MAX_LINE_LENGTH];
  char params[MAX_PARAMS][20];
  int resetCount;
  char reason[30];
  pid_t pid;
} SoftwareBlock;

SoftwareBlock set[MAX_PROCESSES];
int swBlockCount;


void runProcess(SoftwareBlock *block) {
    printf("%s init...\n", block->name);
    pid_t pid = fork();

    if (pid < 0) {
        printf("%s init error..\n", block->name);
        exit(1);
    } else if (pid == 0) {
        printf("%s run..\n", block->name);

        sleep(3);
        exit(0);
    } else {
        block->pid = pid;
    }
}

int readFileList(void) {
  FILE *file = fopen("FileList.txt", "r");
  
  if (file == NULL) {
      perror("File open error");
      exit(1);
  }
  char buf[256];
    int index = 0;

    while (index < MAX_PROCESSES && fgets(buf, sizeof(buf), file)) {
      char *token = strtok(buf, ";");
      trim_str(token);
      strcpy(set[index].name, token);

      for (int paramIndex = 0; paramIndex < MAX_PARAMS; paramIndex++) {
          token = strtok(NULL, ";");
          if (token) {
              trim_str(token);
              strcpy(set[index].params[paramIndex], token);
          }
      }

      index++;
    }
  fclose(file);
  return index;
}
void printLog(FILE *log, char name[30], int restartCount, char restartTime[30], char reason[30]) {

    fprintf(log, "----------------------------------------\n\n");
    fprintf(log, "name: %s\n", name);
    fprintf(log, "resetcount: %d\n", restartCount);
    fprintf(log, "last reset time: %s\n", restartTime);
    fprintf(log, "reason: %s\n\n", reason);
    fprintf(log, "----------------------------------------\n\n");

    printf("\n----------------------------------------\n\n");
    printf("name: %s\n", name);
    printf("reset count: %d\n", restartCount);
    printf("last reset time: %s\n", restartTime);
    printf("reason: %s\n\n", reason);
    printf("----------------------------------------\n");
}
void signalHandler(int signum) {
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        SoftwareBlock *set = NULL;
        for (int i = 0; i < swBlockCount; i++) {
            if (set[i].pid == pid) {
                set = &set[i];
                break;
            }
        }

        if (set) {
            set->resetCount++;
            set->lastRestartTime = time(NULL);
            strcpy(set->reason, strsignal(signum));

            FILE *log = fopen("log.txt", "a");
            if (log == NULL) {
                printf("open log file error");
                exit(1);
            }

            printLog(log, set->name, set->resetCount, ctime(&set->lastRestartTime), set->reason);
            fclose(log);

            runProcess(set);
        } else {
            printf("child process %d stop.\n", pid);
        }
    }
}
void initSigaction() {
    struct sigaction sa;
    sa.sa_handler = signalHandler;
    sigemptyset(&sa.sa_mask);

    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa, 0) == -1) {
        printf("sigaction");
        exit(1);
    }
}


int main() {
  FILE *logFile = fopen("log.txt", "w");

  swBlockCount = readFileList();
  fclose(logFile);

  initSigaction();
  for (int i = 0; i < swBlockCount; i++) {
    runProcess(&set[i]);
  }

  while (1) {
    sleep(1);
  }
}

