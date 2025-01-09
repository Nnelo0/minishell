/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 09:37:48 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/09 11:33:17 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	command_shell(char *input, char **envp, t_shell *shell)
{
	char	*path;

	path = find_command_path(*shell->args, envp);
	if (!path)
	{
		ft_printf("command not found: %s\n", input);
		free(path);
		free_args(shell->args);
		exit(EXIT_FAILURE);
	}
	execve(path, shell->args, envp);
	ft_printf("command not found: %s\n", input);
	free(path);
	free_args(shell->args);
	exit(EXIT_FAILURE);
}

void	ft_shell(char *input, char **envp, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
		command_shell(input, envp, shell);
	wait(NULL);
}

int	ft_pwd(void)
{
	char	*cwd;
	char	*buffer;

	buffer = (char *)malloc(sizeof(char) * 4096);
	if (!buffer)
	{
		perror("malloc");
		return (1);
	}
	cwd = getcwd(buffer, 4096);
	if (!cwd)
	{
		perror("");
		return (1);
	}
	printf("%s\n", cwd);
	free(buffer);
	return (1);
}

int	ft_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	return (1);
}

void	free_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}
