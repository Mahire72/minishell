/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_builder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:02:01 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:02:03 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_argv(t_token *tokens)
{
	int		count;
	t_token	*cur;

	count = 0;
	cur = tokens;
	while (cur && cur->type != OP_PIPE)
	{
		if (cur->type == OP_REDIR_IN || cur->type == OP_REDIR_OUT
			|| cur->type == OP_APPEND)
		{
			cur = cur->next;
			if (cur)
				cur = cur->next;
			continue ;
		}
		if (cur->type == CMD || cur->type == ARG)
			count++;
		cur = cur->next;
	}
	return (count);
}

static void	fill_argv(char **argv, t_token *tokens)
{
	t_token	*cur;
	int		i;

	cur = tokens;
	i = 0;
	while (cur && cur->type != OP_PIPE)
	{
		if (cur->type == OP_REDIR_IN || cur->type == OP_REDIR_OUT
			|| cur->type == OP_APPEND)
		{
			cur = cur->next;
			if (cur)
				cur = cur->next;
			continue ;
		}
		if (cur->type == CMD || cur->type == ARG)
			argv[i++] = ft_strdup(cur->value);
		cur = cur->next;
	}
	argv[i] = NULL;
}

char	**construire_argv(t_token *tokens)
{
	int		count;
	char	**argv;

	count = count_argv(tokens);
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	fill_argv(argv, tokens);
	return (argv);
}
