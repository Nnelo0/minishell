/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 09:37:48 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/25 15:16:21 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	stp_redirection(t_shell *shell)
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
	(void)shell;
	execve(path, cmd, envp);
}

void	verif_close(t_shell *shell)
{
	if (shell->fd_in != -1)
		close(shell->fd_in);
	if (shell->fd_out != -1)
		close(shell->fd_out);
}

int	ft_shell(char **cmd, char **envp, t_shell *shell, int status)
{
	pid_t	pid;
	char	*path;

	if (ft_strcmp(cmd[0], "") == 0)
		return (shell->status);
	path = find_command_path(cmd[0], envp);
	if (!path)
		return (ft_putstr_fd(cmd[0], 2), ft_putstr_fd(": command not found\n", 2
			), 127);
	pid = fork();
	if (pid == -1)
		return (127);
	if (pid == 0)
		ft_shell_utils(path, cmd, envp, shell);
	return (waitpid(pid, &status, 0), free(path), (status >> 8) & 0xFF);
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
