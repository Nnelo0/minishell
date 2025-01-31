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

void	setup_redirection(t_shell *shell)
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
}

static void	ft_shell_utils(char *path, char **cmd, char **envp, t_shell *shell)
{
	setup_redirection(shell);
	close(shell->in);
	close(shell->out);
	execve(path, cmd, envp);
}

int	ft_shell(char *input, char **envp, t_shell *shell)
{
	pid_t	pid;
	char	*path;
	char	**cmd;

	shell->in = dup(STDIN_FILENO);
	shell->out = dup(STDOUT_FILENO);
	if (shell->in == -1 || shell->out == -1)
		return (perror("dup failed"), 1);
	cmd = ft_split(input, ' ');
	path = find_command_path(cmd[0], envp);
	if (!path)
		return (printf("%s: command not found\n", input), close(shell->in),
			close(shell->out), free_args(cmd), 127);
	pid = fork();
	if (pid == -1)
		return (0);
	if (pid == 0)
		ft_shell_utils(path, cmd, envp, shell);
	if (dup2(shell->in, STDIN_FILENO) == -1
		|| dup2(shell->out, STDOUT_FILENO) == -1)
		return (perror("dup2 failed"), close(shell->in), close(shell->out), 1);
	return (close(shell->in), close(shell->out), free(path),
		free_args(cmd), wait(NULL), 1);
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
