/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:53:43 by nnelo             #+#    #+#             */
/*   Updated: 2025/01/09 14:28:15 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft/libft.h"
# include "libft/printf/ft_printf.h"
# include <sys/wait.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <signal.h>
# include <sys/wait.h>

typedef struct s_shell
{
	sig_atomic_t	signal_status;
	char			**args;
	char			**envp1;
	char			**cmds;
}	t_shell;

void	handle_prompt(t_shell *shell, char **envp);
int		keypress(char *input, t_shell *shell);
void	handle_sigint(int sig);
int		ft_exit(char *input, t_shell *shell);
int		commands(char *input, char **envp, t_shell *shell);
void	ft_shell(char *input, char **envp, t_shell *shell);
int		ft_quotes(char *input);
void	ft_remove_quotes(char *input);
int		ft_echo(char *input);
void	free_args(char **args);
int		ft_env(char **envp);
int		ft_pwd(void);
int		ft_pipe(char *input, char **envp, t_shell *shell);
int		which_commands(char *input, char **envp, t_shell *shell);
char	*find_command_path(char *cmd, char **envp);

#endif