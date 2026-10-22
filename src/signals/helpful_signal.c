/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpful_signal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:06:25 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:06:28 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_last_signal = 0;

void	parent_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_last_signal = 130;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	heredoc_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		g_last_signal = 130;
		close(0);
		close(3);
		close(4);
	}
}

void	setup_parent_signal(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGINT);
	sa.sa_flags = 0;
	sa.sa_handler = parent_handler;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}
