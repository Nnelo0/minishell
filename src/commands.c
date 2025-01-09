/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:01:33 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/09 12:47:52 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_exit(char *input, t_shell *shell)
{
	printf("exit\n");
	free(input);
	free_args(shell->args);
	return (0);
}

int	ft_exe(t_shell *shell, char **envp)
{
	pid_t	pid;

	if (!shell->args || !shell->args[0])
		return (printf("minishell: command not found\n"));
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		if (execve(shell->args[0], shell->args, envp) == -1)
		{
			perror("minishell");
			free_args(shell->args);
			exit(1);
		}
	}
	wait(NULL);
	free_args(shell->args);
	shell->args = NULL;
	return (1);
}

int	ft_cd(t_shell *shell)
{
	char	*target;

	if (!shell->args[1])
	{
		target = getenv("HOME");
		if (!target)
			return (printf("cd: HOME not set\n"));
	}
	else if (shell->args[2])
		return (printf("cd: too many arguments\n"));
	else
		target = shell->args[1];
	if (chdir(target) == -1)
	{
		perror("cd");
		return (1);
	}
	return (1);
}

int ft_pipe(char *input, char **envp)
{
    char    **cmds;
    int     pipefd[2];
    pid_t   pid;
    int     i;
    int     prev_fd;
    char    *path;
	char	**args;

    i = 0;
    prev_fd = 0;
    cmds = ft_split(input, '|');
	if (!cmds)
        return (1);
    while (cmds[i])
    {
        if (cmds[i + 1] && pipe(pipefd) == -1)
        {
            free_cmd(cmds);
            return (1);
        }
        pid = fork();
        if (pid == -1)
        {
            free_cmd(cmds);
            return (1);
        }
        if (pid == 0)
        {
            if (cmds[i + 1])
                dup2(pipefd[1], STDOUT_FILENO);
            if (prev_fd != 0)
                dup2(prev_fd, STDIN_FILENO);
            close(pipefd[0]);
            close(pipefd[1]);
			args = ft_split(cmds[i], ' ');
            path = find_command_path(args[0], envp);
            if (!path)
            {
                ft_printf("command not found in pipe: %s\n", cmds[i]);
                free(path);
                free_cmd(cmds);
                exit(EXIT_FAILURE);
            }
            execve(path, args, envp);
            ft_printf("command not found in pipe afte execve: %s\n",  cmds[i]);
            free(path);
            free_cmd(cmds);
            exit(EXIT_FAILURE);
        }
		if (prev_fd != 0)
        	close(prev_fd);
		if (cmds[i + 1])
			prev_fd = pipefd[0];
		close(pipefd[1]);
        i++;
    }
    while (wait(NULL) > 0)
		;
    free_cmd(cmds);
    return (1);
}

int	which_commands(char *input, char **envp, t_shell *shell)
{
	if (ft_strchr(input, '|'))
		return(ft_pipe(input, envp));
	if (ft_strncmp(input, "exit", 4) == 0
		&& (input[4] == ' ' || input[4] == '\0'))
		return (ft_exit(input, shell));
	if (ft_strncmp(input, "echo", 4) == 0
		&& (input[4] == ' ' || input[4] == '\0'))
		return (ft_echo(input));
	if (ft_strncmp(input, "cd", 2) == 0
		&& (input[2] == ' ' || input[2] == '\0'))
		return (ft_cd(shell));
	if (ft_strncmp(input, "./", 2) == 0)
		return (ft_exe(shell, envp));
	if (ft_strncmp(input, "env", 3) == 0 && input[3] == '\0')
		return (ft_env(envp));
	if (ft_strncmp(input, "pwd", 3) == 0 && input[3] == '\0')
		return (ft_pwd());
	ft_shell(input, envp, shell);
	return (1);
}

int	commands(char *input, char **envp, t_shell *shell)
{
	while (*input && (*input == ' ' || *input == '\t'))
		input++;
	if (*input == '\0')
		return (1);
	shell->args = ft_split(input, ' ');
	if (!ft_quotes(input))
		return (ft_printf("open quote\n"));
	ft_remove_quotes(input);
	which_commands(input, envp, shell);
	return (1);
}
