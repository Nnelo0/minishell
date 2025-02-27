/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 16:07:31 by cle-berr          #+#    #+#             */
/*   Updated: 2025/02/26 13:51:36 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*create_env_node(const char *env_var)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->value = ft_strdup(env_var);
	if (!new_node->value)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

void	append_env_node(t_env **head, const char *env_var)
{
	t_env	*new_node;

	new_node = create_env_node(env_var);
	if (!new_node)
		return ;
	if (*head == NULL)
		*head = new_node;
	else
		verif_env(head, env_var, new_node);
}

t_env	*init_env_list(char **env)
{
	t_env	*env_list;
	int		i;

	env_list = NULL;
	i = 0;
	while (env[i])
	{
		append_env_node(&env_list, env[i]);
		i++;
	}
	return (env_list);
}

void	free_env_list(t_env *env_list)
{
	t_env	*temp;

	while (env_list)
	{
		temp = env_list->next;
		free(env_list->value);
		free(env_list);
		env_list = temp;
	}
}
