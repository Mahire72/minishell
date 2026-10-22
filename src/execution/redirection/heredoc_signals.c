/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:04:12 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:04:14 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_parent_heredoc_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_child_heredoc_signals(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, heredoc_handler);
}

void	restore_parent_signals(void)
{
	signal(SIGINT, parent_handler);
	signal(SIGQUIT, SIG_DFL);
}
