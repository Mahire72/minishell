/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:06:36 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:06:38 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token_list(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head->next;
		if (head->value)
			free(head->value);
		free(head);
		head = tmp;
	}
}

void	free_token_array(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_data(t_data *data)
{
	int	i;

	if (!data)
		return ;
	if (data->tokens)
	{
		free_token_list(data->tokens);
		data->tokens = NULL;
	}
	if (data->env)
	{
		i = 0;
		while (data->env[i])
		{
			free(data->env[i]);
			i++;
		}
		free(data->env);
		data->env = NULL;
	}
}

void	init_pipex(t_pipex *px)
{
	px->infile = NULL;
	px->outfile = NULL;
	px->tokens = NULL;
	px->pipefd = NULL;
	px->data = NULL;
	px->nb_cmds = 0;
	px->pids = NULL;
	px->pids_allocated = 0;
}

void	cleanup_child_memory(t_token *tokens, t_token *cmd_tokens,
	t_data *data, pid_t *pids)
{
	if (cmd_tokens)
		free_token_list(cmd_tokens);
	if (tokens)
		free_token_list(tokens);
	if (pids)
		free(pids);
	if (data)
	{
		free_data(data);
		free(data);
	}
}
