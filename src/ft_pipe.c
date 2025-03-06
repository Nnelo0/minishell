/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 12:50:57 by ebroudic          #+#    #+#             */
/*   Updated: 2025/03/06 10:53:39 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_pipe(t_shell *shell, int i)
{
	if (shell->input[i] && shell->input[i + 1]
		&& ft_strcmp(shell->input[i + 1], "|") == 0)
		return (1);
	return (0);
}

void	ft_exe_pipe_utils(t_shell *shell)
{
	if (shell->save_in != -1)
	{
		if (dup2(shell->save_in, STDIN_FILENO) == -1)
			perror("dup2 save IN");
		close (shell->save_in);
		shell->save_in = -1;
	}
	if (shell->save_out != -1)
	{
		if (dup2(shell->save_out, STDOUT_FILENO) == -1)
			perror("dup2 save OUT");
		close (shell->save_out);
		shell->save_out = -1;
	}
}

int	execute_pipe(t_shell *shell, int i)
{
	shell->pipe = ft_split_quote(shell->input[i], ' ');
	if (shell->prev_fd != -1)
	{
		dup2(shell->prev_fd, STDIN_FILENO);
		close(shell->prev_fd);
	}
	if (is_pipe(shell, i))
	{
		dup2(shell->pipefd[1], STDOUT_FILENO);
		close(shell->pipefd[1]);
		close(shell->pipefd[0]);
	}
	close(shell->pipefd[0]);
	shell->pipe = get_command(shell->pipe, shell, -1);
	shell->status = which_commands(shell->pipe, shell);
	ft_exe_pipe_utils(shell);
	verif_close(shell);
	free_args(shell->pipe);
	free_args(shell->input);
	free_env_list(shell->env_list);
	free_export_list(shell->export_list);
	free(shell->tmp);
	exit(shell->status);
}

int	parse_commands_pipe(t_shell *shell, int i, int status)
{
	pid_t	pid;

	shell->prev_fd = -1;
	while (shell->input[++i])
	{
		if (ft_strcmp(shell->input[i], "|") == 0)
			i++;
		if (is_pipe(shell, i) && pipe(shell->pipefd) == -1)
			return (perror("pipe: "), 127);
		pid = fork();
		if (pid == -1)
			return (perror("fork: "), 127);
		if (pid == 0)
			execute_pipe(shell, i);
		if (shell->prev_fd != -1)
			close(shell->prev_fd);
		if (is_pipe(shell, i))
		{
			close(shell->pipefd[1]);
			shell->prev_fd = shell->pipefd[0];
		}
	}
	while (waitpid(pid, &status, 0) > 0)
		;
	return (free_args(shell->input), shell->input = NULL, (status >> 8) & 0xFF);
}

int	ft_pipe(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->input[i])
	{
		if (ft_strcmp(shell->input[i], "|") == 0)
		{
			if (valid_pipe(shell->input[i], i) == 2)
				break ;
			if (valid_pipe(shell->input[i], i) == 1 || !shell->input[i + 1])
				return (free_args(shell->input), shell->input = NULL,
					ft_putstr_fd("Invalid Pipes\n", 2), shell->status);
			shell->input = merge_args(shell->input, "|", -1, 0);
			shell->status = parse_commands_pipe(shell, -1, 0);
			return (shell->status);
		}
		i++;
	}
	return (shell->status);
}
