/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_child_setup.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:03:37 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:03:39 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child1(t_pipex *px)
{
	if (px->nb_cmds > 1)
	{
		if (dup2(px->pipefd[0][1], STDOUT_FILENO) == -1)
			free_and_exit(px, NULL, EXIT_FAILURE);
	}
	close_all_pipes(px);
}

void	child2(t_pipex *px)
{
	if (px->nb_cmds > 1)
	{
		if (dup2(px->pipefd[px->nb_cmds - 2][0], STDIN_FILENO) == -1)
			free_and_exit(px, NULL, EXIT_FAILURE);
	}
	close_all_pipes(px);
}

void	child_intermediate(t_pipex *px, int cmd_index)
{
	if (cmd_index > 0 && cmd_index < px->nb_cmds - 1)
	{
		if (dup2(px->pipefd[cmd_index - 1][0], STDIN_FILENO) == -1)
			free_and_exit(px, NULL, EXIT_FAILURE);
		if (dup2(px->pipefd[cmd_index][1], STDOUT_FILENO) == -1)
			free_and_exit(px, NULL, EXIT_FAILURE);
	}
	close_all_pipes(px);
}
