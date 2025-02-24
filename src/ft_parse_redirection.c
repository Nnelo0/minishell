/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_redirection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 09:39:51 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/24 10:32:52 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	parse_commands(char **commands, char *tmp, char *args)
{
	char	*tmp_cmd;

	tmp = ft_strjoin(*commands, " ");
	tmp_cmd = ft_strjoin(tmp, args);
	free(tmp);
	free(*commands);
	*commands = tmp_cmd;
}

int	parse_in(t_shell *shell, int i)
{
	if (ft_strcmp(shell->copy[i], "<") == 0)
	{
		if (shell->copy[i + 1])
		{
			free(shell->in_file);
			shell->in_file = ft_strdup(shell->copy[++i]);
		}
		else
			return (free(shell->in_file), free_args(shell->copy),
				free(shell->cmd), 1);
	}
	return (0);
}

int	parse_out(t_shell *shell, int i, int *out_count, int *append)
{
	if (ft_strcmp(shell->copy[i], ">") == 0
		|| ft_strcmp(shell->copy[i], ">>") == 0)
	{
		if (ft_strcmp(shell->copy[i], ">>") == 0)
			*append = 1;
		else
			*append = 0;
		if (shell->copy[i + 1])
			shell->out_file[(*out_count)++] = ft_strdup(shell->copy[++i]);
		else
			return (shell->out_file[(*out_count)] = NULL,
				free(shell->in_file), free_args(shell->copy),
				free(shell->cmd), 1);
	}
	return (0);
}

int	ft_strlen_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return (0);
	while (tab[i] != NULL)
		i++;
	return (i);
}

char	**copy_string(char **input)
{
	int		i;
	int		len;
	char	**copy;

	i = 0;
	len = ft_strlen_tab(input);
	copy = malloc(sizeof(char *) * (len +1));
	while (i < len)
	{
		copy[i] = ft_strdup(input[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}
