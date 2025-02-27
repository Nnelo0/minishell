/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:01:33 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/26 14:14:18 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	which_commands(char **input, char **envp, t_shell *shell)
{
	int	i;

	i = -1;
	input = ft_redirection(input, shell);
	if (!input)
		return (shell->status);
	while (input[++i])
		ft_remove_quotes(input[i]);
	if (ft_strncmp(input[0], "exit", 4) == 0 && input[0][4] == '\0')
		return (ft_exit(input, shell));
	if (ft_strncmp(input[0], "echo", 4) == 0 && input[0][4] == '\0')
		return (ft_echo(input, shell, 0, 1));
	if (ft_strncmp(input[0], "cd", 2) == 0 && input[0][2] == '\0')
		return (ft_cd(input));
	if (ft_strncmp(input[0], "./", 2) == 0)
		return (ft_exe(input, envp, shell));
	if (ft_strncmp(input[0], "env", 3) == 0 && input[0][3] == '\0')
		return (ft_env(shell));
	if (ft_strncmp(input[0], "pwd", 3) == 0 && input[0][3] == '\0')
		return (ft_pwd());
	if (ft_strncmp(input[0], "export", 6) == 0 && input[0][6] == '\0')
		return (ft_export(shell));
	if (ft_strncmp(input[0], "unset", 5) == 0 && input[0][5] == '\0')
		return (ft_unset(shell));
	return (ft_shell(input, envp, shell, 0));
}

void	command_utils(t_shell *shell)
{
	if (shell->input)
	{
		shell->input = get_command(shell, -1);
		if (!shell->input[0])
			return ;
		shell->status = which_commands(shell->input, shell->env, shell);
		if (shell->save_in != -1)
		{
			if (dup2(shell->save_in, STDIN_FILENO) == -1)
				perror("dup2 save IN");
			close (shell->save_in);
			shell->save_in = -1;
		}
		if (shell->save_out != -1)
		{
			if (dup2(shell->save_out, STDOUT_FILENO) == -1)
				perror("dup2 save OUT");
			close (shell->save_out);
			shell->save_out = -1;
		}
		free_args(shell->input);
	}
}

void	parse_input(char *input, t_shell *shell)
{
	shell->input = ft_split_quote(input, ' ');
	free(input);
	shell->cmd = NULL;
	shell->status = ft_pipe(shell->env, shell);
}

int	commands(char *input, t_shell *shell, int *status)
{
	while (*input && (*input == ' ' || *input == '\t'))
		input++;
	if (*input == '\0')
		return (0);
	status_ctrl_c(status, shell);
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
	parse_input(input, shell);
	command_utils(shell);
	unlink(".tmp_heredoc");
	return (free(shell->tmp), shell->status);
}
