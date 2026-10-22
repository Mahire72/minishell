/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:05:50 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:05:52 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->quote_type = 0;
	token->next = NULL;
	token->data = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new)
{
	t_token	*tmp;

	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

t_token_type	get_token_type(const char *s)
{
	if (s[0] == '|' && s[1] == '\0')
		return (OP_PIPE);
	if (s[0] == '<' && s[1] == '\0')
		return (OP_REDIR_IN);
	if (s[0] == '>' && s[1] == '\0')
		return (OP_REDIR_OUT);
	if (s[0] == '<' && s[1] == '<')
		return (OP_HEREDOC);
	if (s[0] == '>' && s[1] == '>')
		return (OP_APPEND);
	return (WORD);
}
