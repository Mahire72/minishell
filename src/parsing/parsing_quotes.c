/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:05:21 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:05:23 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	are_quotes_closed_properly(const char *line)
{
	int		i;
	bool	in_single_quote;
	bool	in_double_quote;

	i = 0;
	in_single_quote = false;
	in_double_quote = false;
	while (line[i])
	{
		if (line[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (line[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		i++;
	}
	return (!in_single_quote && !in_double_quote);
}

bool	quote_type_at_pos(const char *line, int pos, char c)
{
	int		i;
	bool	in_single_quote;
	bool	in_double_quote;

	i = 0;
	in_single_quote = false;
	in_double_quote = false;
	while (i <= pos)
	{
		if (line[i] == '\'' && !in_double_quote)
		{
			if (in_single_quote && i == pos)
				return (true);
			in_single_quote = !in_single_quote;
		}
		else if (line[i] == '"' && !in_single_quote)
		{
			if (in_double_quote && i == pos)
				return (true);
			in_double_quote = !in_double_quote;
		}
		i++;
	}
	return (get_quote_state(c, in_single_quote, in_double_quote));
}

bool	get_quote_state(char c, bool in_single_quote, bool in_double_quote)
{
	if (c == '\'')
		return (in_single_quote);
	return (in_double_quote);
}

int	count_valid_quotes(const char *line)
{
	int		total;
	int		i;

	total = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' && quote_type_at_pos(line, i, '\''))
			total++;
		else if (line[i] == '"' && quote_type_at_pos(line, i, '"'))
			total++;
		i++;
	}
	return (total);
}

char	*strip_quotes(const char *str)
{
	char	*final_line;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	if (!are_quotes_closed_properly(str))
		return (NULL);
	final_line = malloc(ft_strlen(str) - count_valid_quotes(str) + 1);
	if (!final_line)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if ((str[i] == '\'' && quote_type_at_pos(str, i, '\''))
			|| (str[i] == '"' && quote_type_at_pos(str, i, '"')))
			i++;
		else
			final_line[j++] = str[i++];
	}
	final_line[j] = '\0';
	return (final_line);
}
