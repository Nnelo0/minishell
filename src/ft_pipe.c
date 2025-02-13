/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 12:50:57 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/13 15:57:11 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	parse_commands_pipe(t_shell *shell, char **envp)
{
	int	i;
	pid_t pid;

	i = 0;
	while (shell->input[i])
	{
		if (shell->input[i + 1] && ft_strcmp(shell->input[i + 1], "|") == 0)
		{	
			if (pipe(shell->pipefd) == -1)
				return (perror("pipe error"), 127);
		}
		pid = fork();
		if (pid == -1)
			return (perror("pipe error"), 127);
		if (pid == 0)
		{
			shell->pipe[0] = ft_strdup(shell->input[i]);
			which_commands(shell->pipe, envp, shell);//ca veut rentrer la dedans
		}
		i++;
	}
	return (shell->status);
}

int	ft_pipe(char **envp, t_shell *shell)
{
	int	i;

	i = 0;
	
	while(shell->input[i])
	{
		if (ft_strchr(shell->input[i], '|'))
		{
			return (parse_commands_pipe(shell, envp));
		}
		else
			shell->status = 0;
		i++;
	}
	return (shell->status);
}
