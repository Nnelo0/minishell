/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnelo <nnelo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:01:16 by ebroudic          #+#    #+#             */
/*   Updated: 2025/03/05 19:05:33 by nnelo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_all(t_shell *shell)
{
	free_args(shell->input);
	free(shell->tmp);
	free_env_list(shell->env_list);
	free_export_list(shell->export_list);
}

int	ft_isdigit_neg(int c)
{
	if (('0' <= c && '9' >= c) || c == '-' || c == '+')
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
		free(shell->tmp);
		free_args(args);
		free_args(envp);
		free_env_list(shell->env_list);
		free_export_list(shell->export_list);
		exit(127);
	}
}

char	**merge_args(char **input, char *c, int i, int j)
{
	char	**new_input;
	char	*tmp;
	char	*cmd;
	char	*new_cmd;

	new_input = malloc(sizeof(char *) * (ft_strlen_tab(input) + 1));
	if (!new_input)
		return (perror("malloc"), NULL);
	while (input[++i])
	{
		cmd = ft_strdup(input[i]);
		while (input[i + 1] && !(ft_strcmp(input[i + 1], c) == 0))
		{
			tmp = ft_strjoin(cmd, " ");
			new_cmd = ft_strjoin(tmp, input[i + 1]);
			free(tmp);
			free(cmd);
			cmd = new_cmd;
			i++;
		}
		new_input[j++] = cmd;
		if (input[i + 1] && ft_strcmp(input[i + 1], c) == 0)
			new_input[j++] = ft_strdup(input[++i]);
	}
	return (free_args(input), new_input[j] = NULL, new_input);
}
