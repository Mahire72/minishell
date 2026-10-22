/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:03:09 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:03:11 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**allocate_array(char const *s, char c)
{
	char	**array;

	array = malloc((nb_wrd(s, c) + 1) * sizeof(char *));
	return (array);
}

static int	fill_array(char **array, char const *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < nb_wrd(s, c))
	{
		while (s[j] == c)
			j++;
		array[i] = allocate_word(s, c, &j);
		if (!array[i])
		{
			free_split(array, i);
			return (0);
		}
		i++;
	}
	array[i] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**array;

	if (!s)
		return (NULL);
	array = allocate_array(s, c);
	if (!array)
		return (NULL);
	if (!fill_array(array, s, c))
		return (NULL);
	return (array);
}
