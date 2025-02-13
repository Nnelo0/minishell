/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:11:29 by cle-berr          #+#    #+#             */
/*   Updated: 2025/02/12 15:56:27 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*find_command_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;
	char	*part_path;

	i = 0;
	while (envp[i] && ft_strnstr(envp[i], "PATH=", 5) == 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
			return (free_args(paths), path);
		free(path);
		i++;
	}
	free_args(paths);
	return (0);
}

char	*get_command_from_path(char *input)
{
	char	*command;
	char	*tmp;

	tmp = ft_strdup(input);
	free(input);
	if (ft_strncmp(tmp, "/", 1) == 0)
	{
		if (access(tmp, X_OK) == 0)
		{
			command = ft_strrchr(tmp, '/') + 1;
			input = ft_strdup(command);
			free(tmp);
			return (input);
		}
		else
			return (tmp);
	}
	return (tmp);
}
