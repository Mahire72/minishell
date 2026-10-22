/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:00:54 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:00:56 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit_args_count(t_token *cmd_token, t_token **args)
{
	int		argc;
	t_token	*tmp;

	argc = 0;
	tmp = NULL;
	if (cmd_token)
		tmp = cmd_token->next;
	while (tmp && tmp->type == ARG)
	{
		argc++;
		tmp = tmp->next;
	}
	if (cmd_token)
		*args = cmd_token->next;
	else
		*args = NULL;
	return (argc);
}

static int	count_args(t_token *cmd_token, t_token **first_arg)
{
	int		argc;
	t_token	*tmp;

	argc = 0;
	tmp = cmd_token->next;
	*first_arg = NULL;
	while (tmp && tmp->type == ARG)
	{
		if (argc == 0)
			*first_arg = tmp;
		argc++;
		tmp = tmp->next;
	}
	return (argc);
}

static int	handle_invalid_numeric(t_data *data, t_token *arg)
{
	write(2, "exit: ", 6);
	write(2, arg->value, ft_strlen(arg->value));
	write(2, ": numeric argument required\n", 27);
	data->exit_code = 2;
	return (0);
}

int	ft_exit(t_data *data, t_token *cmd_token)
{
	int		argc;
	t_token	*args;

	if (!cmd_token)
		return (0);
	argc = count_args(cmd_token, &args);
	write(1, "exit\n", 5);
	if (argc == 0)
	{
		data->exit_code = 0;
		return (0);
	}
	if (!is_numeric(args->value))
		return (handle_invalid_numeric(data, args));
	if (argc > 1)
	{
		write(2, "exit: too many arguments\n", 25);
		data->exit_code = 1;
		return (1);
	}
	data->exit_code = ft_atoi(args->value) % 256;
	return (0);
}
