/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:01:33 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/06 17:42:40 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_yes(void)
{
	while (1)
		printf("y\n");
}

int	ft_exit(char *input)
{
	free(input);
	return (0);
}
void	ft_echo(char *input)
{
	int	i;
	int	j;

	i = 0;
	while (input[i])
	{
		if (input[i] == 34)
		{
			j = i;
			while (input[j])
			{
				if ()
			}
		}
	}
	if (input[5] == '-' && input[6] == 'n' && (input[7] == ' ' || input[7] == '\0'))
	{
		ft_putstr_fd(input + 8, 0);
		return ;
	}
	ft_putstr_fd(input + 5, 0);
	ft_printf("\n");

}

int	commands(char *input)
{
	if (ft_strncmp(input, "exit", 4) == 0 && (input[4] == ' ' || input[4] == '\0'))
			return(ft_exit(input));
	if (ft_strcmp(input, "yes") == 0)
		ft_yes();
	if (ft_strncmp(input, "echo", 4) == 0 && (input[4] == ' ' || input[4] == '\0'))
		ft_echo(input);
	return (1);
}
