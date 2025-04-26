#include "pipex.h"

#define NUM 2

// ls | grep as | wc -l
void exec_middle(pipex_t *data)
{
    int     fd[2];
    int     idx;
    pid_t   pid;

    if (pipe(fd) == -1)
        exit(3);
    idx = data->count;
    // Duplicate prev read end of pipe to STDIN
    if (dup2(data->prev_fd, STDIN_FILENO) == -1)
        perror("dup2");
    // Close prev read end of pipe
    if (data->prev_fd)
        close(data->prev_fd);
    // Store curr read end of pipe
    data->prev_fd = fd[0];
    // Create Child
    pid = fork();

    // Child
    if (pid == 0)
    {
        // Close Read end Because we are not use it
        close(fd[0]);
        // Duplicate curr write end of pipe to STDOUT
        if (dup2(fd[1], STDOUT_FILENO) == -1)
            perror("dup2");
        close(fd[1]);
        // Execute command
        execve(data->path[idx], data->cmd[idx], data->env);
        perror("execve");
        return ;
    }
    // Close curr write end of pipe in Parrent Because We are not use it
    close(fd[1]);
}
// ls | wc -w -> 10
int main(int argc, char const *argv[], char **env)
{
    pipex_t *data;
    int     idx;

    (void)argc;
    (void)argv;
    data = NULL;
    init_struct(data, NUM, env);
    data->prev_fd = 0;
    idx = 1;
    // execate first cmd manual
    exec_first(data);
    while (data->path[idx + 1])
    {
        // execate middle cmds
        exec_middle(data);
        data->count++;
        idx++;
    }
    // execate last cmd manual
    exec_last(data);
    while (wait(NULL) != -1 || errno != ECHILD)
    {
        printf("wait...\n");
    }
    free(data);
    return 0;
}
