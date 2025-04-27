#include "pipex.h"

// ls | grep as | wc -l
void exec_middle(pipex_t *data)
{
    int     fd[2];
    int     idx;
    pid_t   pid;

    if (pipe(fd) == -1)
        exit(3);
    idx = data->count++;
	fprintf(stderr, "midd idx = %d\n", idx);
    // Duplicate prev read end of pipe to STDIN
    if (dup2(data->prev_fd, STDIN_FILENO) == -1)
        perror("dup2");
    // Close prev read end of pipe
    if (data->prev_fd)
        close(data->prev_fd);
    // Store curr read end of pipe
    data->prev_fd = fd[0];
    // Create Child
    data->pid[idx] = fork();

    // Child
    if (data->pid[idx] == 0)
    {
        // Close Read end Because we are not use it
        close(fd[0]);
        // Duplicate curr write end of pipe to STDOUT
        if (dup2(fd[1], STDOUT_FILENO) == -1)
            perror("dup2");
        close(fd[1]);
        // Execute command
		if (idx == 1)
		{
			char *arg[] = {"grep", "404", NULL};
			execve("/usr/bin/grep", arg, data->env);
		}
		else if (idx == 2)
		{
			char *arg[] = {"awk", "{print $1}", NULL};
			execve("/usr/bin/awk", arg, data->env);
		}
		else if (idx == 3)
		{
			char *arg[] = {"sort", NULL};
			execve("/usr/bin/sort", arg, data->env);
		}
		else if (idx == 4)
		{
			char *arg[] = {"uniq", NULL};
			execve("/usr/bin/uniq", arg, data->env);
		}
        perror("execve1");
        return ;
    }
    // Close curr write end of pipe in Parrent Because We are not use it
    close(fd[1]);
}
// ls | wc -w -> 10
int main(int argc, char const *argv[], char **env)
{
    pipex_t data;

    (void)argc;
    (void)argv;
    init_struct(&data, NUM, env);
    data.prev_fd = 0;
	/*
	num_cmd = 3
	count
		ls | grep ".c" | wc -l
		0  |   	1      |    2
	*/
    // execate first cmd manual
    exec_first(&data);
    while (data.count < (data.num_cmd - 1))
    {
		// execate middle cmds
        exec_middle(&data);
    }
    // data.count
    // execate last cmd manual
    exec_last(&data);
    int i = 2;
	while ((wait(NULL) > 0))
		printf("wait...\n");
    clean_all(&data, EXIT_SUCCESS);
    return (EXIT_SUCCESS);
}

/*
test tree cmds :

ls | grep .c | wc

*/
