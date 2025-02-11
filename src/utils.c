/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:01:16 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/11 15:23:20 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_all(t_shell *shell, char *input)
{
	free(input);
	free(shell->input);
	free_args(shell->args);
	free_args(shell->ipt);
	free_args(shell->exit);
	free_env_list(shell->env_list);
	free_export_list(shell->export_list);
}

int	ft_isdigit_neg(int c)
{
	if (('0' <= c && '9' >= c) || c == '-')
		return (1);
	return (0);
}

int	ft_isdigit_s(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (!ft_isdigit_neg(s[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_execute(char **args, char **envp, t_shell *shell)
{
	if (execve(args[0], args, envp) == -1)
	{
		perror(args[0]);
		free_args(shell->ipt);
		free(shell->input);
		free_args(args);
		free_env_list(shell->env_list);
		free_export_list(shell->export_list);
		exit(127);
	}
}

int	verif_shell(char *input, t_shell *shell)
{
	if ((input[0] == '\'' && input[1] == '\'' )
		|| (input[0] == '"' && input[1] == '"'))
		return (ft_printf("Command '' not found\n"), close(shell->in),
			close(shell->out), 127);
	if (input[0] == '.' && !input[1])
		return (ft_printf(".: filename argument required\n\
.: usage: . filename [arguments]\n"), close(shell->in), close(shell->out), 2);
	if (input[0] == '/' && !input[1])
		return (ft_printf("/: Is a directory\n"), close(shell->in),
			close(shell->out), 126);
	return (0);
}
