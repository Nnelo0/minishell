/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:32:41 by cle-berr          #+#    #+#             */
/*   Updated: 2025/02/24 10:45:53 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	remove_exp_node(t_export **head, char *var_name)
{
	t_export	*temp;
	t_export	*prev;
	char		**name;

	temp = *head;
	prev = NULL;
	while (temp)
	{
		name = ft_split(temp->value, '=');
		if (ft_strcmp(name[0], var_name) == 0)
		{
			if (prev)
				prev->next = temp->next;
			else
				*head = temp->next;
			free_args(name);
			free(temp->value);
			free(temp);
			return ;
		}
		free_args(name);
		prev = temp;
		temp = temp->next;
	}
}

void	remove_env_node(t_env **head, char *var_name)
{
	t_env	*temp;
	t_env	*prev;
	char	**name;

	temp = *head;
	prev = NULL;
	while (temp)
	{
		name = ft_split(temp->value, '=');
		if (ft_strcmp(name[0], var_name) == 0)
		{
			if (prev)
				prev->next = temp->next;
			else
				*head = temp->next;
			free_args(name);
			free(temp->value);
			free(temp);
			return ;
		}
		free_args(name);
		prev = temp;
		temp = temp->next;
	}
}

int	ft_unset(t_shell *shell)
{
	char		**args;
	int			i;

	args = ft_split_quote(shell->tmp, ' ');
	i = 1;
	while (args[i])
	{
		remove_exp_node(&(shell->export_list), args[i]);
		remove_env_node(&(shell->env_list), args[i]);
		i++;
	}
	free_args(args);
	return (0);
}
