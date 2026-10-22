/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:01:47 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:01:49 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd_count_args(t_token *cmd_token)
{
	t_token	*args;
	int		count;

	count = 0;
	args = NULL;
	if (cmd_token != NULL)
		args = cmd_token->next;
	while (args && args->type == ARG)
	{
		count++;
		args = args->next;
	}
	return (count);
}

int	arg_is_valid_n(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	if (arg[i] == '\0')
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

t_token	*check_echo_n(t_token *args, int *print_newline)
{
	*print_newline = 1;
	while (args && args->type == ARG && arg_is_valid_n(args->value))
	{
		*print_newline = 0;
		args = args->next;
	}
	return (args);
}

char	*get_env_value(char **env, const char *name, size_t len)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return ("");
}

int	print_invalid_identifier(const char *value)
{
	const char	*prefix = "export: `";
	const char	*suffix = "': not a valid identifier\n";

	write(STDERR_FILENO, prefix, ft_strlen(prefix));
	if (value)
		write(STDERR_FILENO, value, ft_strlen(value));
	write(STDERR_FILENO, suffix, ft_strlen(suffix));
	return (1);
}
