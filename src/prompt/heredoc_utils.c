/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:06:13 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:06:15 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_heredoc_tokens(t_token *tokens)
{
	t_token	*heredoc_token;
	t_token	*delim_token;

	if (tokens->type == OP_HEREDOC)
	{
		delim_token = tokens->next;
		if (delim_token)
		{
			free(tokens->value);
			free(tokens);
			free(delim_token->value);
			free(delim_token);
		}
		return ;
	}
	if (tokens->next && tokens->next->type == OP_HEREDOC)
	{
		heredoc_token = tokens->next;
		delim_token = heredoc_token->next;
		tokens->next = delim_token->next;
		free(heredoc_token->value);
		free(heredoc_token);
		free(delim_token->value);
		free(delim_token);
	}
}

int	setup_heredoc_redirect(int heredoc_fd)
{
	int	result;

	result = dup2(heredoc_fd, STDIN_FILENO);
	if (result == -1)
	{
		perror("dup2 heredoc");
		close(heredoc_fd);
		return (-1);
	}
	close(heredoc_fd);
	return (0);
}

void	restore_stdin(void)
{
	int	tty_fd;
	int	result;

	tty_fd = open("/dev/tty", 2);
	if (tty_fd != -1)
	{
		result = dup2(tty_fd, STDIN_FILENO);
		(void)result;
		close(tty_fd);
	}
}

char	*read_user_input(void)
{
	char	*line;

	line = readline("\033[3mMinishell \033[0m > ");
	return (line);
}
