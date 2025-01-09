/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 11:25:25 by cle-berr          #+#    #+#             */
/*   Updated: 2025/01/09 16:39:35 by cle-berr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sort_list(t_list **head)
{
	t_list	*current;
	t_list	*next_node;
	char	*temp;

	if (!head || !*head)
		return ;
	current = *head;
	while (current)
	{
		next_node = current->next;
		while (next_node)
		{
			if (strcmp(current->content, next_node->content) > 0)
			{
				temp = current->content;
				current->content = next_node->content;
				next_node->content = temp;
			}
			next_node = next_node->next;
		}
		current = current->next;
	}
}

void	print_sorted_list(t_list *head)
{
	while (head)
	{
		printf("declare -x %s\n", head->content);
		head = head->next;
	}
}

void	free_list(t_list *head)
{
	t_list	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->content);
		free(temp);
	}
}

static void	ft_export_no_arg(t_shell *shell, char **envp)
{
	t_list	*env_list;

	env_list = copy_env(shell, envp);
	if (!env_list)
		return ;
	sort_list(&env_list);
	print_sorted_list(env_list);
	free_list(env_list);
}

int	ft_export(char **envp, t_shell *shell)
{
	if (!shell->args[1])
		ft_export_no_arg(shell, envp);
	else
		ft_export_with_arg(envp);
	return (1);
}
