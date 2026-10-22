/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:03:20 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:03:22 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parent_close_pipes(t_pipex *px)
{
	int	i;

	i = 0;
	if (px->pipefd)
	{
		while (i < px->nb_cmds - 1)
		{
			close(px->pipefd[i][0]);
			close(px->pipefd[i][1]);
			i++;
		}
	}
}

static void	handle_signal_exit(int status)
{
	if (WTERMSIG(status) == SIGINT)
	{
		g_last_signal = 130;
		write(1, "\n", 1);
	}
	else if (WTERMSIG(status) == SIGQUIT)
	{
		g_last_signal = 131;
		write(2, "Quit\n", 5);
	}
}

static int	get_exit_status(int status)
{
	int	exit_code;

	exit_code = 0;
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		exit_code = 128 + WTERMSIG(status);
		handle_signal_exit(status);
	}
	return (exit_code);
}

void	parent_wait_children(t_pipex *px, pid_t *pids)
{
	int		i;
	int		status;
	int		last_exit_code;
	pid_t	wpid;

	i = 0;
	last_exit_code = 0;
	while (i < px->nb_cmds)
	{
		if (pids[i] > 0)
		{
			wpid = waitpid(pids[i], &status, 0);
			while (wpid == -1 && errno == EINTR)
				wpid = waitpid(pids[i], &status, 0);
			if (wpid == -1)
				perror("waitpid");
			else
				last_exit_code = get_exit_status(status);
		}
		i++;
	}
	px->data->exit_code = last_exit_code;
}

void	parent_close_pipes_and_wait(t_pipex *px, pid_t *pids)
{
	parent_close_pipes(px);
	parent_wait_children(px, pids);
}
