/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 11:25:25 by cle-berr          #+#    #+#             */
/*   Updated: 2025/01/09 12:15:43 by cle-berr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	**copy_env(char **envp)
{
	int		i;
	char	**copy;

	i = 0;
	while (envp[i])
		i++;
	copy = malloc((i + 1) * sizeof(char *));
	if (!copy)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		copy[i] = strdup(envp[i]);
		if (!copy[i])
			return (NULL);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

static void	sort_env(char **env)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (env[i])
	{
		j = i + 1;
		while (env[j])
		{
			if (strcmp(env[i], env[j]) > 0)
			{
				temp = env[i];
				env[i] = env[j];
				env[j] = temp;
			}
			j++;
		}
		i++;
	}
}

static void	print_sorted(char **sorted_env)
{
	int	i;

	i = 0;
	while (sorted_env[i])
	{
		printf("%s\n", sorted_env[i]);
		i++;
	}
}

static void	ft_export_no_arg(char **envp)
{
	char	**sorted_env;
	int		i;

	i = 0;
	sorted_env = copy_env(envp);
	if (!sorted_env)
	{
		perror("minishell: malloc");
		return ;
	}
	sort_env(sorted_env);
	print_sorted(sorted_env);
	while (sorted_env[i])
		free(sorted_env[i++]);
	free(sorted_env);
}

int	ft_export(char **envp, t_shell *shell)
{
	if (!shell->args[1])
		ft_export_no_arg(envp);
	else if (!shell->args[2])
		ft_export_with_arg(envp);
	else
		return (printf("minishell: export: too many arguments\n"));
	return (1);
}
