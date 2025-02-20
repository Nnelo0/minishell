/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnelo <nnelo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:01:33 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/20 18:37:25 by nnelo            ###   ########.fr       */
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
		return (ft_pwd(shell));
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

char	*ft_add_space(char *input, int i, int len, int in_quotes)
{
	char	*result;

	result = malloc(sizeof(char) * (strlen(input) * 2 + 1));
	if (!result)
		return (NULL);
	while (input[++i])
	{
		if (input[i] == '"' || input[i] == '\'')
			in_quotes = !in_quotes;
		if (!in_quotes)
		{
			if (i > 0 && is_separator(input[i]) && input[i - 1] != ' '
				&& (!is_separator(input[i - 1]) || input[i - 1] != input[i]))
				result[len++] = ' ';
			result[len++] = input[i];
			if (is_separator(input[i]) && input[i + 1] != ' '
				&& input[i + 1] && (!is_separator(input[i + 1])
					|| input[i + 1] != input[i]))
				result[len++] = ' ';
		}
		else
			result[len++] = input[i];
	}
	return (result[len] = '\0', result);
}

int	commands(char *input, char **envp, t_shell *shell, int *status)
{
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
		return (ft_putstr_fd("open quote\n", 2), 127);
	if (verif_shell(input, shell, 0) == 1)
		return(free_args(shell->env), shell->status);
	shell->tmp = ft_strdup(input);
	input = ft_add_space(input, -1, 0, 0);
	shell->input = ft_split_quote(input, ' ');
	free(input);
	shell->cmd = NULL;
	shell->status = ft_pipe(envp, shell);
	if (shell->input)
	{
		ft_remove_quotes(shell->input[0]);
		shell->input[0] = get_command_from_path(shell->input[0]);
		shell->status = which_commands(shell->input, envp, shell);
		verif_close(shell);
	}
	return (free_args(shell->env), free(shell->tmp), shell->status);
}
