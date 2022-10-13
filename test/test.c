#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


void handler(int num) {
    printf("wowowowoow");
    pause();
}

int main() {
    pid_t pid;

    if ((pid = fork()) == 0) {
        signal(SIGTTIN, handler);
        printf("child's process group id is %d\n", (int) getpgrp());
        setpgid(0, 0);
        printf("child's process group id is now %d\n", (int) getpgrp());
        int x;
        scanf("%d", &x);
        exit(0);
    } else {
        printf("parent's process group id is %d\n", (int) getpgrp());
        printf("parent's process group id is now %d\n", (int) getpgrp());
        sleep(5);
    }
    return 0;
}