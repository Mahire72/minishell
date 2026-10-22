/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:00:37 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:00:39 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_simple(char *arg)
{
	write(STDOUT_FILENO, arg, ft_strlen(arg));
}

static void	print_with_expansion(char *arg, t_data *data)
{
	char	*expanded;

	expanded = expand_variable(arg, data);
	if (expanded)
	{
		write(STDOUT_FILENO, expanded, ft_strlen(expanded));
		free(expanded);
	}
	else
		write(STDOUT_FILENO, arg, ft_strlen(arg));
}

static void	print_no_quote(char *arg, t_data *data)
{
	char	*arg_stripped;

	arg_stripped = strip_quotes(arg);
	if (!arg_stripped)
		arg_stripped = ft_strdup(arg);
	print_with_expansion(arg_stripped, data);
	free(arg_stripped);
}

void	print_echo_arg(char *arg, int *first, t_data *data, char quote_type)
{
	if (!*first)
		write(STDOUT_FILENO, " ", 1);
	*first = 0;
	if (quote_type == IN_SQUOTE)
		print_simple(arg);
	else if (quote_type == IN_DQUOTE)
		print_with_expansion(arg, data);
	else
		print_no_quote(arg, data);
}

void	ft_echo(t_data *data, t_token *cmd_token)
{
	int		print_newline;
	int		first;
	t_token	*args;

	print_newline = 1;
	first = 1;
	args = cmd_token->next;
	if (!cmd_token)
		return ;
	args = check_echo_n(args, &print_newline);
	while (args)
	{
		if (args->type == ARG)
			print_echo_arg(args->value, &first, data, args->quote_type);
		args = args->next;
	}
	if (print_newline)
		write(STDOUT_FILENO, "\n", 1);
	data->exit_code = 0;
}
