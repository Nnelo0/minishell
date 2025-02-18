/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 09:36:00 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/18 13:06:27 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_sizeenv(t_env *lst)
{
	int	len;

	len = 0;
	while (lst != NULL)
	{
		len++;
		lst = lst->next;
	}
	return (len);
}

char	**env_in_stars(t_shell *shell)
{
	t_env	*env;
	char	**env_stars;
	int		i;

	env = shell->env_list;
	env_stars = malloc(sizeof(char *) * (ft_sizeenv(env) + 1));
	if (!env_stars)
		return (NULL);
	i = 0;
	while (env)
	{
		env_stars[i] = ft_strdup(env->value);
		if (!env_stars[i])
		{
			while (i > 0)
				free(env_stars[--i]);
			free(env_stars);
			return (NULL);
		}
		env = env->next;
		i++;
	}
	env_stars[i] = NULL;
	return (env_stars);
}
