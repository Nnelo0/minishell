/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_operators_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 09:39:51 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/17 15:35:27 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**ft_split_chevrons(char *input, int i, int j)
{
	char	**res;
	char	*start;

	if (!input)
		return (NULL);
	res = malloc(sizeof (char *) * (ft_strlen(input) + 1));
	if (!res)
		return (NULL);
	start = input;
	while (input[++i])
	{
		if ((input[i] == '<' || input[i] == '>') && (input[i + 1] == input[i]))
		{
			if (start != input + i)
				res[j++] = ft_substr(start, 0, input + i - start);
			res[j++] = ft_substr(input + i, 0, 2);
			i++;
			start = input + i + 1;
		}
		else if (input[i] == '<' || input[i] == '>' || input[i] == ' '
			|| input[i] == '|')
		{
			if (start != input + i)
				res[j++] = ft_substr(start, 0, input + i - start);
			if (input[i] != ' ')
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
