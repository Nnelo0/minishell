/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:04:05 by cle-berr          #+#    #+#             */
/*   Updated: 2025/02/26 12:38:43 by cle-berr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_export	*create_export_node(const char *export_var)
{
	t_export	*new_node;

	new_node = (t_export *)malloc(sizeof(t_export));
	if (!new_node)
		return (NULL);
	new_node->value = ft_strdup(export_var);
	if (!new_node->value)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

void	append_exp_node(t_export **head, const char *export_var, int tpe)
{
	t_export	*new_node;

	new_node = create_export_node(export_var);
	if (!new_node)
		return ;
	if (*head == NULL)
	{
		*head = new_node;
	}
	else
		verif_exp(head, export_var, new_node, tpe);
}

void	ft_sort_export_list(t_export *export_list)
{
	t_export	*current;
	t_export	*next;
	char		*temp;

	if (!export_list)
		return ;
	current = export_list;
	while (current)
	{
		next = current->next;
		while (next)
		{
			if (ft_strcmp(current->value, next->value) > 0)
			{
				temp = current->value;
				current->value = next->value;
				next->value = temp;
			}
			next = next->next;
		}
		current = current->next;
	}
}

t_export	*init_export_list(char **env)
{
	t_export	*export_list;
	int			i;

	export_list = NULL;
	i = 0;
	while (env[i])
	{
		append_exp_node(&export_list, env[i], 1);
		i++;
	}
	ft_sort_export_list(export_list);
	return (export_list);
}

void	free_export_list(t_export *export_list)
{
	t_export	*temp;

	while (export_list)
	{
		temp = export_list->next;
		free(export_list->value);
		free(export_list);
		export_list = temp;
	}
}
