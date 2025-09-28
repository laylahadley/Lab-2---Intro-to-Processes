#include <stdio.h>
#include <stdlib.h>     // srandom, random, exit
#include <unistd.h>     // fork, getpid, getppid, sleep
#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // wait, WIFEXITED, WEXITSTATUS
#include <time.h>       // time

void child_process(void);

int main(void) {
    // Ensure each printf appears promptly (helpful with multiple processes).
    setvbuf(stdout, NULL, _IONBF, 0);

    int pid;
    for (int i = 0; i < 2; i++) {
        pid = fork(); // create child
        if (pid == 0) {
            // Child path
            child_process();
            // child_process() calls exit(0), but add a return as a safeguard
            return 0;
        } else if (pid < 0) {
            // Fork failed
            perror("fork");
            exit(1);
        }
        // Parent continues to next iteration to fork the second child
    }

    // Parent waits for both children to complete
    for (int i = 0; i < 2; i++) {
        int status = 0;
        pid_t completed_pid = wait(&status);
        if (completed_pid == -1) {
            perror("wait");
            exit(1);
        }
        if (WIFEXITED(status)) {
            printf("Child Pid: %d has completed with exit status: %d\n",
                   (int)completed_pid, WEXITSTATUS(status));
        } else {
            printf("Child Pid: %d terminated abnormally\n", (int)completed_pid);
        }
    }

    return 0;
}

// Run one child process
void child_process(void) {
    pid_t pid = getpid();

    // Seed RNG uniquely per child *after* fork to avoid identical sequences
    srandom((unsigned)(time(NULL) ^ pid));

    // Random number of iterations in [1, 30]
    int n = 1 + (int)(random() % 30);

    for (int i = 0; i < n; i++) {
        // Random sleep duration in [1, 10] seconds
        unsigned int nap = 1 + (unsigned int)(random() % 10);
        printf("Child Pid: %d is going to sleep for %u seconds!\n", (int)pid, nap);
        sleep(nap);

        // Get current parent PID (in case it ever changes)
        pid_t parent_pid = getppid();
        printf("Child Pid: %d is awake!\nWhere is my Parent: %d?\n",
               (int)pid, (int)parent_pid);
    }

    exit(0);
}
