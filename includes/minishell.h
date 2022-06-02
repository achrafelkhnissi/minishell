/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fathjami <fathjami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 14:49:02 by fathjami          #+#    #+#             */
/*   Updated: 2022/05/30 13:53:01 by fathjami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <dirent.h>
# include <fcntl.h>
# include <errno.h>
# include "env.h"
# include <readline/history.h>
# include <readline/readline.h>

# define EQUAL 0

//install readline bash tkhdm.
# define PROMPT "\x1B[36m[minishell]\x1B[35m[:)]\x1B[36m~> \x1B[0m"

typedef struct sigaction	t_sigaction;

typedef enum e_bool
{
	false,
	true
}	t_bool;

enum e_token
{
	WORD = -1,
	WHITE_SPACE = ' ',
	NEW_LINE = '\n',
	QOUTE = '\'',
	DOUBLE_QUOTE = '\"',
	ESCAPE = '\\',
	ENV = '$',
	PIPE_LINE = '|',
	REDIR_IN = '<',
	REDIR_OUT = '>',
	HERE_DOC,
	DREDIR_OUT,
	_NULL = '0',
};

enum e_shell_state
{
	SH_READING,
	SH_EXECUTING,
};

typedef struct s_elem		t_elem;
typedef struct s_redir_elem	t_redir_elem;
typedef union u_union		t_union;
typedef struct s_ast_node	t_ast_node;
typedef struct s_pipe		t_pipe;
typedef struct s_redir_list	t_redir_list;

enum e_state
{
	IN_DQUOTE,
	IN_QUOTE,
	ESCAPED,
	GENERAL,
};

enum e_node_type
{
	CMD,
	PIPE,
};

typedef struct s_fd
{
	int	in;
	int	out;
}	t_fd;
/*
	@desc: struct to store the command's dependencies.
	@name: to store the full binary path or the command's name with or
			without options.
	@args: to store the arguments needed for the command.
	@env: you know.
	@redir: to store all the redirections if founded.
*/
typedef struct s_cmd
{
	char			**args;
	char			**env;
	t_fd			fd;
	t_redir_list	*redir;
}	t_cmd;
/*
	@desc: struct to store the PIPE dependencies.
*/
typedef struct s_pipe
{
	t_ast_node	*left;
	t_ast_node	*right;
}	t_pipe;

/*
	@desc: union to store the defferent possibilities for an AST node.
*/
typedef union u_union
{
	t_pipe	*pipe;
	t_cmd	*cmd;
}	t_union;

/*
	@desc: struct to store an AST node.
*/
typedef struct s_ast_node
{
	enum e_node_type	type;
	t_union				*content;
}	t_ast_node;

typedef struct s_ast
{
	t_ast_node	*root;	
}	t_ast;

/* ----------------------------------------
            LEXER FUNCTIONS/STRUCTS
------------------------------------------*/

/*
	@desc: a linked list element used to store
	a single token returned from the laxer/tokenizer
	(fshek).
*/
typedef struct s_elem
{
	char			*content;
	int				len;
	enum e_token	type;
	enum e_state	state;
	t_elem			*next;
	t_elem			*prev;
}	t_elem;

/*
	@desc: a doubly linked list to store the full command (lexer ouput).
*/
typedef struct s_list
{
	t_elem	*head;
	t_elem	*tail;
	int		size;
}	t_list;

typedef struct s_data
{
	int					exit_status;
	enum e_shell_state	shell_state;
	char				*siglist[NSIG];
	int					pid;
	int					which;
	t_env				*ev;
	t_list				*lex;
	t_ast				*tree;
	int					alloc;
	char				**env;
}	t_data;

t_data						g_data;

//----> lexer.c:
t_list			*lexer(char *line);
void			print_list(t_list *list); // TO REMOVE LATER
int				tokenize(t_list *tokens, char *line, int i,
					enum e_state *state);
int				tokenize_redir(t_list *tokens, char *line, int i,
					enum e_state *state);

// ----> list.c:
int				is_empty(t_list *list);
t_list			*init_list(t_list *list);
t_elem			*new_elem(char *content, int len,
					enum e_token type, enum e_state state);
void			add_tail(t_list *list, t_elem *new);
void			free_list(t_list *list);

// ----> lexer_utils.c
int				ft_isspace(char c);
int				in_charset(char c);
int				get_word(t_list *tokens, char *str, enum e_state state);
int				get_env_var(t_list *tokens, char *str, enum e_state state);
void			quotes_state(t_list *tokens, char *str,
					enum e_state *state, int flag);

/*-----------------------------------------*/

/* ----------------------------------------
              PARSER FUNCTIONS
------------------------------------------*/

/*
	@desc: a linked list to store the redirecton's dependencies.
	@arg: it can be a "filename" or a "delimiter" for HERE_DOC.
	@type: < or > or << or >>.
*/
typedef struct s_redir_elem
{
	char			*arg;
	enum e_token	type;
	t_redir_elem	*next;
}	t_redir_elem;

typedef struct s_redir_list
{
	t_redir_elem	*head;
	t_redir_elem	*tail;
	int				size;
}	t_redir_list;

