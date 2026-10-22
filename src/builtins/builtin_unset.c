/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:01:13 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:01:15 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	remove_env_var(t_data *data, const char *name)
{
	int		i;
	size_t	name_len;

	if (!data || !data->env || !name)
		return (false);
	name_len = ft_strlen(name);
	i = 0;
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], name, name_len) == 0
			&& (data->env[i][name_len] == '='
			|| data->env[i][name_len] == '\0'))
		{
			free(data->env[i]);
			while (data->env[i + 1])
			{
				data->env[i] = data->env[i + 1];
				i++;
			}
			data->env[i] = NULL;
			return (true);
		}
		i++;
	}
	return (false);
}

int	is_valid_identifier(const char *str)
{
	size_t	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	ft_unset(t_data *data, t_token *cmd_token)
{
	t_token	*current;

	current = cmd_token->next;
	while (current)
	{
		if (!is_valid_identifier(current->value))
			return (0);
		else
			remove_env_var(data, current->value);
		current = current->next;
	}
	return (0);
}
