/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:00:19 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 11:14:19 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>
# include <readline/history.h>
# include <readline/readline.h>

# define OUT_QUOTE 0
# define IN_SQUOTE 1
# define IN_DQUOTE 2

typedef struct s_data	t_data;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_OPERATOR,
	OP_PIPE,
	OP_REDIR_IN,
	OP_REDIR_OUT,
	OP_HEREDOC,
	OP_APPEND,
	INFILE,
	OUTFILE,
	DELIM,
	APPEND_FILE,
	CMD,
	ARG,
	WORD
}	t_token_type;

typedef struct s_token
{
	char			quote_type;
	char			*value;
	t_token_type	type;
	struct s_token	*next;
	t_data			*data;
}	t_token;

typedef struct s_data
{
	int		exit_code;
	char	**env;
	t_token	*tokens;
}	t_data;

typedef struct s_pipex
{
	char	*infile;
	char	*outfile;
	int		**pipefd;
	int		nb_cmds;
	t_token	*tokens;
	t_data	*data;
	pid_t	*pids;
	int		pids_allocated;
}	t_pipex;

typedef struct s_expansion
{
	const char	*input;
	t_data		*data;
	char		buffer[1024];
	size_t		i;
	size_t		j;
}	t_expansion;

typedef enum e_signal_mode
{
	PARENT,
	CHILD
}	t_signal_mode;

extern int				g_last_signal;
void					handle_ctrl_c(int sig);
char					*read_user_input(void);
bool					are_quotes_closed_properly(const char *line);
bool					quote_type_at_pos(const char *line, int pos, char c);
bool					get_quote_state(char c, bool in_single_quote,
							bool in_double_quote);
int						count_valid_quotes(const char *line);
char					*strip_quotes(const char *str);
char					detect_quote_type(const char *line, int *index);
t_token					*create_word_token(const char *line, int start, int end,
							char quote_type);
t_token					*extract_word_token(const char *line, int *index);
t_token					*extract_token(const char *line, int *index);
int						is_operator_char(char c);
t_token					*extract_operator_token(const char *line, int *index);
t_token					*tokenize(const char *line);
int						is_separator(char c);
t_token					*handle_redir_and_file(t_token *cur);
t_token					*process_token(t_token *cur, int *expecting_command);
void					annotate_tokens(t_token *tokens);
t_token					*new_token(char *value, t_token_type type);
void					add_token(t_token **head, t_token *new);
void					free_tokens(t_token *tokens);
t_token_type			get_token_type(const char *s);
size_t					ft_strlen(const char *s);
char					*ft_substr(char const *s, unsigned int start,
							size_t len);
char					*ft_strdup(const char *s);
void					*ft_memset(void *b, int c, size_t len);
char					*ft_strchr(const char *s, int c);
int						ft_strcmp(const char *s1, const char *s2);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
int						ft_isalpha(int c);
int						ft_isalnum(int c);
void					*ft_memcpy(void *dst, const void *src, size_t n);
char					*ft_strcpy(char *dst, const char *src);
int						ft_isdigit(int c);
int						ft_atoi(const char *str);
char					*ft_strcat(char *dest, const char *src);
int						ft_numlen(int n);
char					*ft_itoa(int n);
char					*ft_strjoin(char const *s1, char const *s2);
char					**ft_split(char const *s, char c);
void					free_token_array(char **args);
void					free_token_list(t_token *head);
void					free_data(t_data *data);
void					free_all_before_exit(t_data *data);
void					free_tab(char **tab);
void					free_split(char **array, int i);
void					ft_echo(t_data *data, t_token *cmd_token);
int						ft_env(t_data *data);
int						ft_cd(t_data *data, t_token *cmd_token);
int						ft_pwd(t_data *data, t_token *args);
int						ft_export(t_data *data, t_token *cmd_token);
int						ft_unset(t_data *data, t_token *cmd_token);
int						ft_exit(t_data *data, t_token *cmd_token);
int						handle_builtin(t_data *data, t_token *cmd_token);
int						arg_is_valid_n(char *arg);
t_token					*check_echo_n(t_token *args, int *print_newline);
char					*get_env_value(char **env, const char *name,
							size_t len);
char					*expand_variable(const char *input, t_data *data);
void					print_echo_arg(char *arg, int *first, t_data *data,
							char quote_type);
char					**copy_env(char **envp);
int						ft_cd_home(t_data *data);
void					update_pwd_and_oldpwd(t_data *data);
char					*ft_cd_get_path(t_data *data, t_token *cmd_token);
int						ft_cd_exec(t_data *data, char *path);
char					*ft_handle_tilde(t_data *data, char *path);
bool					remove_env_var(t_data *data, const char *name);
int						is_valid_identifier(const char *str);
char					*build_env_var(const char *name, size_t name_len,
							const char *value);
bool					ft_update_env_var(t_data *data, const char *name,
							size_t name_len, const char *value);
