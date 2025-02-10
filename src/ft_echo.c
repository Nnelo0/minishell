/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:17:37 by nnelo             #+#    #+#             */
/*   Updated: 2025/02/10 15:04:23 by cle-berr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_quotes(char *input)
{
	int	i;
	int	single_open;
	int	double_open;

	i = 0;
	single_open = 0;
	double_open = 0;
	while (input[i])
	{
		if (input[i] == 39 && double_open == 0)
			single_open = !single_open;
		else if (input[i] == 34 && single_open == 0)
			double_open = !double_open;
		i++;
	}
	if (single_open || double_open)
		return (0);
	return (1);
}

void	print(char **args, int i)
{
	ft_remove_quotes(args[i]);
	printf("%s", args[i]);
	if (args[i + 1])
		printf(" ");
}

int	ft_dollar_alpha(char *args, t_shell *shell, int i)
{
	t_env	*temp;
	char	*tmp;
	char	**check_list;
	char	*check_args;

	temp = shell->env_list;
	check_args = malloc(sizeof(char) * ft_strlen(args) + 1);
	if (!check_args)
		return (perror("malloc"), 127);
	while (args[++i + 1])
		check_args[i] = args[i + 1];
	check_args[i] = '\0';
	while (temp)
	{
		check_list = ft_split(temp->value, '=');
		if (ft_strcmp(check_args, check_list[0]) == 0)
		{
			tmp = ft_strdup(temp->value);
			printf("%s", tmp);
			return (free(tmp), free_args(check_list), free(check_args), 1);
		}
		free_args(check_list);
		temp = temp->next;
	}
	return (free(check_args), 1);
}

void	ft_dollar(char *args, t_shell *shell)
{
	int		i;

	i = 1;
	ft_remove_quotes(args);
	if (!args[1])
		printf("$");
	if (args[1] == '0')
		printf("./minishell");
	if (ft_isdigit(args[1]))
		while (args[++i])
			printf("%c", args[i]);
	else if (args[1] == '?')
	{
		printf("%d", shell->status);
		while (args[++i])
			printf("%c", args[i]);
	}
	else if (ft_isalpha(args[1]))
		ft_dollar_alpha(args, shell, -1);
}

int	ft_echo(char *input, t_shell *shell, int n, int i)
{
	char	**args;
	int		j;

	j = 0;
	args = ft_split_quote(input, ' ');
	if (!args[1])
		printf("\n");
	else
	{
		if (ft_strcmp(args[1], "-n") == 0)
			n = 1;
		if (args[i][0] == '"')
			j += 1;
		if (args[i][j] == '$')
			ft_dollar(args[i], shell);
		else if ((i == 1 && n != 1) || i != 1)
			print(args, i);
		if (args[i + 1])
			ft_echo(input, shell, n, i + 1);
		if (n != 1 && i == 1)
			printf("\n");
	}
	free_args(args);
	shell->status = 0;
	return (1);
}
