/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:04:46 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:04:48 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redir_basic_ops(t_token *tokens)
{
	if (tokens->type == OP_REDIR_IN && tokens->next
		&& (tokens->next->type == ARG || tokens->next->type == INFILE))
	{
		if (handle_redir_in(tokens) == -1)
			return (-1);
		return (1);
	}
	if (tokens->type == OP_REDIR_OUT && tokens->next
		&& (tokens->next->type == ARG || tokens->next->type == OUTFILE))
	{
		if (handle_redir_out(tokens) == -1)
			return (-1);
		return (1);
	}
	if (tokens->type == OP_APPEND && tokens->next
		&& (tokens->next->type == ARG || tokens->next->type == OUTFILE))
	{
		if (handle_redir_append(tokens) == -1)
			return (-1);
		return (1);
	}
	return (0);
}

int	handle_redirections_basic(t_token *tokens)
{
	int	res;

	while (tokens)
	{
		res = handle_redir_basic_ops(tokens);
		if (res == -1)
			return (-1);
		if (res == 1)
			tokens = tokens->next;
		tokens = tokens->next;
	}
	return (0);
}

int	handle_redirections_heredoc(t_token *tokens)
{
	int	heredoc_fd;

	while (tokens)
	{
		if (tokens->type == OP_HEREDOC && tokens->next
			&& (tokens->next->type == ARG || tokens->next->type == DELIM))
		{
			heredoc_fd = handle_heredoc(NULL, tokens->next->value);
			if (heredoc_fd == -1)
				return (-1);
			if (heredoc_fd == -2)
				return (-2);
			if (dup2(heredoc_fd, STDIN_FILENO) == -1)
			{
				perror("dup2 heredoc");
				close(heredoc_fd);
				return (-1);
			}
			close(heredoc_fd);
			tokens = tokens->next;
		}
		tokens = tokens->next;
	}
	return (0);
}

int	handle_redirections(t_token *tokens)
{
	int	ret;

	if (handle_redirections_basic(tokens) == -1)
		return (-1);
	ret = handle_redirections_heredoc(tokens);
	if (ret == -1)
		return (-1);
	if (ret == -2)
		return (-2);
	return (0);
}
