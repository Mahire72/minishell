/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:00:30 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:00:33 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_pwd_and_oldpwd(t_data *data)
{
	char	*oldpwd;
	char	*newpwd;

	oldpwd = get_env_value(data->env, "PWD", ft_strlen("PWD"));
	newpwd = getcwd(NULL, 0);
	if (oldpwd != NULL)
		ft_update_env_var(data, "OLDPWD", 6, oldpwd);
	if (newpwd != NULL)
	{
		ft_update_env_var(data, "PWD", 3, newpwd);
		free(newpwd);
	}
}

int	ft_cd_home(t_data *data)
{
	char	*home;

	home = get_env_value(data->env, "HOME", ft_strlen("HOME"));
	if (home == NULL)
	{
		data->exit_code = 1;
		write(2, "cd: HOME not set\n", 17);
		return (1);
	}
	if (chdir(home) == -1)
	{
		data->exit_code = 1;
		perror("cd");
		return (1);
	}
	update_pwd_and_oldpwd(data);
	data->exit_code = 0;
	return (0);
}

int	ft_cd(t_data *data, t_token *cmd_token)
{
	int		arg_count;
	char	*path;

	arg_count = ft_cd_count_args(cmd_token);
	if (arg_count > 1)
	{
		write(2, "cd: too many arguments\n", 23);
		data->exit_code = 1;
		return (1);
	}
	path = ft_cd_get_path(data, cmd_token);
	if (!path)
		return (ft_cd_home(data));
	return (ft_cd_exec(data, path));
}
