/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 12:50:57 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/09 14:29:42 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_execute_command(t_shell *shell, int pipefd[2], int prev_fd, int i)
{
	char	*path;
	char	**args;

	if (shell->cmds[i + 1])
		dup2(pipefd[1], STDOUT_FILENO);
	if (prev_fd != 0)
		dup2(prev_fd, STDIN_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	args = ft_split(shell->cmds[i], ' ');
	path = find_command_path(args[0], shell->envp1);
	if (!path)
	{
		ft_printf("command not found: %s\n", shell->cmds[i]);
		free(path);
		free_args(shell->cmds);
		exit(EXIT_FAILURE);
	}
	execve(path, args, shell->envp1);
	ft_printf("command not found: %s\n", shell->cmds[i]);
	free(path);
	free_args(shell->cmds);
	exit(EXIT_FAILURE);
}

int	ft_command_pipe(char **cmds, t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid;
	int		i;
	int		prev_fd;

	i = 0;
	prev_fd = 0;
	while (cmds[i])
	{
		if (cmds[i + 1] && pipe(pipefd) == -1)
			return (free_args(cmds), 1);
		pid = fork();
		if (pid == -1)
			return (free_args(cmds), 1);
		if (pid == 0)
			ft_execute_command(shell, pipefd, prev_fd, i);
		if (prev_fd != 0)
			close(prev_fd);
		if (cmds[i + 1])
			prev_fd = pipefd[0];
		close(pipefd[1]);
		i++;
	}
	return (1);
}

int	ft_pipe(char *input, char **envp, t_shell *shell)
{
	shell->cmds = ft_split(input, '|');
	if (!shell->cmds)
		return (1);
	
	shell->envp1 = envp;
	ft_command_pipe(shell->cmds, shell);
	while (wait(NULL) > 0)
		;
	free_args(shell->cmds);
	return (1);
}

char	*find_command_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;
	char	*part_path;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
		{
			free_args(paths);
			return (path);
		}
		free(path);
		i++;
	}
	free_args(paths);
	return (0);
}