/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:11:29 by cle-berr          #+#    #+#             */
/*   Updated: 2025/02/26 15:42:51 by cle-berr         ###   ########.fr       */
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

char	*ft_dollar_env(char *args, t_shell *shell)
{
	t_env	*temp;
	char	*tmp;
	char	**check_list;
	char	*check_args;

	temp = shell->env_list;
	check_args = ft_strdup(args);
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

char	*ft_dollar(char *in, t_shell *shell, int i, int j)
{
	while (in[i] && in[i] != '$')
		i++;
	if (!in[i] || (in[i] == '$' && (!in[i + 1] || in[i + 1] == ' ')))
		return (in);
	shell->before = ft_strndup(in, i);
	j = i + 1;
	if (in[j++] == '?')
		shell->value = ft_itoa(shell->status);
	else
	{
		while (in[j] && is_valid_var_char(in[j]))
			j++;
		shell->var = ft_strndup(in + i + 1, j - i - 1);
		shell->value = ft_dollar_env(shell->var, shell);
		free(shell->var);
	}
	shell->after = ft_strdup(in + j);
	shell->var = ft_strjoin(shell->before, shell->value);
	shell->res = ft_strjoin(shell->var, shell->after);
	if (ft_strcmp(shell->value, "") != 0)
		free(shell->value);
	return (free(shell->var), free(shell->before), free(shell->after),
		free(in), ft_dollar(shell->res, shell, 0, 0));
}

char	**get_commande_utils(t_shell *shell, int i, char **res)
{
	int		j;
	char	*tmp;

	j = 0;
	while (shell->input[++i])
	{
		tmp = ft_strdup(shell->input[i]);
		if (ft_strchr(tmp, '$') && !ft_strchr(tmp, '\''))
		{
			ft_remove_quotes(tmp);
			tmp = ft_dollar(tmp, shell, 0, 0);
			if (!tmp || tmp[0] == '\0')
			{
				free(tmp);
				continue ;
			}
			tmp = ft_add_quotes(tmp);
		}
		res[j] = ft_strdup(tmp);
		res[j] = get_command_from_path(res[j]);
		j++;
		free(tmp);
	}
	return (res[j] = NULL, res);
}

char	**get_command(t_shell *shell, int i)
{
	char	**res;

	res = ft_calloc(ft_strlen_tab(shell->input) + 1, sizeof(char *));
	if (!res)
		return (shell->input);
	res = get_commande_utils(shell, i, res);
	free_args(shell->input);
	return (res);
}
