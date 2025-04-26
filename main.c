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
		char *arg[] = {"grep", ".c", NULL};
        execve("/usr/bin/grep", arg, data->env);
        perror("execve1");
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
    data = init_struct(NUM, env);
    data->prev_fd = 0;
    idx = 1;
	// fprintf(stderr, "cmd 1 path %s| 1=%s\n", data->cmd[0][0], data->cmd[0][1]);
	// fprintf(stderr, "cmd 2 path %s| 1=%s\n", data->cmd[1][0], data->cmd[1][1]);
    // execate first cmd manual
    exec_first(data);
    // while (data->path[idx + 1])
    // {
    //     // execate middle cmds
        exec_middle(data);
    //     data->count++;
    //     idx++;
    // }
    // data->count
    // execate last cmd manual
    exec_last(data);
    int i = 2;
	while ((wait(NULL) > 0))
		printf("wait...\n");
    free(data);
    return 0;
}

/*
test tree cmds :

ls | grep .c | wc

*/
