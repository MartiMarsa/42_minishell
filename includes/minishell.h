/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarsa-s <mmarsa-s@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:40:49 by mmarsa-s          #+#    #+#             */
/*   Updated: 2024/06/07 13:19:13 by andmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define NORM 1
# define N_INTERACT 3
# define CHILD 2

extern int			g_sig_rec;

typedef struct s_envlst
{
	char			*key;
	char			*val;
	struct s_envlst	*next;
}					t_env;

typedef enum s_tokens
{
	MYSPACE = 0,
	WORD,
	SIMPLEQ,
	DOUBLEQ,
	INFILE,
	OUTFILE,
	HEREDOC,
	OUTFILEAPP,
	PIPE,
	HEREDOC_NO_EXP,
}					t_tokens;

typedef struct s_lexer
{
	char			*str;
	t_tokens		token;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}					t_lexer;

typedef struct s_exp
{
	char			*str;
	char			*new;
	char			*var;
	char			*val;
	int				alloc;
	int				k;
	int				j;
	int				fl;
	char			q;
}					t_exp;

typedef struct s_fd
{
	int				fd;
	t_tokens		token;
	int				exp;
	char			*str;
	struct s_fd		*next;
}					t_fd;

typedef struct s_pipe
{
	char			**cmd;
	char			*path;
	t_fd			*fd_lst;
	int				in_fd;
	int				out_fd;
	int				builtin;
	struct s_pipe	*next;
}					t_pipe;

typedef struct s_exec
{
	int				fdp[2];
	int				pid;
	int				stat;
}					t_exec;

typedef struct s_toolkit
{
	t_env			*env_lst;
	t_lexer			*lex_lst;
	t_pipe			*pipe_lst;
	t_fd			*hd_lst;
	t_exp			*exp;
	char			*args;
	char			**paths;
	int				exit;
	int				pipes;
	int				check;
	t_exec			*exe;
	char			**env;
	int				power_on;
}					t_toolkit;

//		-> MAIN <-			//
int					main(int argc, char **argv, char **envp);
int					minishell_loop(t_toolkit *tool);

//		-> SHELL_INIT <-		//
t_toolkit			*shell_re(t_toolkit **tool, t_lexer *lex, t_fd *hd);
int					sh_init(t_toolkit *sh, char **env);
void				sh_clean(t_toolkit *sh);
int					sh_loop_init(t_toolkit *sh);
int					allocate_exe(t_toolkit *sh);

//		-> SIGNALS <-		//
void				norm_handler(int sig, siginfo_t *data, void *non_used_data);
void				do_sigign(int signum);
int					init_signals(int mode);
void				exit_status(t_toolkit *sh, int j);
void				child_sigint_handler(int sig);

//		-> LEXER <-			//
int					lexer(t_toolkit *tool, char *input);
t_lexer				*rd_in_quotes(char *input, int *i);
t_lexer				*rd_symbol(t_toolkit *tool, char *input, int *i);
t_lexer				*rd_word(char *input, int *i, char q);
t_lexer				*rd_space(char *input, int *i);

//		-> HEREDOC <-		//
int					heredoc(t_toolkit *tool, char *input, int i);
int					wheredoc(char *str, int i);
char				*keyword_hd(t_fd *new, char *in, int *i, char q);
int					save_hd(t_toolkit *tool, char *key, char *str, int token);
int					hd_close(int fd[], int flag);

//		-> fd_UTILS <-		//
void				fd_add(t_fd **hd_list, t_fd *new);
int					fd_init(t_fd *new, t_toolkit *tool, int fd);
void				fd_clean(t_fd **hd, int flag);
int					ft_open_built(t_toolkit *sh, t_pipe *p, t_fd *fd1,
						int prev);

//		-> SYNTAX <-		//
int					check_quotes(char *str);
int					check_input(char *in);
int					check_syntax(t_toolkit *sh, t_lexer *current,
						int prev_token);
int					exp_quotes(t_toolkit *sh, t_lexer **head, int *flag);

//		-> t_lex_list <-	//
t_lexer				*lex_new(char *info, t_tokens type);
t_lexer				*lex_last(t_lexer *lex_list);
void				lex_add(t_lexer **lst, t_lexer *new);
int					lst_clear(t_lexer **lst);
void				lex_insert(t_toolkit *tool, t_lexer *new, t_lexer **lex,
						t_lexer *temp);

//		-> TOOLS <-			//
int					check_chr(char c);
int					word_in_quotes(char *input, char *q, int j);
char				*trim_quotes(char *str, char q, int len, int i);
int					len_no_q(char *s, char q, int len, int i);

//		-> lexer_UTILS <-		//
int					ft_longer(char *str, char *key);
char				*ft_triplejoin(char *s1, char *s2, char *s3);
int					lex_clean(t_lexer **lst);
char				**arr_clean(char **cmd, int flag);
int					open_q(t_exp *exp, char c, int type);
char				*ft_smart_join(char *s1, char *s2, char *s3);

//		-> EXPANSER <-			//
char				*expand_hd(t_toolkit *tool, char *str, int token);
char				*expand_str(t_toolkit *tool, char *str, int token, int i);
t_lexer				*rd_word_exp(char *in, int *i, char q, int j);
int					expand_word(t_toolkit *tool, t_lexer **lex);
int					expanser(t_toolkit *t, t_lexer *head, int flag);

//		-> EXPANSER_UTILS <-		//
int					check_exp(char *str, int token, int q);
int					new_len(t_toolkit *tool, char *cont, int type, int len);
char				*get_var(char *cont);
char				*check_value(t_toolkit *tool, char *var);
int					check_file_exp(char *str);

//		-> EXPANSER_STRUCTURE <-	//
char				*exp_fd(t_toolkit *tool, char *str, t_fd *new);
int					exp_start(t_toolkit *tool, char *str, int token);
int					exp_init(t_toolkit *tool);
void				exp_clean(t_exp **exp);
void				exp_spc_clean(t_exp *exp);

//		-> ERRORS <-		//
int					error_quotes(t_toolkit *tool);
int					err_exit(t_toolkit *sh, char *name, char *message, int err);
int					err_break(t_toolkit *sh, char *name, char *message,
						int err);
int					err_char(t_toolkit *sh, int token);
void				print_error(char **args);

//		-> PARSER <-		//
t_lexer				*next_word(t_lexer *temp);
int					count_cmd(t_lexer *temp);
int					parse_cmd(t_pipe *new, t_lexer *temp, t_toolkit *t, int j);
int					parse_redir(t_pipe *new, t_lexer *lex, t_fd *hd,
						t_toolkit *t);
int					parser(t_toolkit *t, t_lexer *lex, t_fd *hd, t_pipe *new);

//		-> PARSER_UTILS <-		//
void				pipe_init(t_pipe *pipe);
void				pipe_add(t_toolkit *tool, t_pipe *new);
int					pipe_clean(t_pipe **lst);

//		-> EXECUTOR <-			//
void				ft_redir(t_toolkit *sh, t_pipe *p);
void				child_process(t_toolkit *sh, t_pipe *p, int option);
int					last_child(t_toolkit *sh, t_pipe *p);
int					executor(t_toolkit *sh, t_pipe *p, int i, int j);
int					exec_builtin(t_toolkit *sh, t_pipe *p);

//		-> EXECUTOR_utils <-			//
void				ft_check_open(t_pipe *p, t_fd *cur, int prev);
int					check_builtin(char **cmd);
void				ft_open(t_toolkit *sh, t_pipe *p, t_fd *fd1, int prev);
void				check_access(t_toolkit *sh, char **cmd, t_pipe *p);
void				check_paths(char **paths, char *cmd, t_toolkit *sh,
						t_pipe *pipe);

//		-> BUILTINS <-		//

//		-> 1.CD <-		//

char				*get_the_path(t_toolkit *sh, int opt);
char				*get_cd_path(int option, t_toolkit *sh);
int					go_to_path(int option, t_toolkit *sh);
int					change_directory(t_toolkit *sh, const char *path);
int					ft_cd(t_toolkit *sh, t_pipe *p);

//		-> 2.ECHO <-		//
int					parse_nl(char *s);
int					check_nl(char *str);
int					num_args(char **args);
int					ft_echo(t_toolkit *sh, t_pipe *p);

//		-> 3.ENV<-		//
int					ft_env(t_toolkit *sh, t_pipe *p);

//		-> 4.EXIT<-		//
long long int		ft_atol_sh(char *str);
int					arg_count(char **grid);
int					p_exit_err(char *str_error, int option);
int					check_exit(char *s);
int					ft_exit(t_toolkit *sh);

//		-> 5.EXPORT_UTILS<-		//
int					print_export(t_env *eprint, t_pipe *p);
int					export_option(const char *name);
char				*find_in_env_variables(t_toolkit *sh, char *variable_name);
void				print_env_fd(t_env *tmp, int output);

//		-> 6.EXPORT<-		//
void				export_plus_equal(t_toolkit *sh, char *key, char *value);
int					error_option(char *str1, char *str2, char **vc);
int					handle_args(t_toolkit *sh, char *arg);
int					ft_export(t_toolkit *sh, t_pipe *p);
char				**split_by_equal(char *str);

//		-> 7.PWD<-		//
int					ft_pwd(t_toolkit *sh, t_pipe *p);
int					update_oldpwd(t_toolkit *sh);
int					goodbye(t_toolkit *sh);

//		-> 8.UNSET<-		//
void				unset_free(t_env *env);
void				unset_var(t_toolkit *sh, char *var);
int					ft_unset(t_toolkit *sh, t_pipe *p);

//		-> ENV <-		//

//		-> 1.ENV_FREE <-		//
int					free_env_lst(t_env *head);
void				free_env(t_toolkit *sh);

//		-> 2.ENV_LIST <-		//
t_env				*ft_getkey_node(char *new_key, t_env *list);
int					env_val_update(t_env *head, char *key, char *n_value);
int					env_add_last(t_toolkit *sh, char *name, char *value,
						int has_value);
int					add_or_update_env(t_toolkit *sh, char *name, char *value);
int					update_existing_env(t_env *env, char *value, t_toolkit *sh);

//		-> 3.ENV_SORT <-		//
void				print_sort_print(t_env *env);
void				sort_env(t_env *head);

//		-> 4.ENV_UTILS <-		//
char				*ft_get_value(t_toolkit *sh, char *key);

//		-> 5.ENV <-		//
char				*ft_envfull(char *key, char *value);
size_t				env_var(t_env *head, int option);
void				add_env_to_list(t_toolkit *sh, t_env *new_env);
int					first_env(t_toolkit *sh, char **env);
char				**env_converter(t_env *env);

#endif
