/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouazrou <zouazrou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:06:26 by zouazrou          #+#    #+#             */
/*   Updated: 2025/04/30 11:06:27 by zouazrou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <errno.h>
#include <stdbool.h>

// chdir():
// opendir();
// readdir():
// closedir():

int pwd(char **argv, char **my_env)
{
    char    *buffer;

	(void)argv;
    buffer = NULL;
    buffer = getcwd(buffer, 0);
    if (!buffer)
	{
		if (errno == ENOENT)

        return (errno);
	}
    printf("%s\n", buffer);
    free(buffer);
    return (0);
}

int cd(char **argv)
{
    struct stat file;

    // check num of argms
    if (!argv || !*argv)

    // if arg 'cd' || 'cd ~' -> moving to $HOME env (check getenv())
    /*
        Handling this case
    */
    if (argv[2] != NULL)
        return (printf("cd: too many arguments\n"), 1);
    // check if exist
    if (access(argv[1], F_OK))
        return (printf("cd: %s: No such file or directory", argv[1]), 2);
    if (stat(argv[1], &file))
        return (perror("stat"), 3);
    // check is dir
    if (!S_ISDIR(file.st_mode))
        return (printf("cd: test_f: Not a directory\n"), 4);
    // check permission
    if (access(argv[1], X_OK))
        return (printf("cd: %s: Permission denied\n", argv[1]), 5);
    // moving to diroctory
    if (chdir(argv[1]))
        return (perror("chdir"), 6);
	// update env var $PWD
    return (0);
}
int is_built_in(char *cmd)
{
    if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "pwd")
    || !ft_strcmp(cmd, "export") ||!ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")
    || !ft_strcmp(cmd, "exit"))
        return (1);
    return (0);
}

int    env(char **env)
{
    int i;

    i = -1;
    while (env[++i])
        printf("%s\n", env[i]);
    return (0);
}

bool	option_nl(char *argv)
{
	int	i;
	int	j;

	j = 0;
	if (!argv || !argv[0] || !argv[1])
		return (false);
	while (argv[j])
	{
		if ((!j && argv[j] != '-') || (j && argv[j] != 'n'))
			return (false);
		j++;
	}
	return (true);
}

int	echo(char **argv)
{
	int		i;
	bool	nl;

	i = 1;
	nl = false;

	if (!argv || !*argv)
		return (1);

	nl = option_nl(argv[i]);
	if (nl == true)
		i++;
    while (argv[i])
    {
        if ((i != 1 && nl == false) || (i != 2 && nl == true))
            printf(" ");
        printf("%s", argv[i++]);
    }
    if (nl == false)	// nl it is mean option '-n' is found or not
        printf("\n");
    return (0);
}

int	main(int argc, char *argv[], char **environment)
{
    t_command cmd = { 0 };

    (void)argc;
    (void)argv;
	char **my_env = retrieve_envp(environment);
    char *av[] = {"echo", "-nnnnnn n", "Hello World", NULL};
    cmd.argv = av;
    cmd.infile = NULL;
    cmd.outfile = NULL;
    cmd.append = 0;
    cmd.pipe = 0;
    cmd.herdoc = NULL;
    cmd.next = NULL;

    // if (is_built_in("echo"))
    //     echo(av);
    // else
    //     printf("\n\nis not built in\n");
    if (pwd(my_env))
    {
        printf("failed\n");
        return (errno);
    }
	free_arr(my_env);
    return 0;
}
