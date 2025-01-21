/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:53:43 by nnelo             #+#    #+#             */
/*   Updated: 2025/01/21 17:13:51 by cle-berr         ###   ########.fr       */
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
# include "pipex.h"
# include <signal.h>

typedef struct s_env
{
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_export
{
	char			*value;
	struct s_export	*next;
}	t_export;

typedef struct s_shell
{
	sig_atomic_t	signal_status;
	char			**args;
	t_env			*env_list;
	t_export		*export_list;
}	t_shell;

void		handle_prompt(t_shell *shell, char **envp);
int			keypress(char *input, t_shell *shell);
void		handle_sigint(int sig);
int			ft_exit(char *input, t_shell *shell);
int			commands(char *input, char **envp, t_shell *shell);
void		ft_shell(char *input, char **envp, t_shell *shell);
int			ft_quotes(char *input);
void		ft_remove_quotes(char *input);
int			ft_echo(char *input);
void		free_args(char **args);
int			ft_env(t_shell *shell);
int			ft_pwd(void);
int			ft_export(t_shell *shell);
t_env		*init_env_list(char **env);
void		free_env_list(t_env *env_list);
t_export	*init_export_list(char **env);
void		free_export_list(t_export *export_list);

#endif