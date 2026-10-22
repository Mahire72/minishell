/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_ctrl_c.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:06:31 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:06:33 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cleanup_on_signal(t_data *data)
{
	if (data->tokens)
	{
		free_token_list(data->tokens);
		data->tokens = NULL;
	}
}

int	handle_ctrl_c_signal(t_data *data, char *line)
{
	if (g_last_signal != 130)
		return (-1);
	data->exit_code = 130;
	cleanup_on_signal(data);
	if (*line == '\0')
	{
		g_last_signal = 0;
		return (0);
	}
	return (-1);
}

int	handle_signal_and_input(t_data *data, char **line)
{
	int	ret;

	setup_parent_signal();
	*line = read_user_input();
	if (!*line)
	{
		printf("exit\n");
		return (1);
	}
	ret = handle_ctrl_c_signal(data, *line);
	if (ret == 0)
	{
		free(*line);
		*line = NULL;
		return (0);
	}
	if (**line == '\0')
	{
		free(*line);
		return (0);
	}
	return (-1);
}
