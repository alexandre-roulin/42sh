/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelazou <sbelazou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/09 17:36:39 by sbelazou          #+#    #+#             */
/*   Updated: 2017/11/15 21:14:23 by sbelazou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sh.h>

char				*ft_path(char *var, unsigned int begin)
{
	char			*path;
	unsigned int	i;
	unsigned int	size;

	i = 0;
	if ((size = (ft_strlen(var) - begin) + 1) == 1 ||
		!(path = malloc(sizeof(char) * (ft_strlen(var) - begin) + 1)))
		return (NULL);
	while (var[begin])
	{
		path[i] = var[begin];
		i++;
		begin++;
	}
	path[i] = 0;
	return (path);
}

static int			cd_home(char **env)
{
	char			*path;
	signed int		i;

	if ((i = search_in_tab(env, "HOME=")) == -1 ||
		(path = ft_path(env[i], 5)) == NULL)
		return (1);
	if (chdir(path) == -1)
		return (1);
	ft_putendl(path);
	free(path);
	return (0);
}


static int			cd_path(char *path)
{
	if (chdir(path) == -1)
		return (1);
	return (0);
}

static int			cd_oldpwd(char **env)
{
	char			*path;
	signed int		i;

	i = -1;
	if ((i = search_in_tab(env, "OLDPWD=")) == -1 ||
		(path = ft_path(env[i], 7)) == NULL)
		return (1);
	if (chdir(path) == -1)
		return (1);
	free(path);
	return (0);
}

uint8_t				builtin_cd(t_node *node, int info)
{
	int				ret;
	char			**env;

	(void)info;
	env = node->content->env_option ? node->content->env : env_table(NULL, ENV_REC);
	if (!node->content->command[1] || !ft_strcmp(node->content->command[1], "~") ||
	!ft_strcmp(node->content->command[1], "--"))
		ret = cd_home(env);
	else if (!ft_strcmp(node->content->command[1], "-"))
		ret = cd_oldpwd(env);
	else
		ret = cd_path(node->content->command[1]);
	if (ret == 0)
		refresh_varenv(env);
	else
		return (cd_check_error(node->content->command[1]));
	return (var_return(ret));
}