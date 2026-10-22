/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_append.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:04:31 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:04:33 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	redir_error(t_token *token, const char *msg)
{
	write(2, "minishell: ", 11);
	write(2, token->next->value, ft_strlen(token->next->value));
	write(2, ": ", 2);
	write(2, strerror(errno), ft_strlen(strerror(errno)));
	write(2, "\n", 1);
	if (token->next->data)
		token->next->data->exit_code = 1;
	(void)msg;
	return (-1);
}

int	handle_redir_in(t_token *token)
{
	int	fd;

	fd = open(token->next->value, O_RDONLY);
	if (fd < 0)
		return (redir_error(token, "infile"));
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2 infile");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_redir_out(t_token *token)
{
	int	fd;

	fd = open(token->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (redir_error(token, "outfile"));
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 outfile");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_redir_append(t_token *token)
{
	int	fd;

	fd = open(token->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (redir_error(token, "append"));
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 append");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}
