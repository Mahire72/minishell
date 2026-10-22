/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:02:18 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:02:21 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	free_all_before_exit(t_data *data)
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
	}
	free(data);
}

void	handle_builtin_and_exit(t_data *data, t_token *cmd_tokens, char **argv)
{
	if (handle_builtin(data, cmd_tokens))
	{
		free_tab(argv);
		exit(EXIT_SUCCESS);
	}
}
