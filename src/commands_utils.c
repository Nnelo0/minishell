/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 09:37:48 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/07 12:29:56 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	command_shell(char *input, char **envp, t_shell *shell)
{
	char	*path;

	path = find_command_path(*shell->args, envp);
	if (!path)
	{
		ft_printf("command not found: %s\n", input);
		free(path);
		exit(EXIT_FAILURE);
	}
	execve(path, shell->args, envp);
	ft_printf("command not found: %s\n", input);
	free(path);
	exit(EXIT_FAILURE);
}

void	ft_shell(char *input, char **envp, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
		command_shell(input, envp, shell);
	wait(NULL);
}
