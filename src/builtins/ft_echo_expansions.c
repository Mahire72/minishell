/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_expansions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:01:41 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:01:43 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	expand_exit_code(char *buffer, size_t j, t_data *data)
{
	char	*code_str;
	int		k;

	code_str = ft_itoa(data->exit_code);
	if (!code_str)
		return (j);
	k = 0;
	while (code_str[k] && j < 1023)
		buffer[j++] = code_str[k++];
	free(code_str);
	return (j);
}

static size_t	expand_env_var(t_expansion *exp)
{
	size_t	start;
	size_t	var_len;
	char	*var_name;
	char	*value;
	size_t	k;

	start = exp->i;
	while (exp->input[exp->i] && (ft_isalnum(exp->input[exp->i])
			|| exp->input[exp->i] == '_'))
		exp->i++;
	var_len = exp->i - start;
	var_name = ft_substr(exp->input, start, var_len);
	if (!var_name)
		return (exp->j);
	value = get_env_value(exp->data->env, var_name, var_len);
	k = 0;
	while (value && value[k] && exp->j < 1023)
		exp->buffer[exp->j++] = value[k++];
	free(var_name);
	return (exp->j);
}

static void	copy_normal_char(t_expansion *exp)
{
	exp->buffer[exp->j++] = exp->input[exp->i++];
}

static void	handle_dollar(t_expansion *exp)
{
	if (exp->input[exp->i] == '?')
	{
		exp->j = expand_exit_code(exp->buffer, exp->j, exp->data);
		exp->i++;
	}
	else if (ft_isalpha(exp->input[exp->i]) || exp->input[exp->i] == '_')
		exp->j = expand_env_var(exp);
	else
	{
		exp->buffer[exp->j++] = '$';
		if (exp->input[exp->i])
			exp->buffer[exp->j++] = exp->input[exp->i++];
	}
}

char	*expand_variable(const char *input, t_data *data)
{
	t_expansion	exp;

	if (!input)
		return (NULL);
	exp.i = 0;
	exp.j = 0;
	exp.input = input;
	exp.data = data;
	while (exp.input[exp.i] && exp.j < 1023)
	{
		if (exp.input[exp.i] == '$')
		{
			exp.i++;
			handle_dollar(&exp);
		}
		else
			copy_normal_char(&exp);
	}
	exp.buffer[exp.j] = '\0';
	return (ft_strdup(exp.buffer));
}
