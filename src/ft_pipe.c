/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 12:50:57 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/12 13:53:30 by cle-berr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	exit(127);
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
			return (free_args(shell->cmds), perror("pipe error"), 127);
		pid = fork();
		if (pid == -1)
			return (free_args(shell->cmds), perror("fork error"), 127);
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
	return (0);
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
		return (127);
	shell->envp1 = envp;
	ft_command_pipe(shell);
	while (wait(NULL) > 0)
		;
	free_args(shell->cmds);
	return (1);
}
