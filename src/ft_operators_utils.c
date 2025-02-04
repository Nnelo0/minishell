/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_operators_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 09:39:51 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/04 13:38:58 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**ft_split_chevrons(char *input, int i, int j)
{
	char	**res;
	char	*start;

	if (!input)
		return (NULL);
	res = malloc(sizeof(char *) * (ft_strlen(input) + 1));
	if (!res)
		return (NULL);
	start = input;
	while (input[++i])
	{
		if (ft_strchr("<>| ", input[i]))
		{
			if (start != input + i)
				res[j++] = ft_substr(start, 0, input + i - start);
			if ((ft_strchr("<>", input[i]) && input[i] == input[i + 1]))
				res[j++] = ft_substr(input + i++, 0, 2);
			else if (input[i] != ' ')
				res[j++] = ft_substr(input + i, 0, 1);
			start = input + i + 1;
		}
	}
	if (start != input + i)
		res[j++] = ft_substr(start, 0, input + i - start);
	return (res[j] = NULL, res);
}

int	is_valid_chevrons(char *input)
{
	int	i;
	int	expect_file;

	i = 0;
	expect_file = 0;
	while (input[i])
	{
		if (input[i] == '<' || input[i] == '>')
		{
			if (input[i + 1] == input[i] && input[i + 2] == input[i])
				return (0);
			if (input[i + 1] == '\0')
				return (0);
			expect_file = 1;
		}
		else if (input[i] != ' ' && expect_file)
			expect_file = 0;
		i++;
	}
	return (expect_file == 0);
}

int	parse_in(t_shell *shell, int i)
{
	if (ft_strcmp(shell->ipt[i], "<") == 0)
	{
		if (shell->ipt[i + 1])
		{
			free(shell->in_file);
			shell->in_file = ft_strdup(shell->ipt[++i]);
		}
		else
			return (free(shell->in_file), free_args(shell->ipt),
				free(shell->cmd), 1);
	}
	return (0);
}

int	parse_out(t_shell *shell, int i, int *out_count, int *append)
{
	if (ft_strcmp(shell->ipt[i], ">") == 0
		|| ft_strcmp(shell->ipt[i], ">>") == 0)
	{
		if (ft_strcmp(shell->ipt[i], ">>") == 0)
			*append = 1;
		else
			*append = 0;
		if (shell->ipt[i + 1])
			shell->out_file[(*out_count)++] = ft_strdup(shell->ipt[++i]);
		else
			return (shell->out_file[(*out_count)] = NULL,
				free(shell->in_file), free_args(shell->ipt),
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
