/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 11:25:25 by cle-berr          #+#    #+#             */
/*   Updated: 2025/02/25 12:21:30 by cle-berr         ###   ########.fr       */
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

int	ft_export_verif(char **arg, int i, int status, int j)
{
	if (!ft_isalpha(arg[i][j]))
	{
		if (arg[i][0] == '=')
			return (status = print_error(arg[i], status), status);
		while (arg[i][j])
		{
			if (arg[i][j] == '=')
				break ;
			if (((!ft_isalnum(arg[i][j]) && status == 0)
				|| (ft_isdigit(arg[i][0]) && status == 0)) && arg[i][j] != '|')
				status = print_error(arg[i], status);
			j++;
		}
	}
	else
	{
		while (arg[i][j])
		{
			if (arg[i][j] == '=')
				break ;
			if (arg[i][j++] == '-' && status == 0)
				status = print_error(arg[i], status);
		}
	}
	return (status);
}
