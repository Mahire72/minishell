/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:02:31 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:02:33 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_expansion_argv0(char **argv, t_data *data)
{
	char	*expanded;
	int		i;

	if (!argv || !argv[0])
		return ;
	if (argv[0][0] == '$')
	{
		expanded = expand_variable(argv[0], data);
		free(argv[0]);
		argv[0] = expanded;
	}
	if (!argv[0] || argv[0][0] == '\0')
	{
		i = 0;
		while (argv[i + 1])
		{
			argv[i] = argv[i + 1];
			i++;
		}
		argv[i] = NULL;
	}
}

void	handle_expansion_all_args(char **argv, t_data *data)
{
	char	*expanded;
	int		i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		if (ft_strchr(argv[i], '$'))
		{
			expanded = expand_variable(argv[i], data);
			if (expanded)
			{
				free(argv[i]);
				argv[i] = expanded;
			}
		}
		i++;
	}
}
