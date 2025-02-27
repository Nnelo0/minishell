/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 09:37:48 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/27 11:18:26 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	verif_close_save(t_shell *shell)
{
	if (shell->save_in != -1)
		close(shell->save_in);
	if (shell->save_out != -1)
		close(shell->save_out);
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
	envp = env_in_stars(shell);
	path = find_command_path(cmd[0], envp);
	if (!path)
		return (ft_putstr_fd(cmd[0], 2), ft_putstr_fd(": command not found\n", 2
			), free_args(envp), 127);
	pid = fork();
	if (pid == -1)
		return (127);
	if (pid == 0)
	{
		verif_close_save(shell);
		verif_close(shell);
		execve(path, cmd, envp);
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		free_args(envp);
		exit(127);
	}
	return (waitpid(pid, &status, 0), free(path), free_args(envp)
		, (status >> 8) & 0xFF);
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
