/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 11:25:25 by cle-berr          #+#    #+#             */
/*   Updated: 2025/01/21 17:23:09 by cle-berr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_export_no_arg(t_shell *shell)
{
	t_export	*temp;

	if (!shell || !shell->export_list)
		return (1);
	temp = shell->export_list;
	while (temp)
	{
		printf("%s\n", temp->value);
		temp = temp->next;
	}
	free(temp);
	return (1);
}

int ft_export_whith_arg(t_shell *shell)
{
	int i;
	int j;

	i = 0;
	while(shell->args[i])
	{
		j = 0;
		while (shell->args[i][j])
		{
			
			j++;
		}
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
