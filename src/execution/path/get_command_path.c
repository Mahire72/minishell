/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command_path.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:02:50 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:02:53 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_command_path(char **argv, t_data *data, t_token *cmd_tokens)
{
	char	*chemin;

	if (ft_strchr(argv[0], '/'))
		return (handle_absolute_path(argv[0], argv, data, cmd_tokens));
	else
	{
		chemin = handle_path_search(argv[0], argv, data->env);
		if (!chemin)
		{
			free_tab(argv);
			free_token_list(cmd_tokens);
			free_all_before_exit(data);
			exit(127);
		}
		return (chemin);
	}
}
