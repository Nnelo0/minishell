/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnelo <nnelo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 09:37:48 by ebroudic          #+#    #+#             */
/*   Updated: 2025/03/05 19:02:57 by nnelo            ###   ########.fr       */
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

void	ft_execute_shell(t_shell *shell, char **cmd, char *path, char **envp)
{
	verif_close_save(shell);
	verif_close(shell);
	execve(path, cmd, envp);
	ft_putstr_fd(cmd[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	free_args(envp);
	exit(127);
}

int	ft_shell(char **cmd, t_shell *shell, int status)
{
	pid_t	pid;
	char	*path;
	char	**envp;

	if (ft_strcmp(cmd[0], "") == 0 || verif_shell(cmd[0], shell, 0) == 1)
		return (shell->status);
	envp = env_in_stars(shell);
	path = find_command_path(cmd[0], envp);
	if (access(path, F_OK) == 0)
	{
		pid = fork();
		if (pid == -1)
			return (127);
		if (pid == 0)
			(signal(SIGQUIT, SIG_DFL),
				ft_execute_shell(shell, cmd, path, envp));
		return (waitpid(pid, &status, 0), free(path), free_args(envp)
			, (status >> 8) & 0xFF);
	}
	return (ft_putstr_fd(cmd[0], 2), ft_putstr_fd(": command not found\n", 2
		), free_args(envp), free(path), 127);
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
