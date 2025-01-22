/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 11:25:25 by cle-berr          #+#    #+#             */
/*   Updated: 2025/01/22 13:21:16 by cle-berr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_export_no_arg(t_shell *shell)
{
	t_export	*temp;

	if (!shell || !shell->export_list)
		return ;
	temp = shell->export_list;
	while (temp)
	{
		printf("declare -x %s\n", temp->value);
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
	printf("%s\n", arg);
}

void	ft_export_with_arg(t_shell *shell)
{
	int	i;

	i = 1;
	while (shell->args[i])
	{
		if (ft_strchr(shell->args[i], '='))
			equal_found(shell->args[i], shell);
		//else
		//	equal_not_found();
		i++;
	}
}

int	ft_export(t_shell *shell)
{
	if (!shell->args[1])
		ft_export_no_arg(shell);
	else
		ft_export_with_arg(shell);
	return (1);
}
