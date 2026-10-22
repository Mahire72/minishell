/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:06:19 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 11:20:15 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_readline(void)
{
	rl_clear_history();
}

int	process_tokens(t_data *data, t_token *tokens, char *line)
{
	data->tokens = tokens;
	if (handle_heredoc_check(data, tokens, line) == 0)
		return (0);
	if (handle_variable_assignment(data, tokens, line) == 0)
		return (0);
	if (handle_builtin_commands(data, tokens, line) == 0)
		return (0);
	lancer_pipex(data);
	free_token_list(tokens);
	data->tokens = NULL;
	free(line);
	if (g_last_signal == 130)
		g_last_signal = 0;
	return (0);
}

void	exit_prompt(t_data *data)
{
	char	*line;
	t_token	*tokens;
	int		status;

	while (1)
	{
		status = handle_signal_and_input(data, &line);
		if (status == 1)
			break ;
		if (status == 0)
			continue ;
		if (*line)
			add_history(line);
		tokens = tokenize(line);
		if (tokens)
			process_tokens(data, tokens, line);
		else
			free(line);
	}
	rl_clear_history();
}

int	handle_builtin(t_data *data, t_token *cmd_token)
{
	if (!cmd_token || !cmd_token->value)
		return (0);
	if (ft_strcmp(cmd_token->value, "exit") == 0)
		return (ft_exit(data, cmd_token));
	if (ft_strcmp(cmd_token->value, "echo") == 0)
		ft_echo(data, cmd_token);
	else if (ft_strcmp(cmd_token->value, "pwd") == 0)
		ft_pwd(data, cmd_token);
	else if (ft_strcmp(cmd_token->value, "cd") == 0)
		ft_cd(data, cmd_token);
	else if (ft_strcmp(cmd_token->value, "env") == 0)
		ft_env(data);
	else if (ft_strcmp(cmd_token->value, "export") == 0)
		ft_export(data, cmd_token);
	else if (ft_strcmp(cmd_token->value, "unset") == 0)
		ft_unset(data, cmd_token);
	else
		return (0);
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;

	(void)argc;
	(void)argv;
	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	ft_memset(data, 0, sizeof(t_data));
	data->env = copy_env(envp);
	data->exit_code = 0;
	setup_parent_signal();
	exit_prompt(data);
	cleanup_readline();
	free_data(data);
	free(data);
	return (0);
}
