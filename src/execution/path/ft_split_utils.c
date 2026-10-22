/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:02:36 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:02:38 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cpy_wrd(char *dest, const char *src, char charset)
{
	int	i;

	i = 0;
	while (src[i] && src[i] != charset)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

char	*allocate_word(const char *str, char charset, int *index)
{
	int		len;
	char	*word;

	len = len_wrd(str + *index, charset);
	word = malloc((len + 1) * sizeof(char));
	if (!word)
		return (NULL);
	cpy_wrd(word, str + *index, charset);
	*index += len;
	return (word);
}

int	len_wrd(const char *str, char charset)
{
	int	i;

	i = 0;
	while (str && str[i] && str[i] != charset)
		i++;
	return (i);
}

int	nb_wrd(const char *str, char charset)
{
	int	count;

	count = 0;
	while (str && *str)
	{
		while (*str == charset)
			str++;
		if (*str && *str != charset)
			count++;
		while (*str && *str != charset)
			str++;
	}
	return (count);
}
