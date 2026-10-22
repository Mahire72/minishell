/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_commande.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:02:24 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:02:26 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	prepare_execution(char **argv, t_data *data, t_token *cmd_tokens)
{
	handle_expansion_all_args(argv, data);
	handle_expansion_argv0(argv, data);
	if (!argv || !argv[0] || argv[0][0] == '\0')
	{
		free_tab(argv);
		free_token_list(cmd_tokens);
		free_all_before_exit(data);
		exit(0);
	}
	handle_builtin_and_exit(data, cmd_tokens, argv);
}

static void	cleanup_before_exec(t_data *data, t_token *cmd_tokens)
{
	if (data->tokens)
	{
		free_token_list(data->tokens);
		data->tokens = NULL;
	}
	free_token_list(cmd_tokens);
	free(data);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
}

int	executer_commande(char **argv, t_data *data, t_token *cmd_tokens)
{
	char	*chemin;
	char	**env_copy;

	prepare_execution(argv, data, cmd_tokens);
	chemin = get_command_path(argv, data, cmd_tokens);
	env_copy = data->env;
	cleanup_before_exec(data, cmd_tokens);
	execve(chemin, argv, env_copy);
	write_errno_error("execve");
	free_tab(argv);
	if (!ft_strchr(argv[0], '/'))
		free(chemin);
	exit(126);
}
