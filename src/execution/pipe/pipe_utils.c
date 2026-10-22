/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:03:26 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:03:28 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_pipes(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == OP_PIPE)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

void	close_all_pipes(t_pipex *px)
{
	int	i;

	i = 0;
	if (!px->pipefd)
		return ;
	while (i < px->nb_cmds - 1)
	{
		close(px->pipefd[i][0]);
		close(px->pipefd[i][1]);
		i++;
	}
}

void	free_all_pipes(t_pipex *px)
{
	int	i;

	if (!px->pipefd)
		return ;
	i = 0;
	while (i < px->nb_cmds - 1)
	{
		if (px->pipefd[i])
		{
			close(px->pipefd[i][0]);
			close(px->pipefd[i][1]);
			free(px->pipefd[i]);
		}
		i++;
	}
	free(px->pipefd);
	px->pipefd = NULL;
}