//----> ast.c:
t_ast			*init_tree(t_ast *tree);
t_ast_node		*create_cmd_node(char **args, char **env, t_redir_list *redir);
void			print_ast_node(t_ast_node *cmd);
void			print_tree(t_ast_node *ptr, char **env); // TO REMOVE LATER
void			free_parser(t_ast *tree);

//----> parse_utils.c:
int				count_args(t_elem *elem);
int				is_redir(enum e_token type);
void			count_quotes_redir_args(t_elem **elem,
					enum e_token type, int *i);

//----> redir_utils.c:
t_redir_list	*init_redir_list(t_redir_list *list);
t_redir_elem	*new_redir_elem(char *arg, enum e_token type);
void			add_redir_tail(t_redir_list *list, t_redir_elem *new);
void			free_redir_list(t_redir_list *list);
void			print_redir_list(t_redir_list *redir);

//----> prase.c:
void			parse_word(t_elem **ptr, char **args, int *i);
t_ast_node		*parse_pipe(t_elem **ptr);
char			*parse_quotes(char *str, t_elem **ptr,
					enum e_token type, t_env *env);
t_redir_list	*parse_redir(t_elem **ptr, t_redir_list *redir, t_env *env);
t_redir_list	*parse_here_doc(t_redir_list *redir, char *arg, char **env);
// ----> parse_core.c:
int				parse(t_ast **tree, t_elem *ptr, t_env *env);
int				parse_cmd_act(t_elem **ptr, t_env *env,
					t_redir_list *redir, char **args);
t_ast_node		*parse_cmd(t_elem **ptr, t_env *env);

/*----------------------------------------*/

/* ----------------------------------------
              UTILS FUNCTIONS
------------------------------------------*/

//----> string.c:
int				ft_strlen(char *str);
char			*ft_strdup(char *str);
char			*ft_substr(char *s, int start, int len);
int				ft_strchr(char *s, int c);
char			*ft_strjoin(char *s1, char *s2);
char			*ft_strndup(char *s1, int n);
int				ft_strcmp(char *s1, char *s2);
char			*get_next_line(int fd);

// ----> string_utils.c:
int				is_alphanum(int c);
int				ft_strlen(char *str);
char			*ft_join_dir(char *s1, char *s2);
int				ft_strncmp(char *s1, char *s2, int n);

void			*ft_calloc(size_t count, size_t size);
char			**ft_split(char *str, char *sap);
int				ft_atoi(const char *nptr);
char			*ft_itoa(int n);
int				ft_strisspace(char *str);
/* ---------------------------------------*/

/* ----------------------------------------
              SYNTAX FUNCTIONS
------------------------------------------*/

// ----> syntax.c:

t_elem			*skip_spaces(t_elem *ptr, int opt);
char			*get_redir(enum e_token type);
int				syntax_errors(t_list *list);

/* ---------------------------------------*/

/* ----------------------------------------
              EXECUTION FUNCTIONS
------------------------------------------*/

// ----> execute.c:

char			*find_command(char *cmd, char **env);
int				execute_cmd(t_cmd *cmd, char **env, int fd[2]);
char			*find_slash_dot(char *cmd, char **env);
void			execute(t_ast_node *ptr, char ***env,
					int cmd_count, t_elem *head);
// ----> redir.c;

void			exec_redir(t_redir_list *list, t_fd *fd);

/* ---------------------------------------*/

/* ----------------------------------------
              CLEAN UP FUNCTIONS
------------------------------------------*/

//----> clean_up.c:
void			free_tree(t_ast_node *node);
void			free_lexer(t_list *lexer);
void			free_cmd(t_cmd *cmd);
void			free_2d(char **arg);
void			free_lexer_parser(t_list *lexer, t_ast *tree);

/* ---------------------------------------*/

/* ft_export funcitons */
int				ft_export(char ***ev, t_cmd *cmd, t_elem *head);
void			print_sorted_env(t_env *env);
void			ft_sort(t_env *env);

int				ft_isalpha(char c);
int				ft_unset(t_cmd *cmd, char ***ev);
int				ft_pwd(t_cmd *cmd);
int				ft_exit(t_cmd *cmd, char **env);
int				ft_cd(t_cmd *cmd, char ***env);
int				ft_env(t_cmd *cmd, char **env);
int				ft_echo(t_cmd *cmd, t_elem *head, char **env);

t_elem			*find_env_key(t_elem *head, char *arg, char **env);

void			init_sigaction(t_data *data);
void			signals_handler(int sig, siginfo_t *siginfo, void *ptr);
void			init_sig_list(t_data *data);

void			lunch_program(char **env);
void			ft_wait(void);

/* ----------------------------------------
              ERROR FUNCTIONS
------------------------------------------*/
int				pipe_error(t_elem *ptr);
int				redir_error(t_elem *ptr);
int				ft_perror(char *msg, char *utils);
int				ft_puterr(char *cmd, char *arg, char *msg, int err_num);

void			execute_builtins(t_cmd *cmd, char ***env, t_elem *head);
void			dup_fd(int *old, int new);
int				check_builtins(char *cmd);
void			ft_which_cmd(t_cmd *cmd, char ***env, t_elem *head);

void			ft_plus(int *i, int plus);
void			check_cmd_state(t_elem**ptr, char **env, int *value);
int				*find_right_left(t_ast_node *ptr);

int				check_export_cmd(t_elem *ptr, char **env);
char			**split_env(char *arg);

#endif
