/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 09:37:48 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/16 12:28:45 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_shell(char *input, char **envp)
{
	pid_t	pid;
	char	*path;
	char	**cmd;

	cmd = ft_split(input, ' ');
	path = find_command_path(cmd[0], envp);
	if (!path)
		return (printf("command not found: %s\n", input), free_args(cmd), 127);
	pid = fork();
	if (pid == -1)
		return (0);
	if (pid == 0)
	{
		execve(path, cmd, envp);
		ft_printf("command not found: %s\n", input);
		free(path);
		free_args(cmd);
		exit(127);
	}
	free(path);
	free_args(cmd);
	wait(NULL);
	return (1);
}

int	ft_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	ft_printf("%s\n", pwd);
	free(pwd);
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
