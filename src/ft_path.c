/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:11:29 by cle-berr          #+#    #+#             */
/*   Updated: 2025/02/19 17:13:27 by cle-berr         ###   ########.fr       */
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
	return(res);
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

char	*ft_dollar(char *args, t_shell *shell, int i, int j)
{
	char	*after;
	char	*before;
	char	*var;
	char	*res;
	int		h;

	if (!args)
		return (NULL);
	h = 0;
	before = calloc(i + 1, sizeof(char));
	if (!before)
		return (NULL);
	after = calloc(ft_strlen(args + i) + 1, sizeof(char));
	var = calloc(ft_strlen(args + i) + 1, sizeof(char));
	if (!after || !var)
		return (free(before), free(after), free(var), NULL);
	while (args[i] && args[i] != '$')
		before[h++] = args[i++];
	before[h] = '\0';
	if (!args[i + 1])
		return (free(after), res = ft_strjoin(before, "$"), free(before), res);
	while (args[++i])
	{
		if (args[i] == 34 || args[i] == 39 || (ft_isdigit(args[i - 1]) && args[i - 2] == '$'))
			break ;
		var[j++] = args[i];
	}
	var[j] = '\0';
	h = 0;
	while (args[i])
		after[h++] = args[i++];
	after[h] = '\0';
	ft_remove_quotes(after);
	if (var[0] == '0')
	{
		free(var);
		var = ft_strdup("./minishell");
	}
	else
	{
		char *temp = ft_dollar_env(var, shell, -1);
		free(var);
		var = temp;
	}
	char *temp = ft_strjoin(before, var);
	free(before);
	free(var);
	res = ft_strjoin(temp, after);
	free(temp);
	free(after);
	return (res);
}


/* {
	char	*after;
	char	*before;
	char	*var;
	char	*res;
	int		h;

	h = 0;
	before = calloc(ft_strlen(args) + 1, sizeof(char));
	after = calloc(ft_strlen(args) + 1, sizeof(char));
	var = calloc(ft_strlen(args) + 1, sizeof(char));
	if (!before || !after || !var)
		return (free(before), free(after), free(var), NULL);
	while (args[i] != '$')
		before[h++] = args[i++];
	before[h] = '\0';
	if (!args[i + 1])
		return (free(after), res = ft_strjoin(before, "$"), free(before), res);
	while (args[++i])
	{
		if (args[i] == 34 || args[i] == 39
			|| (ft_isdigit(args[i - 1]) && args[i - 2] == '$'))
			break ;
		var[j++] = args[i];
	}
	var[j] = '\0';
	h = 0;
	while (args[i])
		after[h++] = args[i++];
	after[h] = '\0';
	ft_remove_quotes(after);
	if (var[0] == '0')
		var = ft_strdup("./minishell");
	else
		var = ft_dollar_env(var, shell, -1);
	res = ft_strjoin(before, var);
	res = ft_strjoin(res, after);
	return (var);
} */

/* {
	if (!args[1])
		printf("$");
	if (args[1] == 34 || args[1] == 39)
	{
		ft_remove_quotes(args);
		while (args[i])
			printf("%c", args[i++]);
		return ;
	}
	ft_remove_quotes(args);
	if (args[1] == '0')
	{
		free(args);
		return("./minishell");
	}
	if (ft_isdigit(args[1]))
		while (args[++i])
			printf("%c", args[i]);
	else if (args[1] == '?')
	{
		printf("%d", shell->status);
		while (args[++i])
			printf("%c", args[i]);
	}
	else if (ft_isalpha(args[1]))
		ft_dollar_alpha(args, shell, -1);
} */

void	get_command(t_shell *shell)
{
	int	i;

	i = -1;
	while (shell->input[++i])
	{
		if (ft_strchr(shell->input[i], '$') != NULL)
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
