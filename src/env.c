/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 09:36:00 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/31 10:18:47 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	ft_printf("%s\n", pwd);
	free(pwd);
	return (1);
}

int	ft_env(t_shell *shell)
{
	t_env	*temp;

	if (!shell || !shell->env_list)
		return (1);
	temp = shell->env_list;
	while (temp)
	{
		printf("%s\n", temp->value);
		temp = temp->next;
	}
	free(temp);
	return (1);
}
