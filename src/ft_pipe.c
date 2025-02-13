/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnelo <nnelo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 12:50:57 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/14 00:49:14 by nnelo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	parse_commands_pipe(t_shell *shell, char **envp)
{
	int		i;
	pid_t	pid;
	int		prev_fd;

	i = 0;
	prev_fd = -1;
	while (shell->input[i])
	{
		if (ft_strcmp(shell->input[i], "|") == 0)
			i++;
		if (shell->input[i] && shell->input[i + 1] && ft_strcmp(shell->input[i + 1], "|") == 0)
		{
			if (pipe(shell->pipefd) == -1)
				return (perror("piped: "), 127);
			printf(RED "pipe effectuer pour %s\n" RESET, shell->input[i]);
		}
		pid = fork();
		if (pid == -1)
			return (perror("fork: "), 127);
		if (pid == 0)
		{
			printf(GREEN "fork effectuer pour %s\n" RESET, shell->input[i]);
			shell->pipe = ft_split(shell->input[i], ' ');
			printf(MAGENTA "extract_command effectuer pour %s->%s\n" RESET, shell->input[i], shell->pipe[0]);
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
				printf(BLUE "dup2 in effectuer pour %s\n" RESET, shell->input[i]);
			}
			if (shell->input[i] && shell->input[i + 1] && ft_strcmp(shell->input[i + 1], "|") == 0)
			{
				dup2(shell->pipefd[1], STDOUT_FILENO);
				close(shell->pipefd[1]);
				close(shell->pipefd[0]);
				fprintf(stderr, YELLOW "dup2 OUT effectuer pour %s\n" RESET, shell->input[i]);
			}
			close(shell->pipefd[0]);
			fprintf(stderr, CYAN "renvoie de la commande: %s\n" RESET, shell->input[i]);
			which_commands(shell->pipe, envp, shell);
			fprintf(stderr, CYAN "renvoie de la commande effectuer pour %s\n" RESET, shell->input[i]);
			exit(shell->status);
		}
		if (prev_fd != -1)
			close(prev_fd);
		if (shell->input[i] && shell->input[i + 1] && ft_strcmp(shell->input[i + 1], "|") == 0)
		{
			close(shell->pipefd[1]);
			prev_fd = shell->pipefd[0];
		}
		i++;
	}
	while (wait(NULL) > 0)
		;
	shell->input = NULL;
	return (shell->status);
}

int	ft_pipe(char **envp, t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->input[i])
	{
		if (ft_strchr(shell->input[i], '|'))
			return (parse_commands_pipe(shell, envp));
		else
			shell->status = 0;
		i++;
	}
	return (shell->status);
}
