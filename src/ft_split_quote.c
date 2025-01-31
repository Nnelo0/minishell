/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 16:29:58 by cle-berr          #+#    #+#             */
/*   Updated: 2025/01/30 16:01:58 by cle-berr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	count_word(char const *s, char c)
{
	int		i;
	int		count;
	int		quote;

	count = 0;
	i = 0;
	quote = 1;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			i++;
		else if (s[i] != c)
		{
			count++;
			while (s[i] != '\0')
			{
				if (s[i] == 39 || s[i] == 34)
					quote = !quote;
				if (s[i] == c && quote == 1)
					break ;
				i++;
			}
		}
	}
	return (count);
}

static char	*ft_strndup(const char *s, size_t n)
{
	char	*dest;
	size_t	i;

	i = 0;
	dest = malloc(sizeof(char) * (n + 1));
	if (dest == NULL)
		return (NULL);
	while (i < n)
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

static int	dup_word(char **dsa, const char *s, char c, int *i)
{
	int	quote;
	int	j;

	j = 0;
	quote = 1;
	while (s[j] != '\0')
	{
		if (s[j] == 39 || s[j] == 34)
			quote = -quote;
		if (s[j] == c && quote == 1)
			break ;
		j++;
	}
	dsa[*i] = ft_strndup(s, j);
	if (!dsa[*i])
	{
		while (*i > 0)
			free(dsa[--(*i)]);
		free(dsa);
		return (0);
	}
	(*i)++;
	return (1);
}

static int	copy_word(char **dsa, const char *s, char c)
{
	int		i;
	int		quote;

	i = 0;
	quote = 1;
	while (*s != '\0')
	{
		if (*s != c)
		{
			if (!dup_word(dsa, s, c, &i))
				return (0);
			while (*s != '\0')
			{
				if (*s == 39 || *s == 34)
					quote = -quote;
				if (*s == c && quote == 1)
					break ;
				s++;
			}
		}
		else
			s++;
	}
	dsa[i] = NULL;
	return (1);
}

char	**ft_split_quote(char const *s, char c)
{
	char	**dsa;

	if (s == NULL)
		return (NULL);
	dsa = malloc(sizeof(char *) * (count_word(s, c) + 1));
	if (dsa == NULL)
		return (NULL);
	if (!copy_word(dsa, s, c))
		return (NULL);
	return (dsa);
}
