/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:05:37 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:05:39 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '\0');
}

t_token	*handle_redir_and_file(t_token *cur)
{
	t_token	*next;

	if (cur->type == OP_REDIR_IN || cur->type == OP_HEREDOC
		|| cur->type == OP_REDIR_OUT || cur->type == OP_APPEND)
	{
		next = cur->next;
		if (next && next->type == WORD)
		{
			if (cur->type == OP_REDIR_IN)
				next->type = INFILE;
			else if (cur->type == OP_REDIR_OUT || cur->type == OP_APPEND)
				next->type = OUTFILE;
			else if (cur->type == OP_HEREDOC)
				next->type = DELIM;
			return (next->next);
		}
	}
	return (cur->next);
}

t_token	*process_token(t_token *cur, int *expecting_command)
{
	if (cur->type == OP_PIPE)
	{
		*expecting_command = 1;
		return (cur->next);
	}
	else if (cur->type == WORD)
	{
		if (*expecting_command)
		{
			cur->type = CMD;
			*expecting_command = 0;
		}
		else
			cur->type = ARG;
		return (cur->next);
	}
	else
		return (handle_redir_and_file(cur));
}

void	annotate_tokens(t_token *tokens)
{
	t_token	*cur;
	int		expecting_command;

	cur = tokens;
	expecting_command = 1;
	while (cur)
		cur = process_token(cur, &expecting_command);
}
