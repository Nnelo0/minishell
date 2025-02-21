/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 11:25:25 by cle-berr          #+#    #+#             */
/*   Updated: 2025/02/21 08:48:50 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	print_error(char *args, int status)
{
	ft_putstr_fd("export: `", 2);
	ft_putstr_fd(args, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (status = 1);
}

int	ft_export_verif(char **args, int i, int status)
{
	int		j;

	j = 0;
	if (!ft_isalpha(args[i][j]))
	{
		while (args[i][j])
		{
			if (!ft_isalnum(args[i][j]) && status == 0)
				status = print_error(args[i], status);
			if (ft_isdigit(args[i][0]) && status == 0)
				status = print_error(args[i], status);
			j++;
		}
	}
	else
	{
		while (args[i][j])
		{
			if (args[i][j] == '-' && status == 0)
				status = print_error(args[i], status);
			j++;
		}
	}
	return (status);
}