void					handle_export_arg(t_data *data, const char *arg);
bool					add_env_var(t_data *data, const char *new_var);
int						is_numeric(const char *str);
int						ft_exit_args_count(t_token *cmd_token, t_token **args);
int						count_env(char **env);
char					**copy_env_for_export(char **env);
void					norm_print_export_var(char *var);
void					print_export_sorted(char **env);
int						ft_cd_count_args(t_token *cmd_token);
int						print_invalid_identifier(const char *value);
void					lancer_pipex(t_data *data);
int						executer_commande(char **argv, t_data *data,
							t_token *cmd_tokens);
int						executer_commande_depuis_tokens_struct(t_data *data,
							t_token *cmd_tokens);
int						count_pipes(t_token *tokens);
void					child1(t_pipex *px);
void					child2(t_pipex *px);
void					child_intermediate(t_pipex *px, int cmd_index);
void					close_all_pipes(t_pipex *px);
void					free_all_pipes(t_pipex *px);
int						allocate_pipes(t_pipex *px, int nb_pipes);
int						create_pipes(t_pipex *px, int nb_pipes);
int						launch_pipeline(t_pipex *px);
int						stdin_redir_exists(t_token *tokens);
int						stdout_redir_exists(t_token *tokens);
pid_t					handle_child_process(t_pipex *px, int cmd_index);
void					parent_close_pipes(t_pipex *px);
void					parent_wait_children(t_pipex *px, pid_t *pids);
void					parent_close_pipes_and_wait(t_pipex *px, pid_t *pids);
void					close_all_pipes(t_pipex *px);
void					free_and_exit(t_pipex *px, t_token *cmd_tokens,
							int code);
char					*find_outfile(t_token *tokens);
char					*find_infile(t_token *tokens);
int						handle_redir_in(t_token *token);
int						handle_redir_out(t_token *token);
int						handle_redir_append(t_token *token);
int						handle_redirections(t_token *tokens);
bool					read_in_stdin(t_data *data, int fd, char *word);
int						handle_heredoc(t_data *data, char *delimiter);
void					setup_child_heredoc_signals(void);
void					setup_parent_heredoc_signals(void);
void					restore_parent_signals(void);
void					free_heredoc_data(t_data *data);
void					handle_line_input(int write_fd, char *line);
void					copy_delimiter(char *dest, const char *src,
							int max_len);
int						child_heredoc_process(int pipefd[2], char *delimiter,
							t_data *data);
char					*construire_chemin_complet(char *repertoire,
							char *commande);
char					*chercher_commande(char **repertoires, char *commande);
char					*chercher_dans_path(char *commande, char **envp);
char					**construire_argv(t_token *tokens);
void					write_error(const char *cmd, const char *msg);
void					write_errno_error(const char *cmd);
char					*get_path_var(char **envp);
char					**split_path_var(char *path_var);
char					*get_command_path(char **argv, t_data *data,
							t_token *cmd_tokens);
void					handle_no_file(char *argv0, char **argv,
							t_token *cmd_tokens, t_data *data);
void					handle_no_permission(char *argv0, char **argv,
							t_token *cmd_tokens, t_data *data);
char					*handle_absolute_path(char *argv0, char **argv,
							t_data *data, t_token *cmd_tokens);
void					handle_expansion_argv0(char **argv, t_data *data);
void					handle_expansion_all_args(char **argv, t_data *data);
void					handle_builtin_and_exit(t_data *data,
							t_token *cmd_tokens, char **argv);
void					handle_path_errors(char *cmd, char **argv,
							char *chemin);
char					*handle_path_search(char *cmd, char **argv, char **env);
void					signals(t_signal_mode mode);
void					parent_handler(int sig);
void					setup_parent_signal(void);
void					heredoc_handler(int sig);
t_token					*copy_token(t_token *src);
t_token					*copy_tokens_until_pipe(t_token *start);
t_token					*get_tokens_for_command(t_token *tokens, int cmd_index);
void					init_pipex(t_pipex *px);
void					handle_and_print_arg(char *raw_arg, int *first,
							t_data *data, char quote_type);
int						handle_redir_basic_ops(t_token *tokens);
int						handle_redirections_basic(t_token *tokens);
int						handle_redirections_heredoc(t_token *tokens);
void					cpy_wrd(char *dest, const char *src, char charset);
char					*allocate_word(const char *str, char charset,
							int *index);
int						nb_wrd(const char *str, char charset);
int						len_wrd(const char *str, char charset);
const char				*get_token_type_name(t_token_type type);
void					print_annotated_tokens(t_token *tokens);
void					cleanup_readline(void);
void					cleanup_child_memory(t_token *tokens,
							t_token *cmd_tokens, t_data *data, pid_t *pids);
int						handle_ctrl_c_signal(t_data *data, char *line);
void					remove_heredoc_tokens(t_token *tokens);
void					restore_stdin(void);
int						handle_heredoc_check(t_data *data,
							t_token *tokens, char *line);
int						handle_variable_assignment(t_data *data,
							t_token *tokens, char *line);
int						handle_builtin_commands(t_data *data, t_token *tokens,
							char *line);
int						handle_signal_and_input(t_data *data, char **line);
int						setup_heredoc_redirect(int heredoc_fd);
int						handle_builtin(t_data *data, t_token *cmd_token);

#endif
