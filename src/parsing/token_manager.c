/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:05:44 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:05:46 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*get_tokens_for_command(t_token *tokens, int cmd_index)
{
	int	current_cmd;

	current_cmd = 0;
	while (tokens && current_cmd < cmd_index)
	{
		if (tokens->type == OP_PIPE)
			current_cmd++;
		tokens = tokens->next;
	}
	if (!tokens)
		return (NULL);
	if (tokens->type == OP_PIPE)
		tokens = tokens->next;
	return (copy_tokens_until_pipe(tokens));
}

t_token	*copy_token(t_token *src)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->type = src->type;
	new_node->quote_type = src->quote_type;
	new_node->data = src->data;
	new_node->value = ft_strdup(src->value);
	if (!new_node->value)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

static void	append_token(t_token **head, t_token **tail, t_token *new_node)
{
	if (!*head)
	{
		*head = new_node;
		*tail = new_node;
	}
	else
	{
		(*tail)->next = new_node;
		*tail = new_node;
	}
}

t_token	*copy_tokens_until_pipe(t_token *start)
{
	t_token	*new_head;
	t_token	*new_tail;
	t_token	*new_node;

	new_head = NULL;
	new_tail = NULL;
	while (start && start->type != OP_PIPE)
	{
		new_node = copy_token(start);
		if (!new_node)
		{
			free_token_list(new_head);
			return (NULL);
		}
		append_token(&new_head, &new_tail, new_node);
		start = start->next;
	}
	return (new_head);
}
