/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:04:07 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:04:09 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_signal_exit(int write_fd, t_data *data)
{
	if (g_last_signal == 130)
	{
		close(write_fd);
		free_heredoc_data(data);
		exit(130);
	}
}

static void	handle_line_or_delim(int write_fd, char *line, char *delimiter,
	t_data *data)
{
	if (!line || ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		close(write_fd);
		free_heredoc_data(data);
		exit(0);
	}
	handle_line_input(write_fd, line);
}

static void	child_heredoc_loop_core(int write_fd, char *delimiter, t_data *data)
{
	char	*line;

	while (1)
	{
		check_signal_exit(write_fd, data);
		line = readline("> ");
		check_signal_exit(write_fd, data);
		handle_line_or_delim(write_fd, line, delimiter, data);
	}
}

void	child_heredoc_loop(int write_fd, char *delimiter, t_data *data)
{
	setup_child_heredoc_signals();
	child_heredoc_loop_core(write_fd, delimiter, data);
}

int	child_heredoc_process(int pipefd[2], char *delimiter, t_data *data)
{
	char	delim_copy[4096];

	close(pipefd[0]);
	copy_delimiter(delim_copy, delimiter, 4096);
	child_heredoc_loop(pipefd[1], delim_copy, data);
	return (0);
}
