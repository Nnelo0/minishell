/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:01:16 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/11 10:52:15 by cle-berr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_all(t_shell *shell, char *input)
{
	free(input);
	free(shell->input);
	free(shell->verif);
	free_args(shell->args);
	free_args(shell->ipt);
	free_args(shell->exit);
	free_env_list(shell->env_list);
	free_export_list(shell->export_list);
}

int	ft_isdigit_neg(int c)
{
	if (('0' <= c && '9' >= c) || c == '-')
		return (1);
	return (0);
}

int	ft_isdigit_s(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (!ft_isdigit_neg(s[i]))
			return (0);
		i++;
	}
	return (1);
}
