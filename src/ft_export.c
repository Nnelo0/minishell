/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 11:25:25 by cle-berr          #+#    #+#             */
/*   Updated: 2025/01/30 14:22:10 by cle-berr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_export_no_arg(t_shell *shell)
{
	t_export	*temp;
	int			i;
	int			status;

	temp = shell->export_list;
	while (temp)
	{
		i = 0;
		status = 0;
		printf("declare -x ");
		while (temp->value[i])
		{
			printf("%c", temp->value[i]);
			if (temp->value[i] == '=' && status++ == 0)
				printf("\"");
			i++;
		}
		if (status >= 1)
			printf("\"");
		printf("\n");
		temp = temp->next;
	}
	free(temp);
}

void	equal_found(char *arg, t_shell *shell)
{
	t_env		*env_list;
	t_export	*export_list;

	env_list = shell->env_list;
	export_list = shell->export_list;
	ft_remove_quotes(arg);
	append_env_node(&env_list, arg);
	append_export_node(&export_list, arg);
	ft_sort_export_list(export_list);
}

void	equal_not_found(char *arg, t_shell *shell)
{
	t_export	*export_list;

	export_list = shell->export_list;
	ft_remove_quotes(arg);
	append_export_node(&export_list, arg);
	ft_sort_export_list(export_list);
}

void	ft_export_with_arg(char **args, t_shell *shell)
{
	int		i;

	i = 1;
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
			equal_found(args[i], shell);
		else
			equal_not_found(args[i], shell);
		i++;
	}
}

int	ft_export(t_shell *shell)
{
	char	**args;

	args = ft_split_export(shell->input, ' ');
	if (!args[1])
		ft_export_no_arg(shell);
	else
		ft_export_with_arg(args, shell);
	free_args(args);
	return (1);
}
