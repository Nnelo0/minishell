/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:11:29 by cle-berr          #+#    #+#             */
/*   Updated: 2025/02/21 11:05:01 by cle-berr         ###   ########.fr       */
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

int	is_valid_var_char(char c)
{
	return (c == '_' || (c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'));
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
		return (ft_strdup(input));
	before = ft_strndup(input, i);
	j = i + 1;
	if (input[j] == '?')
	{
		value = ft_itoa(shell->status);
		j++;
	}
	else
	{
		while (input[j] && is_valid_var_char(input[j]))
			j++;
		var = ft_strndup(input + i + 1, j - i - 1);
		value = ft_dollar_env(var, shell, -1);
		free(var);
	}
	return (after = ft_strdup(input + j), var = ft_strjoin(before, value),
		res = ft_strjoin(var, after), free(var), free(before), free(after),
		free(input), ft_dollar(res, shell, 0, 0));
}

void	get_command(t_shell *shell)
{
	int	i;

	i = -1;
	while (shell->input[++i])
	{
		if (ft_strchr(shell->input[i], '$') != NULL && ft_strchr(shell->input[i], '\'') == NULL)
			shell->input[i] = ft_dollar(shell->input[i], shell, 0, 0);
		ft_remove_quotes(shell->input[0]);
		shell->input[i] = get_command_from_path(shell->input[i]);
	}
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
