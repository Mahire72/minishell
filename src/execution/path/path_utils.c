/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduzun <eduzun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 09:03:04 by eduzun            #+#    #+#             */
/*   Updated: 2025/09/02 09:03:06 by eduzun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**split_path_var(char *path_var)
{
	char	**repertoires;

	repertoires = ft_split(path_var, ':');
	if (!repertoires)
		perror("Erreur ft_split PATH");
	return (repertoires);
}

char	*get_path_var(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	write(2, "Erreur: La variable PATH n'est pas définie\n", 44);
	return (NULL);
}

char	*construire_chemin_complet(char *repertoire, char *commande)
{
	size_t	len;
	char	*chemin_complet;

	len = ft_strlen(repertoire) + ft_strlen(commande) + 2;
	chemin_complet = malloc(len);
	if (!chemin_complet)
		return (NULL);
	ft_strcpy(chemin_complet, repertoire);
	ft_strcat(chemin_complet, "/");
	ft_strcat(chemin_complet, commande);
	return (chemin_complet);
}

char	*chercher_commande(char **repertoires, char *commande)
{
	int		i;
	char	*chemin;

	i = 0;
	while (repertoires && repertoires[i])
	{
		chemin = construire_chemin_complet(repertoires[i], commande);
		if (!chemin)
			return (NULL);
		if (access(chemin, X_OK) == 0)
			return (chemin);
		free(chemin);
		i++;
	}
	return (NULL);
}

char	*chercher_dans_path(char *commande, char **envp)
{
	char	*path_var;
	char	**repertoires;
	char	*chemin;

	path_var = get_path_var(envp);
	if (!path_var)
		return (NULL);
	repertoires = split_path_var(path_var);
	if (!repertoires)
		return (NULL);
	chemin = chercher_commande(repertoires, commande);
	free_tab(repertoires);
	return (chemin);
}
