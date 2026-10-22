/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   absolute_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:01:56 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:01:57 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_directory(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		return (1);
	return (0);
}

static void	handle_is_directory(char *argv0, char **argv,
	t_token *cmd_tokens, t_data *data)
{
	if (is_directory(argv0))
	{
		fprintf(stderr, "%s: Is a directory\n", argv0);
		free_tab(argv);
		free_token_list(cmd_tokens);
		free_all_before_exit(data);
		exit(126);
	}
}

void	handle_no_file(char *argv0, char **argv,
	t_token *cmd_tokens, t_data *data)
{
	if (access(argv0, F_OK) == -1)
	{
		write_errno_error(argv0);
		free_tab(argv);
		free_token_list(cmd_tokens);
		free_all_before_exit(data);
		exit(127);
	}
}

void	handle_no_permission(char *argv0, char **argv,
	t_token *cmd_tokens, t_data *data)
{
	if (access(argv0, X_OK) != 0)
	{
		write_error(argv0, "Permission denied");
		free_tab(argv);
		free_token_list(cmd_tokens);
		free_all_before_exit(data);
		exit(126);
	}
}

char	*handle_absolute_path(char *argv0, char **argv,
	t_data *data, t_token *cmd_tokens)
{
	handle_is_directory(argv0, argv, cmd_tokens, data);
	handle_no_file(argv0, argv, cmd_tokens, data);
	handle_no_permission(argv0, argv, cmd_tokens, data);
	return (argv0);
}
