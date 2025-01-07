/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:01:33 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/07 14:21:03 by cle-berr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_cd(t_shell *shell)
{
	char	*target;
	char	cwd[1024];

	if (!shell->args[1])
	{
		target = getenv("HOME");
		if (!target)
			return (printf("cd: HOME not set\n"));
	}
	else if (shell->args[2])
		return (fprintf(stderr, "cd: too many arguments\n"));
	else
		target = shell->args[1];
	if (chdir(target) == -1)
	{
		perror("cd");
		return (1);
	}
	if (getcwd(cwd, sizeof(cwd)) && setenv("PWD", cwd, 1) == -1)
	{
		perror("cd: Failed to set PWD");
		return (1);
	}
	return (1);
}

int	ft_exit(char *input)
{
	printf("exit\n");
	free(input);
	return (0);
}

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
		if (input[i] == 39)
			single_open = !single_open;
		else if (input[i] == 34)
			double_open = !double_open;
		i++;
	}
	if (single_open || double_open)
		return (0);
	return (1);
}

void	ft_echo(char *input)
{
	int	i;

	if (input[5] == '-' && input[6] == 'n'
		&& (input[7] == ' ' || input[7] == '\0'))
	{
		i = 8;
		while (input[i])
		{
			while (input[i] == 34 || input[i] == 39)
				i++;
			write(1, &input[i], 1);
			i++;
		}
		return ;
	}
	i = 5;
	while (input[i])
	{
		while (input[i] == 34 || input[i] == 39)
			i++;
		write(1, &input[i], 1);
		i++;
	}
	ft_printf("\n");
}

int	commands(char *input, char **envp, t_shell *shell)
{
	while (*input && (*input == ' ' || *input == '\t'))
		input++;
	if (*input == '\0')
		return (1);
	shell->args = ft_split(input, ' ');
	if (!ft_quotes(input))
		return (ft_printf("open quote\n"));
	if (ft_strncmp(input, "exit", 4) == 0
		&& (input[4] == ' ' || input[4] == '\0'))
		return (ft_exit(input));
	if (ft_strncmp(input, "echo", 4) == 0
		&& (input[4] == ' ' || input[4] == '\0'))
	{
		ft_echo(input);
		return (1);
	}
	if (ft_strncmp(input, "cd", 2) == 0
		&& (input[2] == ' ' || input[2] == '\0'))
	{
		ft_cd(shell);
		return (1);
	}
	if (ft_strncmp(input, "./", 2) == 0)
	{
		ft_exe(shell, shell);
		return (1);
	}
	ft_shell(input, envp, shell);
	return (1);
}
