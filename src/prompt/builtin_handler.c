/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:06:01 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:06:03 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_variable_assignment(t_data *data, t_token *tokens, char *line)
{
	if (!(tokens->type == CMD && ft_strchr(tokens->value, '=')))
		return (-1);
	handle_export_arg(data, tokens->value);
	free_token_list(tokens);
	data->tokens = NULL;
	free(line);
	return (0);
}

static int	handle_exit_builtin(t_data *data, t_token *tokens, char *line)
{
	int	ret;
	int	exit_code;

	ret = ft_exit(data, tokens);
	free_token_list(tokens);
	data->tokens = NULL;
	free(line);
	if (ret == 0)
	{
		exit_code = data->exit_code;
		rl_clear_history();
		cleanup_readline();
		free_data(data);
		free(data);
		exit(exit_code);
	}
	return (0);
}

static int	has_redirections(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == OP_REDIR_OUT || tokens->type == OP_REDIR_IN
			|| tokens->type == OP_HEREDOC || tokens->type == OP_APPEND)
			return (1);
		tokens = tokens->next;
	}
	return (0);
}

static int	has_pipes(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == OP_PIPE)
			return (1);
		tokens = tokens->next;
	}
	return (0);
}

int	handle_builtin_commands(t_data *data, t_token *tokens, char *line)
{
	if (tokens->type != CMD)
		return (-1);
	if (has_redirections(tokens) || has_pipes(tokens))
		return (-1);
	if (ft_strcmp(tokens->value, "echo") == 0)
		ft_echo(data, tokens);
	else if (ft_strcmp(tokens->value, "cd") == 0)
		ft_cd(data, tokens);
	else if (ft_strcmp(tokens->value, "exit") == 0)
		return (handle_exit_builtin(data, tokens, line));
	else if (ft_strcmp(tokens->value, "export") == 0)
		ft_export(data, tokens);
	else if (ft_strcmp(tokens->value, "unset") == 0)
		ft_unset(data, tokens);
	else
		return (-1);
	free_token_list(tokens);
	data->tokens = NULL;
	free(line);
	if (g_last_signal == 130)
		g_last_signal = 0;
	return (0);
}
