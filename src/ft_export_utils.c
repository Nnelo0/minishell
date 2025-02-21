/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 11:25:25 by cle-berr          #+#    #+#             */
/*   Updated: 2025/02/21 14:37:47 by ebroudic         ###   ########.fr       */
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

int	ft_export_verif(char **args, int i, int status, int j)
{
	if (!ft_isalpha(args[i][j]))
	{
		if (args[i][0] == '=')
			return (status = print_error(args[i], status), status);
		while (args[i][j])
		{
			if (args[i][j] == '=')
				break ;
			if ((!ft_isalnum(args[i][j++]) && status == 0)
				|| (ft_isdigit(args[i][0]) && status == 0))
				status = print_error(args[i], status);
		}
	}
	else
	{
		while (args[i][j])
		{
			if (args[i][j] == '=')
				break ;
			if (args[i][j++] == '-' && status == 0)
				status = print_error(args[i], status);
		}
	}
	return (status);
}
