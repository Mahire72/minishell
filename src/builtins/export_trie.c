/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_trie.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:01:27 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:01:29 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_env_for_export(char **env)
{
	int		i;
	int		len;
	char	**copy;

	i = 0;
	len = count_env(env);
	copy = malloc(sizeof(char *) * (len + 1));
	if (!copy)
		return (NULL);
	while (i < len)
	{
		copy[i] = ft_strdup(env[i]);
		if (!copy[i])
		{
			while (i > 0)
				free(copy[--i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

static void	ft_sort_env(char **env, int len)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < len - 1)
	{
		j = 0;
		while (j < len - i - 1)
		{
			if (ft_strcmp(env[j], env[j + 1]) > 0)
			{
				tmp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	norm_print_export_var(char *var)
{
	char	*equal;

	write(1, "export ", 7);
	equal = ft_strchr(var, '=');
	if (equal)
	{
		write(1, var, equal - var);
		write(1, "=\"", 2);
		write(1, equal + 1, ft_strlen(equal + 1));
		write(1, "\"\n", 2);
	}
	else
	{
		write(1, var, ft_strlen(var));
		write(1, "\n", 1);
	}
}

void	print_export_sorted(char **env)
{
	int		i;
	int		len;
	char	**copy;

	if (!env)
		return ;
	len = count_env(env);
	copy = copy_env_for_export(env);
	if (!copy)
		return ;
	ft_sort_env(copy, len);
	i = 0;
	while (i < len)
	{
		norm_print_export_var(copy[i]);
		i++;
	}
	i = 0;
	while (i < len)
	{
		free(copy[i]);
		i++;
	}
	free(copy);
}
