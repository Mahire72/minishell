/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_launch.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:03:44 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:03:46 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	allocate_pids(t_pipex *px, pid_t pids_stack[1024])
{
	if (px->nb_cmds > 1024)
	{
		px->pids = malloc(sizeof(pid_t) * px->nb_cmds);
		if (!px->pids)
			return ;
		px->pids_allocated = 1;
	}
	else
	{
		px->pids = pids_stack;
		px->pids_allocated = 0;
	}
}

static void	fork_all_children(t_pipex *px)
{
	int	i;

	i = 0;
	while (i < px->nb_cmds)
	{
		px->pids[i] = handle_child_process(px, i);
		if (px->pids[i] == -1)
		{
			px->nb_cmds = i;
			break ;
		}
		i++;
	}
}

static void	handle_children_and_cleanup(t_pipex *px)
{
	pid_t	pids_stack[1024];

	allocate_pids(px, pids_stack);
	fork_all_children(px);
	parent_close_pipes_and_wait(px, px->pids);
	if (px->pipefd)
		free_all_pipes(px);
	if (px->pids_allocated && px->pids)
		free(px->pids);
	px->pids = NULL;
}

static int	check_pipe_syntax(t_token *tokens, t_data *data)
{
	t_token	*tmp;

	tmp = tokens;
	if (!tmp || tmp->type == OP_PIPE)
	{
		write(2,
			"Minishell: syntax error near unexpected token `|'\n", 50);
		data->exit_code = 2;
		return (0);
	}
	while (tmp)
	{
		if (tmp->type == OP_PIPE)
		{
			if (!tmp->next || tmp->next->type == OP_PIPE)
			{
				write(2,
					"Minishell: syntax error near unexpected token `|'\n", 50);
				data->exit_code = 2;
				return (0);
			}
		}
		tmp = tmp->next;
	}
	return (1);
}

void	lancer_pipex(t_data *data)
{
	t_pipex				px;
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGINT);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa, NULL);
	init_pipex(&px);
	px.tokens = data->tokens;
	px.data = data;
	if (!check_pipe_syntax(px.tokens, px.data))
	{
		free_all_pipes(&px);
		return ;
	}
	if (launch_pipeline(&px) == -1)
	{
		free_all_pipes(&px);
		return ;
	}
	handle_children_and_cleanup(&px);
	setup_parent_signal();
	if (g_last_signal == 130)
		g_last_signal = 0;
}
