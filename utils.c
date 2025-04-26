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
    data->pid[idx] = fork();
	if (data->pid[idx] == -1)
		exit ((perror("fork"), 1));
    // Child
    if (data->pid[idx] == 0)
    {
        // Close unused fd
        close(fd[0]);
        // Duplicate write end of pipe to STDOUT
        if (dup2(fd[1], STDOUT_FILENO) == -1)
            perror("df");
        close(fd[1]);
		char *arg[] = {"ls", NULL};
        execve("/usr/bin/ls", arg, data->env);
        perror("execve2");
        exit(9) ;
    }
    close(fd[1]);
    data->prev_fd = fd[0];
}

void exec_last(pipex_t *data)
{
    int     idx;
    pid_t   pid;

    idx = data->count;
    // Create Child
    data->pid[idx] = fork();
	if (data->pid[idx] == -1)
		exit ((perror("fork"), 2));
    // Child
    if (data->pid[idx] == 0)
    {
        // Duplicate read end of pipe to STDIN
        if (dup2(data->prev_fd, STDIN_FILENO) == -1)
            perror("df");
        close(data->prev_fd);
        // Execute command
		char *arg[] = {"wc", NULL};
        execve("/usr/bin/wc", arg, data->env);
        perror("execve3");
        exit (1);
    }
    close(data->prev_fd);
}

pipex_t    *init_struct(int num, char **env)
{
    pipex_t     *data;


    data = malloc(sizeof(pipex_t));
    if (!data)
        exit((perror("malooc"), 1));
    data->cmd = malloc((num + 1) * sizeof(char **));
    if (!data->cmd)
        exit (2);
    data->path = malloc((num + 1) * sizeof(char *));
    if (!data->path)
        exit ((free(data->cmd), 3));
    data->pid = malloc((num) * sizeof(int));
    if (!data->pid)
        exit ((free(data->cmd), 4));
    // just test
	malloc(sizeof(char *) * 2);
	char *c1[] = {"/usr/bin/ls", NULL};
    char *c2[] = {"/usr/bin/wc", NULL};
    // data->cmd[0] = &c1;
    // data->cmd[1] = &c2;
    data->cmd[num] = NULL;
	// path
    data->path[0] = "/usr/bin/ls";
    data->path[1] = "/usr/bin/wc";
    data->path[num] = NULL;
    // end test

    data->env = env;
    data->count = 0;
    data->prev_fd = -42;
	return (data);
}
