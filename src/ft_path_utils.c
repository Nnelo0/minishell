/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_path_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 12:22:03 by cle-berr          #+#    #+#             */
/*   Updated: 2025/02/27 13:58:15 by cle-berr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_after_equal(char *arg)
{
	char	*res;
	int		i;
	int		j;

	i = 1;
	j = 0;
	res = malloc(sizeof(char) * ft_strlen(arg) + 1);
	while (arg[i - 1] != '=')
		i++;
	while (arg[i])
		res[j++] = arg[i++];
	free(arg);
	res[j] = '\0';
	return (res);
}

int	is_valid_var_char(char c)
{
	return (c == '_' || (c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'));
}

char	*get_command_from_path(char *input)
{
	char		*command;
	char		*tmp;
	struct stat	path_stat;

	if (ft_strcmp(input, "") == 0)
		return (input);
	tmp = ft_strdup(input);
	free(input);
	if (ft_strncmp(tmp, "/", 1) == 0)
	{
		if (access(tmp, X_OK) == 0)
		{
			if (stat(tmp, &path_stat) == 0 && S_ISREG(path_stat.st_mode))
			{
				command = ft_strrchr(tmp, '/') + 1;
				input = ft_strdup(command);
				free(tmp);
				return (input);
			}
		}
	}
	return (tmp);
}

int	quote_isgood(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '"')
		{
			i++;
			while (input[i] != '"')
				i++;
		}
		if (input[i] == '\'')
			return (0);
		i++;
	}
	return (1);
}
