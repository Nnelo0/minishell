/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnelo <nnelo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:01:33 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/15 00:25:26 by nnelo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	which_commands(char **input, char **envp, t_shell *shell)
{
	if (ft_strncmp(input[0], "exit", 4) == 0
		&& (input[0][4] == ' ' || input[0][4] == '\0'))
		return (ft_exit(input, shell));
	if (ft_strncmp(input[0], "echo", 4) == 0
		&& (input[0][4] == ' ' || input[0][4] == '\0'))
		return (ft_echo(input, shell, 0, 1));
	if (ft_strncmp(input[0], "cd", 2) == 0
		&& (input[0][2] == ' ' || input[0][2] == '\0'))
		return (ft_cd(input));
	if (ft_strncmp(input[0], "./", 2) == 0)
		return (ft_exe(input, envp, shell));
	if (ft_strncmp(input[0], "env", 3) == 0
		&& (input[0][3] == ' ' || input[0][3] == '\0'))
		return (ft_env(shell));
	if (ft_strncmp(input[0], "pwd", 3) == 0 && input[0][3] == '\0')
		return (ft_pwd());
	if (ft_strncmp(input[0], "export", 6) == 0
		&& (input[0][6] == ' ' || input[0][6] == '\0'))
		return (ft_export(shell));
	if (ft_strncmp(input[0], "unset", 5) == 0
		&& (input[0][5] == ' ' || input[0][5] == '\0'))
		return (ft_unset(shell));
	return (ft_shell(input, envp, shell, 0));
}

/* int	verif_input(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->input[i])
	{
		if (!is_valid_pipe(shell->input[i]))
			return (ft_printf("invalid pipes\n"), 127);
		if (!is_valid_chevrons(shell->input[i]))
			return (ft_printf("invalid '<' or '>'\n"), 127);
		i++;
	}
	return (0);
} */

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

int	commands(char *input, char **envp, t_shell *shell, int *status)
{
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
	shell->tmp = ft_strdup(input);
	input = ft_add_space(input, -1);
	shell->input = ft_split_quote(input, ' ');
	free(input);
	shell->input[0] = get_command_from_path(shell->input[0]);
	ft_remove_quotes(shell->input[0]);
	shell->cmd = NULL;
	shell->ipt = NULL;
	shell->status = ft_pipe(envp, shell);
	if (shell->input)
		shell->status = which_commands(shell->input, envp, shell);
	free(shell->tmp);
	return (shell->status);
}
