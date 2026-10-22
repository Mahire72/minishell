/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:01:20 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:01:22 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_handle_tilde(t_data *data, char *path)
{
	char	*home;
	char	*tmp;

	if (path[0] != '~')
		return (ft_strdup(path));
	home = get_env_value(data->env, "HOME", ft_strlen("HOME"));
	if (!home)
	{
		write(2, "cd: HOME not set\n", 17);
		data->exit_code = 1;
		return (NULL);
	}
	tmp = ft_strjoin(home, path + 1);
	return (tmp);
}

char	*ft_cd_get_path(t_data *data, t_token *cmd_token)
{
	t_token	*args;
	char	*expanded_path;
	char	*final_path;

	args = NULL;
	if (cmd_token != NULL)
		args = cmd_token->next;
	if (!args)
		return (NULL);
	expanded_path = expand_variable(args->value, data);
	if (!expanded_path)
		expanded_path = ft_strdup(args->value);
	if (!expanded_path)
		return (NULL);
	final_path = ft_handle_tilde(data, expanded_path);
	free(expanded_path);
	return (final_path);
}

int	ft_cd_exec(t_data *data, char *path)
{
	if (!path)
	{
		perror("cd");
		data->exit_code = 1;
		return (1);
	}
	if (chdir(path) == -1)
	{
		perror("cd");
		free(path);
		data->exit_code = 1;
		return (1);
	}
	update_pwd_and_oldpwd(data);
	free(path);
	data->exit_code = 0;
	return (0);
}
