/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:04:01 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:04:03 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_heredoc_status(int status, int pipefd)
{
	if (WIFEXITED(status))
	{
		g_last_signal = WEXITSTATUS(status);
		if (g_last_signal == 130)
		{
			close(pipefd);
			return (-2);
		}
	}
	else if (WIFSIGNALED(status))
	{
		g_last_signal = 128 + WTERMSIG(status);
		close(pipefd);
		return (-2);
	}
	return (pipefd);
}

int	handle_heredoc(t_data *data, char *delimiter)
{
	pid_t	pid;
	int		pipefd[2];
	int		status;

	if (pipe(pipefd) == -1)
		return (-1);
	setup_parent_heredoc_signals();
	pid = fork();
	if (pid < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
		child_heredoc_process(pipefd, delimiter, data);
	close(pipefd[1]);
	waitpid(pid, &status, 0);
	restore_parent_signals();
	return (handle_heredoc_status(status, pipefd[0]));
}
