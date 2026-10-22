/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:01:33 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:01:35 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_env(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

char	*build_env_var(const char *name, size_t name_len, const char *value)
{
	char	*new_var;

	new_var = malloc(name_len + 1 + ft_strlen(value) + 1);
	if (!new_var)
		return (NULL);
	ft_memcpy(new_var, name, name_len);
	new_var[name_len] = '=';
	ft_strcpy(new_var + name_len + 1, value);
	return (new_var);
}

bool	ft_update_env_var(t_data *data, const char *name,
			size_t name_len, const char *value)
{
	int		i;
	char	*new_var;

	if (!data || !data->env || !name || !value)
		return (false);
	new_var = build_env_var(name, name_len, value);
	if (!new_var)
		return (false);
	i = 0;
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], name, name_len) == 0
			&& data->env[i][name_len] == '=')
		{
			free(data->env[i]);
			data->env[i] = new_var;
			return (true);
		}
		i++;
	}
	free(new_var);
	return (false);
}

bool	add_env_var(t_data *data, const char *new_var)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	while (data->env && data->env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (false);
	j = 0;
	while (j < i)
	{
		new_env[j] = data->env[j];
		j++;
	}
	new_env[i] = ft_strdup(new_var);
	new_env[i + 1] = NULL;
	free(data->env);
	data->env = new_env;
	return (true);
}
