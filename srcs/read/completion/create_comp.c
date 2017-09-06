/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_comp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/13 04:48:03 by aroulin           #+#    #+#             */
/*   Updated: 2017/08/31 16:42:42 by aroulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sh.h>


static inline int	check_str(int value[2], char match[127], char *data, char *comp)
{
	int i;

	if (value[0])
		if (value[1])
			return (!((*data) >= match[value[0] - 1] && (*data) <= match[value[0] + 1])
					? management_wildcard(data + 1, comp + 1) : 0);
		else
			return (((*data) >= match[value[0] - 1] && (*data) <= match[value[0] + 1])
					? management_wildcard(data + 1, comp + 1) : 0);
	else
	{
		i = -1;
		while (match[++i])
			if ((value[1] && *data != match[i]) || (!value[1] && *data == match[i]))
				return (management_wildcard(data + 1, comp + 1));
	}
	return (0);
}

static inline int	management_wildcard2(char *data, char *comp)
{
	int		value[2];
	int		i;
	char 	match[127];

	value[0] = 0;
	i = -1;
	ft_bzero(match, 127);
	value[1] = ((*comp) == '!') && (comp++)? 1 : 0;
	while (i < 127 && (*comp) != ']')
	{
		match[++i] = (*comp);
		value[0] = ((*comp) == '-') ? i : value[0];
		(comp)++;
	}
	match[++i] = '\0';
	return (check_str(value, match, data, comp));
}

static inline int 		management_wildcard(char *data, char *comp)
{
	if (*data != '\0' && *comp == '[')
		return (management_wildcard2(data, comp + 1));
	if (*data != '\0' && *comp == '?')
		return (management_wildcard(data + 1, comp + 1));
	if (*data != '\0' && *comp == '*')
		return (management_wildcard(data + 1, comp) + management_wildcard(data, comp + 1));
	if (*data == '\0' && *comp == '*')
		return (management_wildcard(data, comp + 1));
	if (*data && *comp && *data == *comp)
		return (management_wildcard(data + 1, comp + 1));
	if (*data == '\0' && *comp == '\0')
		return (1);
	return (0);
}

void					create_comp(t_read **read_std, t_path f)
{
	DIR					*dir;
	struct dirent		*repo = 0;
	int					index;

	index = 0;
	if ((dir = opendir((!f.path ? "." : f.path))))
	{
		while ((repo = readdir(dir)))
			if ((!f.to_comp && repo->d_name[0] != '.') || (f.to_comp && (management_wildcard(repo->d_name, f.to_comp) || !ft_strncmp(f.to_comp, repo->d_name, ft_strlen(f.to_comp) - 1))))
			{
				init_files(&((*read_std)->comp->tab_->file), repo->d_name,
						repo->d_type, index++);
			}
		closedir(dir);
	}
	(*read_std)->comp->tab_->element = index;
}
