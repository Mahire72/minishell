/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_child_exec.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:03:32 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:03:34 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	safe_free_pids(t_pipex *px)
{
	if (px->pids_allocated && px->pids)
		free(px->pids);
}

void	free_and_exit(t_pipex *px, t_token *cmd_tokens, int code)
{
	if (cmd_tokens)
		free_token_list(cmd_tokens);
	if (px->tokens)
	{
		free_token_list(px->tokens);
		px->data->tokens = NULL;
	}
	if (px->pids)
		safe_free_pids(px);
	free_all_pipes(px);
	free_data(px->data);
	free(px->data);
	exit(code);
}

static t_token	*prepare_child_execution(t_pipex *px, int cmd_index)
{
	t_token	*cmd_tokens;
	int		ret;

	cmd_tokens = get_tokens_for_command(px->tokens, cmd_index);
	if (!cmd_tokens)
		free_and_exit(px, NULL, EXIT_FAILURE);
	if (px->nb_cmds > 1)
	{
		if (cmd_index == 0)
			child1(px);
		else if (cmd_index == px->nb_cmds - 1)
			child2(px);
		else
			child_intermediate(px, cmd_index);
	}
	else
		close_all_pipes(px);
	ret = handle_redirections(cmd_tokens);
	if (ret == -2)
		free_and_exit(px, cmd_tokens, 130);
	else if (ret == -1)
		free_and_exit(px, cmd_tokens, EXIT_FAILURE);
	return (cmd_tokens);
}

pid_t	handle_child_process(t_pipex *px, int cmd_index)
{
	pid_t				pid;
	t_token				*cmd_tokens;
	struct sigaction	sa;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		sa.sa_handler = SIG_DFL;
		sigaction(SIGINT, &sa, NULL);
		cmd_tokens = prepare_child_execution(px, cmd_index);
		if (handle_builtin(px->data, cmd_tokens))
			free_and_exit(px, cmd_tokens, px->data->exit_code);
		free_token_list(px->tokens);
		px->data->tokens = NULL;
		free_all_pipes(px);
		exit(executer_commande_depuis_tokens_struct(px->data, cmd_tokens));
	}
	return (pid);
}
