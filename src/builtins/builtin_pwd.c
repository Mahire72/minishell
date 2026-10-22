/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:01:07 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:01:09 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_data *data, t_token *args)
{
	char	*cwd;

	(void)args;
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		write(1, cwd, ft_strlen(cwd));
		write(1, "\n", 1);
		free(cwd);
		data->exit_code = 0;
	}
	else
	{
		perror("minishell: pwd");
		data->exit_code = 1;
	}
	return (data->exit_code);
}
