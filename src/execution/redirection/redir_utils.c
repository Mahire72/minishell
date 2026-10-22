/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:04:39 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:04:41 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_outfile(t_token *tokens)
{
	char	*outfile;

	outfile = NULL;
	while (tokens)
	{
		if ((tokens->type == OP_REDIR_OUT || tokens->type == OP_APPEND)
			&& tokens->next
			&& tokens->next->type == OUTFILE)
			outfile = tokens->next->value;
		tokens = tokens->next;
	}
	return (outfile);
}

char	*find_infile(t_token *tokens)
{
	char	*infile;

	infile = NULL;
	while (tokens)
	{
		if ((tokens->type == OP_REDIR_IN || tokens->type == OP_HEREDOC)
			&& tokens->next
			&& tokens->next->type == INFILE)
			infile = tokens->next->value;
		tokens = tokens->next;
	}
	return (infile);
}

int	stdin_redir_exists(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == OP_REDIR_IN)
			return (1);
		tokens = tokens->next;
	}
	return (0);
}

int	stdout_redir_exists(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == OP_REDIR_OUT || tokens->type == OP_APPEND)
			return (1);
		tokens = tokens->next;
	}
	return (0);
}
