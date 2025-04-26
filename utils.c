#include "pipex.h"

void exec_first(pipex_t *data)
{
    int     fd[2];
    int     idx;
    pid_t   pid;

    if (pipe(fd) == -1)
        exit (4);
    idx = data->count++;
    
    // Create Child
    pid = fork();
    // Child
    if (pid == 0)
    {
        // Close unused fd
        close(fd[0]);
        // Duplicate write end of pipe to STDOUT
        if (dup2(fd[1], STDOUT_FILENO) == -1)
            perror("df");
        close(fd[1]);
        // Execute command
        execve(data->path[idx], data->cmd[idx], data->env);
        perror("execve");
        return ;
    }
    close(fd[1]);
}

void exec_last(pipex_t *data)
{
    int     idx;
    pid_t   pid;

    idx = data->count;
    // Create Child
    pid = fork();
    // Child
    if (pid == 0)
    {
        // Duplicate read end of pipe to STDIN
        if (dup2(data->prev_fd, STDIN_FILENO) == -1)
            perror("df");
        close(data->prev_fd);
        // Execute command
        execve(data->path[idx], data->cmd[idx], data->env);
        perror("execve");
        return ;
    }
}

void    init_struct(pipex_t *data, int num, char **env)
{
    char *c1[] = {"ls", NULL};
    char *c2[] = {"wc", "-w", NULL};

    data = malloc(sizeof(pipex_t));
    if (!data)
        exit((perror("malooc"), 1));
    data->cmd = malloc((num + 1) * sizeof(char **));
    if (!data->cmd)
        exit (1);
    data->path = malloc((num + 1) * sizeof(char *));
    if (!data->path)
        exit ((free(data->cmd), 2));

    // just test
    data->cmd[0] = c1;
    data->cmd[1] = c2;
    data->cmd[num] = NULL;
    data->path[0] = "/usr/bin/ls";
    data->path[1] = "/usr/bin/wc"; 
    data->path[num] = NULL;
    // end test

    data->env = env;
    data->count = 0;
    data->prev_fd = -42;
}