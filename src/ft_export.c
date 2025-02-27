/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 11:25:25 by cle-berr          #+#    #+#             */
/*   Updated: 2025/02/25 08:47:16 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_export_no_arg(t_shell *shell)
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
	return (0);
}

void	equal_found(char *arg, t_shell *shell)
{
	t_env		*env_list;
	t_export	*export_list;

	env_list = shell->env_list;
	export_list = shell->export_list;
	append_env_node(&env_list, arg);
	append_exp_node(&export_list, arg, 1);
	ft_sort_export_list(export_list);
}

void	equal_not_found(char *arg, t_shell *shell)
{
	t_export	*export_list;

	export_list = shell->export_list;
	append_exp_node(&export_list, arg, 1);
	ft_sort_export_list(export_list);
}

int	ft_export_with_arg(char **args, t_shell *shell)
{
	int		i;
	int		status;

	i = 1;
	while (args[i])
	{
		status = 0;
		ft_remove_quotes(args[i]);
		status = ft_export_verif(args, i, status, 0);
		if (ft_strchr(args[i], '=') && status == 0)
			equal_found(args[i], shell);
		else if (status == 0)
			equal_not_found(args[i], shell);
		i++;
	}
	return (status == 1);
}

int	ft_export(t_shell *shell)
{
	char	**args;
	int		status;

	args = ft_split_quote(shell->tmp, ' ');
	if (!args[1])
		status = ft_export_no_arg(shell);
	else
		status = ft_export_with_arg(args, shell);
	free_args(args);
	return (status);
}
