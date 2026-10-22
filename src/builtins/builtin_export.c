/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:01:00 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:01:03 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_export_with_equal(t_data *data, const char *arg)
{
	char	*equal_sign;
	size_t	name_len;

	equal_sign = ft_strchr(arg, '=');
	name_len = equal_sign - arg;
	if (!ft_update_env_var(data, arg, name_len, equal_sign + 1))
	{
		if (!add_env_var(data, arg))
			data->exit_code = 1;
		else
			data->exit_code = 0;
	}
	else
		data->exit_code = 0;
}

static void	handle_export_without_equal(t_data *data, const char *arg)
{
	if (!ft_update_env_var(data, arg, ft_strlen(arg), NULL))
	{
		if (!add_env_var(data, arg))
			data->exit_code = 1;
		else
			data->exit_code = 0;
	}
	else
		data->exit_code = 0;
}

void	handle_export_arg(t_data *data, const char *arg)
{
	if (!is_valid_identifier(arg))
	{
		write(2, "export: `", 9);
		write(2, arg, ft_strlen(arg));
		write(2, "`: not a valid identifier\n", 27);
		data->exit_code = 1;
		return ;
	}
	if (ft_strchr(arg, '='))
		handle_export_with_equal(data, arg);
	else
		handle_export_without_equal(data, arg);
}

int	ft_export(t_data *data, t_token *cmd_token)
{
	t_token	*token;
	int		error;

	if (!cmd_token || !cmd_token->next)
	{
		print_export_sorted(data->env);
		data->exit_code = 0;
		return (0);
	}
	error = 0;
	token = cmd_token->next;
	while (token)
	{
		if (!is_valid_identifier(token->value))
			error |= print_invalid_identifier(token->value);
		else
			handle_export_arg(data, token->value);
		token = token->next;
	}
	data->exit_code = error;
	return (error);
}
