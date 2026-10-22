/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extractors_word.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:05:32 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:05:34 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	detect_quote_type(const char *line, int *index)
{
	char	quote_type;

	quote_type = OUT_QUOTE;
	if (line[*index] == '\'')
		quote_type = IN_SQUOTE;
	else if (line[*index] == '"')
		quote_type = IN_DQUOTE;
	if (!are_quotes_closed_properly(line))
	{
		while (line[*index])
			(*index)++;
		return (OUT_QUOTE);
	}
	return (quote_type);
}

t_token	*create_word_token(const char *line, int start,
		int end, char quote_type)
{
	char	*raw;
	char	*cleaned;
	t_token	*token;

	raw = ft_substr(line, start, end - start);
	if (!raw)
		return (NULL);
	cleaned = strip_quotes(raw);
	free(raw);
	if (!cleaned)
		return (NULL);
	token = new_token(cleaned, TOKEN_WORD);
	if (!token)
	{
		free(cleaned);
		return (NULL);
	}
	token->quote_type = quote_type;
	return (token);
}

t_token	*extract_word_token(const char *line, int *index)
{
	int		start;
	char	quote_type;

	start = *index;
	quote_type = detect_quote_type(line, index);
	if (quote_type == OUT_QUOTE
		&& (line[*index] == '\'' || line[*index] == '"'))
		return (NULL);
	while (line[*index])
	{
		if ((is_separator(line[*index]) || is_operator_char(line[*index]))
			&& !quote_type_at_pos(line, *index, '\'')
			&& !quote_type_at_pos(line, *index, '"'))
			break ;
		(*index)++ ;
	}
	return (create_word_token(line, start, *index, quote_type));
}
