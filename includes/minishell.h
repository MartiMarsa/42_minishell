/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarsa-s <mmarsa-s@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:40:49 by mmarsa-s          #+#    #+#             */
/*   Updated: 2024/03/12 16:40:51 by mmarsa-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "Libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <fcntl.h>
# include <signal.h>

typedef struct s_envlst
{
	char				*key;
	char				*val;
	struct s_envlst		*next;
}	t_env;

typedef enum s_tokens
{
	SPACE = 0,
	WORD,
	SIMPLEQ,
	DOUBLEQ,
	INFILE,
	OUTFILE,
	HEREDOC,
	OUTFILEAPP,
	PIPE,
	HEREDOC_NO_EXP,
}	t_tokens;

typedef struct s_lexer
{
	char			*str;
	t_tokens		token;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}	t_lexer;

typedef struct s_exp
{
	char	*str;
	char	*new;
	char	*var;
	char	*val;
	int		alloc;
	int		k;
	int		j;
	int		fl;
	char	q;
}	t_exp;

typedef struct s_fd
{
	int			fd;
	t_tokens	token;
	int			exp;
	char		*str;
	struct s_fd	*next;
}	t_fd;

typedef struct s_pipe
{
	char			**cmd;
	char			*path;
	t_fd			*fd_lst;
	int				in_fd;
	int				out_fd;
	int				builtin;
	struct s_pipe	*next;
}	t_pipe;

typedef struct s_exec
{
	int		fdp[2];
	int		pid;
	int		stat;
}	t_exec;

typedef struct s_toolkit
{
	t_env	*env_lst;
	t_lexer	*lex_lst;
	t_pipe	*pipe_lst;
	t_fd	*hd_lst;
	t_exp	*exp;
	char	*args;
	char	**paths;
	int		exit;
	int		pipes;
	int		check;
	t_exec	*exe;
	char	**env;
	int		power_on;
}	t_toolkit;


//		-> MAIN <-			//

int	main(int argc, char **argv, char **envp);
int	minishell_loop(t_toolkit *tool);

//		-> SHELL_INIT <-		//

t_toolkit	*shell_re(t_toolkit **tool, t_lexer *lex, t_fd *hd);

//		-> LEXER <-			//

int lexer(t_toolkit *tool, char	*input);
t_lexer	*rd_in_quotes(char *input, int *i);
t_lexer	*rd_symbol(t_toolkit *tool, char *input, int *i);
t_lexer	*rd_word(char *input, int *i, char q);
t_lexer	*rd_space(char *input, int *i);

//		-> HEREDOC <-		//

int	heredoc(t_toolkit *tool, char *input, int i);
int	wheredoc(char *str, int i);
char	*keyword_hd(t_fd *new, char *in, int *i, char q);
int	save_hd(t_toolkit *tool, char *key, char *str, int token);
int	hd_close(int fd[], int flag);

//		-> fd_UTILS <-		//

void	fd_add(t_fd **hd_list, t_fd *new);

//		-> SYNTAX <-		//

int	check_quotes(char *str);

//		-> t_lex_list <-	//

t_lexer	*lex_new(char *info, t_tokens type);
t_lexer	*lex_last(t_lexer *lex_list);
void	lex_add(t_lexer **lst, t_lexer *new);
int	lst_clear(t_lexer **lst);
void	lex_insert(t_toolkit *tool, t_lexer *new, t_lexer **lex, t_lexer *temp);

//		-> TOOLS <-			//

int	check_chr(char c);
int	word_in_quotes(char *input, char *q, int j);
char	*trim_quotes(char *str, char q, int len, int i);
int	len_no_q(char *s, char q, int len, int i);

//		-> EXPANSER <-			//

char	*expand_hd(t_toolkit *tool, char *str, int token);
char	*expand_str(t_toolkit *tool, char *str, int token, int i);
t_lexer	*rd_word_exp(char *in, int *i, char q, int j);
int	expand_word(t_toolkit *tool, t_lexer **lex);
int	expanser(t_toolkit *t, t_lexer *head, int flag);

//		-> EXPANSER_UTILS <-		//

int	check_exp(char *str, int token, int q);


//		-> EXPANSER_STRUCTURE <-	//

char	*exp_fd(t_toolkit *tool, char *str, t_fd *new);
int	exp_start(t_toolkit *tool, char *str, int token);
int	exp_init(t_toolkit *tool);
void	exp_clean(t_exp **exp);
void	exp_spc_clean(t_exp *exp);

//		-> ERRORS <-		//

int	error_quotes(t_toolkit *tool);

//		-> PARSER <-		//

t_lexer	*next_word(t_lexer *temp);
int	count_cmd(t_lexer *temp);
int	parse_cmd(t_pipe *new, t_lexer *temp, t_toolkit *t, int j);
int	parse_redir(t_pipe *new, t_lexer *lex, t_fd *hd, t_toolkit *t);
int	parser(t_toolkit *t, t_lexer *lex, t_fd *hd, t_pipe *new);

//		-> PARSER_UTILS <-		//

void	pipe_init(t_pipe *pipe);
void	pipe_add(t_toolkit *tool, t_pipe *new);

#endif