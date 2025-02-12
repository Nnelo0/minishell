/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:53:43 by nnelo             #+#    #+#             */
/*   Updated: 2025/02/12 13:12:50 by cle-berr         ###   ########.fr       */
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
	char			**envp1;
	char			*cmd;
	char			**cmds;
	char			**ipt;
	int				fd_in;
	int				fd_out;
	char			*in_file;
	char			**out_file;
	int				in;
	int				out;
	char			*input;
	int				pipefd[2];
	int				status;
	char			**exit;
	char			*verif;
	t_env			*env_list;
	t_export		*export_list;
}	t_shell;

void		handle_prompt(t_shell *shell, char **envp);
int			keypress(char *input, t_shell *shell);
void		handle_sigint(int sig);
int			ft_exit(char *input, t_shell *shell);
int			commands(char *input, char **envp, t_shell *shell);
int			ft_shell(char *input, char **envp, t_shell *shell);
int			ft_quotes(char *input);
void		ft_remove_quotes(char *input);
int			ft_echo(char *input, t_shell *shell, int n, int i);
void		free_args(char **args);
int			ft_env(t_shell *shell);
int			ft_pwd(void);
int			ft_pipe(char *input, char **envp, t_shell *shell);
int			which_commands(char *input, char **envp, t_shell *shell);
char		*find_command_path(char *cmd, char **envp);
int			ft_redirection(char *input, t_shell *shell);
int			is_valid_chevrons(char *input);
char		**ft_split_chevrons(char *input, int i, int j);
int			ft_strlen_tab(char **tab);
void		parse_commands(char **commands, char *tmp, char *args);
int			is_valid_pipe(char *input);
int			parse_out(t_shell *shell, int i, int *out_count, int *append);
int			parse_in(t_shell *shell, int i);
int			ft_export(t_shell *shell);
t_env		*init_env_list(char **env);
void		free_env_list(t_env *env_list);
t_export	*init_export_list(char **env);
void		free_export_list(t_export *export_list);
void		append_env_node(t_env **head, const char *env_var);
void		append_exp_node(t_export **head, const char *export_var, int tpe);
void		ft_sort_export_list(t_export *export_list);
void		verif_env(t_env **head, const char *env_var, t_env *new_node);
void		verif_exp(t_export **head, const char *var, t_export *new, int tpe);
char		**ft_split_quote(char const *s, char c);
int			ft_unset(t_shell *shell);
void		read_heredoc(t_shell *shell, char *delimiter);
int			parse_heredoc(t_shell *shell, int i);
int			ft_isdigit_neg(int c);
int			ft_isdigit_s(char *s);
void		free_all(t_shell *shell, char *input);
char		*find_command_path(char *cmd, char **envp);
char		*get_command_from_path(char *input);

#endif