/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:04:23 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:04:25 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_line_input(int write_fd, char *line)
{
	write(write_fd, line, ft_strlen(line));
	write(write_fd, "\n", 1);
	free(line);
}

void	copy_delimiter(char *dest, const char *src, int max_len)
{
	int	i;

	i = 0;
	while (src[i] && i < max_len - 1)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

void	free_heredoc_data(t_data *data)
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
			free(data->env[i++]);
		free(data->env);
	}
	free(data);
}
