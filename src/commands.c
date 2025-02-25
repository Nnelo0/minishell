/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:01:33 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/25 08:55:53 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	which_commands(char **input, char **envp, t_shell *shell)
{
	input = ft_redirection(input, shell);
	if (!input)
		return (shell->status);
	if (ft_strncmp(input[0], "exit", 4) == 0
		&& input[0][4] == '\0')
		return (ft_exit(input, shell));
	if (ft_strncmp(input[0], "echo", 4) == 0
		&& input[0][4] == '\0')
		return (ft_echo(input, shell, 0, 1));
	if (ft_strncmp(input[0], "cd", 2) == 0
		&& input[0][2] == '\0')
		return (ft_cd(input));
	if (ft_strncmp(input[0], "./", 2) == 0)
		return (ft_exe(input, envp, shell));
	if (ft_strncmp(input[0], "env", 3) == 0
		&& input[0][3] == '\0')
		return (ft_env(shell));
	if (ft_strncmp(input[0], "pwd", 3) == 0
		&& input[0][3] == '\0')
		return (ft_pwd());
	if (ft_strncmp(input[0], "export", 6) == 0
		&& input[0][6] == '\0')
		return (ft_export(shell));
	if (ft_strncmp(input[0], "unset", 5) == 0
		&& input[0][5] == '\0')
		return (ft_unset(shell));
	return (ft_shell(input, envp, shell, 0));
}

int	is_separator(char c)
{
	if (c == '|' || c == '>' || c == '<')
		return (1);
	return (0);
}

char	*ft_add_space(char *input, int i)
{
	int		len;
	char	*result;
	int		in_quotes;

	if (!input)
		return (NULL);
	result = malloc(ft_strlen(input) * 2 + 1);
	if (!result)
		return (free(input), NULL);
	len = 0;
	in_quotes = 0;
	while (input[++i])
	{
		if (input[i] == '"' || input[i] == '\'')
			in_quotes = !in_quotes;
		if (!in_quotes && i > 0 && is_separator(input[i])
			&& input[i - 1] != ' ' && !is_separator(input[i - 1]))
			result[len++] = ' ';
		result[len++] = input[i];
		if (!in_quotes && is_separator(input[i])
			&& input[i + 1] != ' ' && !is_separator(input[i + 1]))
			result[len++] = ' ';
	}
	return (result[len] = '\0', result);
}

int	verif_input(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\"' || input[i] == '\'')
		{
			i++;
			while (input[i] && (input[i] != '\"' || input[i] != '\''))
				i++;
		}
		if (input[i] == '|' && input[i + 1] == '|')
			return (2);
		if (input[i] == '>' && input[i + 1] == '>' && input[i + 2] == '>')
			return (1);
		if (input[i] == '<' && input[i + 1] == '<' && input[i + 2] == '<')
			return (1);
		i++;
	}
	return (0);
}

int	commands(char *input, char **envp, t_shell *shell, int *status)
{
	if (shell->env)
		free_args(shell->env);
	shell->env = env_in_stars(shell);
	while (*input && (*input == ' ' || *input == '\t'))
		input++;
	if (*input == '\0')
		return (0);
	if (*status == 130)
	{
		shell->status = 130;
		*status = 0;
	}
	if (!ft_quotes(input))
		return (ft_printf("open quote\n"), 127);
	if (verif_shell(input, shell, 0) == 1)
		return (free_args(shell->env), shell->status);
	shell->tmp = ft_strdup(input);
	input = ft_add_space(input, -1);
	if (verif_input(input) == 2)
		return (ft_putstr_fd("Invalid Pipes\n", 2), free(input)
			, free(shell->tmp), free_args(shell->env), shell->status);
	if (verif_input(input) == 1)
		return (ft_putstr_fd("Invalid Redirections\n", 2), free(input)
			, free(shell->tmp), free_args(shell->env), shell->status);
	shell->input = ft_split_quote(input, ' ');
	free(input);
	shell->cmd = NULL;
	shell->status = ft_pipe(envp, shell);
	if (shell->input)
	{
		get_command(shell);
		shell->status = which_commands(shell->input, envp, shell);
		verif_close(shell);
		free_args(shell->input);
	}
	return (free(shell->tmp), shell->status);
}
