/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:18:50 by cle-berr          #+#    #+#             */
/*   Updated: 2025/01/28 16:10:27 by cle-berr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	env_utils(int type, t_env *prev, t_env *temp, t_env *new_node)
{
	if (type == 1)
		prev->next = new_node;
	else
	{
		new_node->next = temp->next;
		prev->next = new_node;
		free(temp->value);
		free(temp);
	}
}

void	verif_env(t_env **head, const char *env_var, t_env *new_node)
{
	t_env	*temp;
	t_env	*prev;
	char	**check_args;
	char	**check_list;
	int		type;

	check_args = ft_split(env_var, '=');
	temp = *head;
	prev = NULL;
	type = 1;
	while (temp)
	{
		check_list = ft_split(temp->value, '=');
		if (ft_strcmp(check_args[0], check_list[0]) == 0)
		{
			type = 0;
			free_args(check_list);
			break ;
		}
		free_args(check_list);
		prev = temp;
		temp = temp->next;
	}
	env_utils(type, prev, temp, new_node);
	free_args(check_args);
}

void	export_utils(int type, t_export *pv, t_export *temp, t_export *new_node)
{
	if (type == 1)
		pv->next = new_node;
	else
	{
		new_node->next = temp->next;
		pv->next = new_node;
		free(temp->value);
		free(temp);
	}
}

void	verif_export(t_export **head, const char *exp_var, t_export *new)
{
	t_export	*temp;
	t_export	*prev;
	char		**check_args;
	char		**check_list;
	int			type;

	check_args = ft_split(exp_var, '=');
	temp = *head;
	prev = NULL;
	type = 1;
	while (temp)
	{
		check_list = ft_split(temp->value, '=');
		if (ft_strcmp(check_args[0], check_list[0]) == 0)
		{
			type = 0;
			free_args(check_list);
			break ;
		}
		free_args(check_list);
		prev = temp;
		temp = temp->next;
	}
	export_utils(type, prev, temp, new);
	free_args(check_args);
}
