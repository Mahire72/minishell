/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:03:52 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:03:54 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	allocate_pipes(t_pipex *px, int nb_pipes)
{
	int	i;

	px->pipefd = malloc(sizeof(int *) * nb_pipes);
	if (!px->pipefd)
		return (-1);
	i = 0;
	while (i < nb_pipes)
	{
		px->pipefd[i] = malloc(sizeof(int) * 2);
		if (!px->pipefd[i])
		{
			while (--i >= 0)
				free(px->pipefd[i]);
			free(px->pipefd);
			px->pipefd = NULL;
			return (-1);
		}
		i++;
	}
	return (0);
}

int	create_pipes(t_pipex *px, int nb_pipes)
{
	int	i;

	i = 0;
	while (i < nb_pipes)
	{
		if (pipe(px->pipefd[i]) == -1)
		{
			perror("pipe");
			while (--i >= 0)
			{
				close(px->pipefd[i][0]);
				close(px->pipefd[i][1]);
				free(px->pipefd[i]);
			}
			free(px->pipefd);
			px->pipefd = NULL;
			return (-1);
		}
		i++;
	}
	return (0);
}

int	launch_pipeline(t_pipex *px)
{
	int	nb_pipes;

	nb_pipes = count_pipes(px->tokens);
	px->nb_cmds = nb_pipes + 1;
	if (nb_pipes == 0)
	{
		px->pipefd = NULL;
		return (0);
	}
	if (allocate_pipes(px, nb_pipes) == -1)
		return (-1);
	if (create_pipes(px, nb_pipes) == -1)
	{
		free_all_pipes(px);
		return (-1);
	}
	return (0);
}
