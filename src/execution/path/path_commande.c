/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_commande.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:02:58 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:03:00 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_path_errors(char *cmd, char **argv, char *chemin)
{
	if (access(chemin, F_OK) == -1)
	{
		write_errno_error(cmd);
		free_tab(argv);
		free(chemin);
		exit(127);
	}
	if (access(chemin, X_OK) != 0)
	{
		write_error(cmd, "Permission denied");
		free_tab(argv);
		free(chemin);
		exit(126);
	}
}

char	*handle_path_search(char *cmd, char **argv, char **env)
{
	char	*chemin;

	chemin = chercher_dans_path(cmd, env);
	if (!chemin)
	{
		write(2, " command not found\n", 19);
		return (NULL);
	}
	handle_path_errors(cmd, argv, chemin);
	return (chemin);
}

int	executer_commande_depuis_tokens_struct(t_data *data,
		t_token *cmd_tokens)
{
	char	**argv;
	int		exit_code;

	if (!cmd_tokens)
	{
		write(2, "Erreur: tokens commande invalides\n", \
			sizeof("Erreur: tokens commande invalides\n") - 1);
		return (EXIT_FAILURE);
	}
	argv = construire_argv(cmd_tokens);
	if (!argv)
	{
		write(2, "Erreur allocation mémoire argv\n", \
			sizeof("Erreur allocation mémoire argv\n") - 1);
		return (EXIT_FAILURE);
	}
	exit_code = executer_commande(argv, data, cmd_tokens);
	free_tab(argv);
	return (exit_code);
}
