#include "pipex.h"

void	clean_all(pipex_t *data, int stat)
{
	int	i;
	int	j;

	i = -1;
	// while (++i < data->num_cmd)
	// {
	// 	j = -1;
	// 	while (data->cmd[i][++j])
	// 	{
	// 		free(data->cmd[i][j]);
	// 		data->cmd[i][j] = NULL;
	// 	}
	// 	free(data->cmd[i][j]);
	// 	data->cmd[i][j] = NULL;
	// }
	free(data->cmd);
	data->cmd = NULL;
	exit(EXIT_FAILURE);
}

void exec_first(pipex_t *data)
{
    int     fd[2];
    int     idx;
    pid_t   pid;

    if (pipe(fd) == -1)
        exit (4);
    idx = data->count++;
	fprintf(stderr, "f idx = %d\n", idx);

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
		char *arg[] = {"cat", "access.log", NULL};
        execve("/usr/bin/cat", arg, data->env);
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

    idx = data->count++;
	fprintf(stderr, "l idx = %d\n", idx);
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
		char *arg[] = {"wc", "-l", NULL};
        execve("/usr/bin/wc", arg, data->env);
        perror("execve3");
        exit (1);
    }
    close(data->prev_fd);
}

pipex_t    *init_struct(pipex_t *data, int num, char **env)
{
	data->cmd = NULL;
	data->env = env;
	data->pid = NULL;
    data->cmd = ft_calloc(num, sizeof(char **));
    if (!data->cmd)
        exit (1);
    data->pid = ft_calloc(num, sizeof(int));
    if (!data->pid)
        exit ((free(data->cmd), 2));
    // just test

    // end test

	// Now must be separate commands '|'
	// and then seperate cmd and arg and set NULL in end of arr
	
    data->count = 0;
    data->num_cmd = NUM;
    data->prev_fd = -42;
	return (data);
}
