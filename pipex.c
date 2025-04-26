#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char **argv, char **envp)
{
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // ─── First child: run `/bin/ls` and send its stdout into pipefd[1] ───
    pid = fork();
    if (pid < 0) {
        perror("fork ls");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        // Child #1
        close(pipefd[0]);                    // close unused read end
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
            perror("dup2 ls");
        close(pipefd[1]);

        char *argv_ls[]  = {"ls", NULL};
        execve("/bin/ls", argv_ls, envp);
        // If execve returns, it failed:
        perror("execve /bin/ls");
        _exit(EXIT_FAILURE);
    }

    // ─── Parent continues here ───
    close(pipefd[1]);  // close write end in parent

    // ─── Second child: read from pipefd[0] as stdin, exec `/usr/bin/wc` ───
    pid = fork();
    if (pid < 0) {
        perror("fork wc");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        // Child #2
        if (dup2(pipefd[0], STDIN_FILENO) == -1)
            perror("dup2 wc");
        close(pipefd[0]);

        char *argv_wc[] = { "wc", NULL };
        execve("/usr/bin/wc", argv_wc, envp);
        perror("execve /usr/bin/wc");
        _exit(EXIT_FAILURE);
    }

    // ─── Parent: close leftover and wait ───
    close(pipefd[0]);

    // This will block for each child, then exit when no more children:
    while (wait(NULL) > 0)
        ;

    return 0;
}
