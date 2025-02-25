/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:11:29 by cle-berr          #+#    #+#             */
/*   Updated: 2025/02/25 15:15:05 by cle-berr         ###   ########.fr       */
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

char	*ft_dollar_env(char *args, t_shell *shell, int i)
{
	t_env	*temp;
	char	*tmp;
	char	**check_list;
	char	*check_args;

	temp = shell->env_list;
	check_args = malloc(sizeof(char) * ft_strlen(args) + 1);
	if (!check_args)
		return (NULL);
	while (args[++i])
		check_args[i] = args[i];
	check_args[i] = '\0';
	while (temp)
	{
		check_list = ft_split(temp->value, '=');
		if (ft_strcmp(check_args, check_list[0]) == 0)
		{
			tmp = ft_strdup(temp->value);
			tmp = get_after_equal(tmp);
			return (free_args(check_list), free(check_args), tmp);
		}
		free_args(check_list);
		temp = temp->next;
	}
	return (free(check_args), "");
}

char	*ft_dollar(char *input, t_shell *shell, int i, int j)
{
	char	*before;
	char	*var;
	char	*after;
	char	*value;
	char	*res;

	while (input[i] && input[i] != '$')
		i++;
	if (!input[i])
		return (res = ft_strdup(input), free(input), res);
	if (input[0] == '$' && !input[1])
		return (input);
	before = ft_strndup(input, i);
	j = i + 1;
	if (input[j++] == '?')
		value = ft_itoa(shell->status);
	else
	{
		while (input[j] && is_valid_var_char(input[j]))
			j++;
		var = ft_strndup(input + i + 1, j - i - 1);
		value = ft_dollar_env(var, shell, -1);
		free(var);
	}
	after = ft_strdup(input + j);
	var = ft_strjoin(before, value);
	res = ft_strjoin(var, after);
	if (ft_strcmp(value, "") != 0)
		free(value);
	return (free(var), free(before), free(after),
		free(input), ft_dollar(res, shell, 0, 0));
}

char	**get_commande_utils(t_shell *shell, int i, char **res)
{
	int	j;
	char	*tmp;

	j = 0;
	while (shell->input[++i])
	{
		tmp = shell->input[i];
		if (ft_strchr(tmp, '$') && !ft_strchr(tmp, '\''))
		{
			ft_remove_quotes(tmp);
			tmp = ft_dollar(tmp, shell, 0, 0);
			if (!tmp || tmp[0] == '\0')
			{
				free(tmp);
				continue ;
			}
		}
		res[j] = ft_strdup(tmp);
		if (ft_strcmp(res[0], "echo") != 0)
			res[j] = get_command_from_path(res[j]);
		j++;
		if (tmp != shell->input[i])
			free(tmp);
	}
	return (res[j] = NULL, res);
}

char	**get_command(t_shell *shell, int i)
{
	int		count;
	char	**res;

	count = 0;
	while (shell->input[count])
		count++;
	res = ft_calloc(count + 1, sizeof(char *));
	if (!res)
		return (shell->input);
	res = get_commande_utils(shell, i, res);
	if (shell->input && shell->input[0])
		free_args(shell->input);
	return (res);
}

char	*get_command_from_path(char *input)
{
	char	*command;
	char	*tmp;
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
