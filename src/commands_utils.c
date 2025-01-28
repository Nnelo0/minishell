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

int	ft_shell(char *input, char **envp, t_shell *shell)
{
	pid_t	pid;
	char	*path;
	char	**cmd;
	int		in;
	int		out;

	in = dup(STDIN_FILENO);
	out = dup(STDOUT_FILENO);
	if (in == -1 || out == -1)
		return (perror("dup failed"), 1);
	cmd = ft_split(input, ' ');
	path = find_command_path(cmd[0], envp);
	if (!path)
		return (printf("%s: command not found\n", input), close(in),
			close(out), free_args(cmd), 127);
	pid = fork();
	if (pid == -1)
		return (0);
	if (pid == 0)
	{
		if (shell->fd_in != -1 && dup2(shell->fd_in, STDIN_FILENO) == -1)
		{
			perror("dup2 failed");
			close(shell->fd_in);
			free(shell->cmd);
			exit(127);
		}
		if (shell->fd_out != -1 && dup2(shell->fd_out, STDOUT_FILENO) == -1)
		{
			perror("dup2 failed");
			close(shell->fd_out);
			free(shell->cmd);
			exit(127);
		}
		if (shell->fd_in != -1)
			close(shell->fd_in);
		if (shell->fd_out != -1)
			close(shell->fd_out);
		close(in);
		close(out);
		execve(path, cmd, envp);
		ft_printf("command not found: %s\n", input);
		free(path);
		free_args(cmd);
		close(in);
		close(out);
		exit(127);
	}
	if (dup2(in, STDIN_FILENO) == -1 || dup2(out, STDOUT_FILENO) == -1)
		return (perror("dup2 failed"), close(in), close(out), 1);
	close(in);
	close(out);
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
