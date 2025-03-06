/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:53:43 by nnelo             #+#    #+#             */
/*   Updated: 2025/03/06 09:54:28 by ebroudic         ###   ########.fr       */
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
# include <sys/stat.h>
# include <errno.h>

# define RESET   "\033[0m"
# define RED     "\033[31m"
# define GREEN   "\033[32m"
# define YELLOW  "\033[33m"
# define BLUE    "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN    "\033[36m"

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
	char			*cmd;
	char			**cmds;
	int				fd_in;
	int				fd_out;
	char			*in_file;
	char			**out_file;
	int				in;
	int				out;
	char			**input;
	int				pipefd[2];
	int				status;
	char			**exit;
	char			*tmp;
	int				prev_fd;
	char			**pipe;
	char			**copy;
	char			*before;
	char			*var;
	char			*after;
	char			*value;
	char			*res;
	int				save_in;
	int				save_out;
	char			*delimiter;
	char			*line;
	t_env			*env_list;
	t_export		*export_list;
}	t_shell;

extern int	g_status;

/*---------------builtins---------------*/

int			ft_echo(char **args, t_shell *shell, int n, int i);
int			ft_cd(char **args);
int			ft_pwd(void);
int			ft_export(t_shell *shell);
int			ft_unset(t_shell *shell);
int			ft_env(t_shell *shell);
int			ft_exit(char **input, t_shell *shell);

/*---------------env_utils---------------*/

t_env		*init_env_list(char **env);
void		free_env_list(t_env *env_list);
void		append_env_node(t_env **head, const char *env_var);
void		verif_env(t_env **head, const char *env_var, t_env *new_node);
void		print_env(char *arg);
int			ft_sizeenv(t_env *lst);
char		**env_in_stars(t_shell *shell);

/*---------------export_utils---------------*/

t_export	*init_export_list(char **env);
void		free_export_list(t_export *export_list);
void		append_exp_node(t_export **head, const char *export_var, int tpe);
void		ft_sort_export_list(t_export *export_list);
void		verif_exp(t_export **head, const char *var, t_export *new, int tpe);
int			ft_export_verif(char **args, int i, int status, int j);

/*---------------ft_shell---------------*/

int			ft_shell(char **cmd, t_shell *shell, int status);
int			verif_shell(char *input, t_shell *shell, int type);
void		verif_close(t_shell *shell);

/*---------------execute---------------*/

void		ft_execute(char **args, char **envp, t_shell *shell);
int			ft_exe(char **args, t_shell *shell);

/*---------------pipe---------------*/

int			ft_pipe(t_shell *shell);
int			valid_pipe(char *input, int i);

/*---------------quotes---------------*/

int			ft_quotes(char *input);
void		ft_remove_quotes(char *input);
char		*ft_add_quotes(char *input, int i);

/*---------------path---------------*/

char		*find_command_path(char *cmd, char **envp);
char		**ft_split_quote(char *s, char c);
char		**get_command(char **input, t_shell *shell, int i);
char		*get_after_equal(char *arg);
int			is_valid_var_char(char c);
int			quote_isgood(char *input);

/*---------------redirection---------------*/

char		**ft_redirection(char **input, t_shell *shell);
void		parse_commands(char **commands, char *tmp, char *args);
int			parse_out(t_shell *shell, int i, int *out_count, int *append);
int			parse_in(t_shell *shell, int i);
void		read_heredoc(t_shell *shell, char *delimiter);
int			parse_heredoc(t_shell *shell, int i);
int			valid_redirection(char *input);
void		setup_redirection(t_shell *shell);

/*---------------commands---------------*/

int			commands(char *input, t_shell *shell);
int			which_commands(char **input, t_shell *shell);
char		*ft_add_space(char *input, int i);

/*---------------free---------------*/

void		free_args(char **args);
void		free_all(t_shell *shell);

/*---------------signals_prompt---------------*/

void		handle_prompt(t_shell *shell);
int			keypress(char *input, t_shell *shell);
void		handle_sigint(int sig);
void		handle_signal(void (*f)(int), t_shell *shell);

/*---------------utils---------------*/

int			ft_strlen_tab(char **tab);
int			ft_isdigit_neg(int c);
int			ft_isdigit_s(char *s);
char		**merge_args(char **input, char *c, int i, int j);
char		**copy_string(char **input);
int			verif_input(char *input);
void		status_ctrl_c(t_shell *shell);

#endif