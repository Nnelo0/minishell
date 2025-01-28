/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 11:25:25 by cle-berr          #+#    #+#             */
/*   Updated: 2025/01/28 16:26:13 by cle-berr         ###   ########.fr       */
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
			if (temp->value[i] == '=' && status == 0)
			{
				printf("\"");
				status++;
			}
			i++;
		}
		printf("\"\n");
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
}

void	ft_export_with_arg(char *input, t_shell *shell)
{
	int		i;
	char	**args;

	i = 1;
	args = ft_split_export(input, ' ');
	while (args[i])
	{
		printf("%s\n", args[i]);
		i++;
	}
	i = 1;
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
			equal_found(args[i], shell);
		//else
		//	equal_not_found();
		i++;
	}
}

int	ft_export(char *input, t_shell *shell)
{
	if (!shell->args[1])
		ft_export_no_arg(shell);
	else
		ft_export_with_arg(input, shell);
	return (1);
}
