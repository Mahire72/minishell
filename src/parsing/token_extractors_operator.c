/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extractors_operator.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:05:26 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:05:28 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator_char(char c)
{
	return (c == '>' || c == '<' || c == '|');
}

t_token	*extract_operator_token(const char *line, int *index)
{
	int		len;
	char	*str;
	t_token	*token;

	len = 1;
	if ((line[*index] == '>' || line[*index] == '<')
		&& line[*index + 1] == line[*index])
		len = 2;
	str = ft_substr(line, *index, len);
	if (!str)
		return (NULL);
	token = new_token(str, TOKEN_OPERATOR);
	if (!token)
		free(str);
	*index += len;
	return (token);
}

t_token	*extract_token(const char *line, int *index)
{
	if (is_operator_char(line[*index]))
		return (extract_operator_token(line, index));
	return (extract_word_token(line, index));
}

t_token	*tokenize(const char *line)
{
	int		i;
	t_token	*head;
	t_token	*token;

	i = 0;
	head = NULL;
	while (line[i])
	{
		if (is_separator(line[i]))
		{
			i++;
			continue ;
		}
		token = extract_token(line, &i);
		if (!token)
		{
			free_tokens(head);
			return (NULL);
		}
		token->type = get_token_type(token->value);
		add_token(&head, token);
	}
	annotate_tokens(head);
	return (head);
}
