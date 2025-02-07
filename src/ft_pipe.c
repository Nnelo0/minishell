/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 12:50:57 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/07 13:30:55 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*find_command_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;
	char	*part_path;

	i = 0;
	while (envp[i] && ft_strnstr(envp[i], "PATH=", 5) == 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
			return (free_args(paths), path);
		free(path);
		i++;
	}
	free_args(paths);
	return (0);
}

void	pipe_commands_utils(t_shell *shell, int prev_fd, int pipefd[2], int i)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (shell->cmds[i + 1])
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	close(pipefd[0]);
	which_commands(shell->cmds[i], shell->envp1, shell);
	free_args(shell->ipt);
	free(shell->input);
	free_args(shell->args);
	free_env_list(shell->env_list);
	free_export_list(shell->export_list);
	free_args(shell->cmds);
	exit(0);
}

int	ft_command_pipe(t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid;
	int		i;
	int		prev_fd;

	i = 0;
	prev_fd = -1;
	while (shell->cmds[i])
	{
		if (shell->cmds[i + 1] && pipe(pipefd) == -1)
			return (free_args(shell->cmds), perror("pipe error"), 1);
		pid = fork();
		if (pid == -1)
			return (free_args(shell->cmds), perror("fork error"), 1);
		if (pid == 0)
			pipe_commands_utils(shell, prev_fd, pipefd, i);
		if (prev_fd != -1)
			close(prev_fd);
		close(pipefd[1]);
		prev_fd = pipefd[0];
		i++;
	}
	if (prev_fd != -1)
		close(prev_fd);
	return (1);
}

int	is_valid_pipe(char *input)
{
	int	i;
	int	valid_command;

	i = 0;
	valid_command = 0;
	while (input[i])
	{
		if (input[i] == '|')
		{
			if (i == 0 || input[i + 1] == '|' || input[i + 1] == '\0')
				return (0);
			if (!valid_command)
				return (0);
			valid_command = 0;
		}
		else if (input[i] != ' ')
			valid_command = 1;
		i++;
	}
	return (valid_command);
}

int	ft_pipe(char *input, char **envp, t_shell *shell)
{
	if (!is_valid_pipe(input))
		return (printf("invalid pipes\n"));
	shell->cmds = ft_split(input, '|');
	if (!shell->cmds)
		return (1);
	shell->envp1 = envp;
	ft_command_pipe(shell);
	while (wait(NULL) > 0)
		;
	free_args(shell->cmds);
	return (1);
}
