/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:06:07 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:06:09 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_heredoc_pattern(t_token *tokens)
{
	if (tokens->type == CMD && tokens->next
		&& tokens->next->type == OP_HEREDOC
		&& tokens->next->next && tokens->next->next->type == DELIM)
		return (1);
	if (tokens->type == OP_HEREDOC && tokens->next
		&& tokens->next->type == DELIM)
		return (1);
	return (0);
}

static int	print_heredoc(t_data *data, t_token *tokens,
		char *line, int heredoc_fd)
{
	char	buffer[4096];
	ssize_t	bytes_read;

	bytes_read = read(heredoc_fd, buffer, sizeof(buffer) - 1);
	while (bytes_read > 0)
	{
		write(STDOUT_FILENO, buffer, bytes_read);
		bytes_read = read(heredoc_fd, buffer, sizeof(buffer) - 1);
	}
	close(heredoc_fd);
	free_token_list(tokens);
	data->tokens = NULL;
	free(line);
	return (0);
}

static int	handle_heredoc_interrupt(t_data *data, t_token *tokens, char *line)
{
	data->exit_code = 130;
	g_last_signal = 0;
	free_token_list(tokens);
	data->tokens = NULL;
	free(line);
	return (0);
}

static int	handle_heredoc_error(t_data *data)
{
	data->exit_code = 1;
	return (-1);
}

int	handle_heredoc_check(t_data *data, t_token *tokens, char *line)
{
	int		heredoc_fd;
	char	*delimiter;

	if (!is_heredoc_pattern(tokens))
		return (-1);
	if (tokens->type == OP_HEREDOC)
		delimiter = tokens->next->value;
	else
		delimiter = tokens->next->next->value;
	heredoc_fd = handle_heredoc(data, delimiter);
	if (heredoc_fd == -2)
		return (handle_heredoc_interrupt(data, tokens, line));
	if (heredoc_fd < 0)
		return (handle_heredoc_error(data));
	if (tokens->type == OP_HEREDOC)
		return (print_heredoc(data, tokens, line, heredoc_fd));
	remove_heredoc_tokens(tokens);
	if (setup_heredoc_redirect(heredoc_fd) == -1)
		return (-1);
	lancer_pipex(data);
	restore_stdin();
	free_token_list(tokens);
	data->tokens = NULL;
	free(line);
	return (0);
}
