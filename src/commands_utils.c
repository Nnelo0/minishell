/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 09:37:48 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/07 11:29:32 by cle-berr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	command_shell(char *input, char **envp)
{
	char	*path;
	char	*args[2];

	path = find_command_path(input, envp);
	if (!path)
	{
		ft_printf("command not found: %s\n", input);
		free(path);
		exit(EXIT_FAILURE);
	}
	args[0] = input;
	args[1] = NULL;
	execve(path, args, envp);
	ft_printf("command not found: %s\n", input);
	free(path);
	exit(EXIT_FAILURE);
}

void	ft_shell(char *input, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
		command_shell(input, envp);
	wait(NULL);
}
