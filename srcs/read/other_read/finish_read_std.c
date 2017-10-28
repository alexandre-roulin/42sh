/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish_read_std.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/24 16:43:48 by aroulin           #+#    #+#             */
/*   Updated: 2017/10/18 21:57:36 by aroulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sh.h>

static char		*convert_to_str(t_cmd *cmd)
{
	int			len;
	char		*line;

	len = 0;
	while (cmd->next)
	{
		len++;
		cmd = cmd->next;
	}
	line = (char *)ft_memalloc(sizeof(char) * (len + 1));
	cmd = first_cmd(cmd, 1);
	len = 0;
	while (cmd->c)
	{
		line[len] = cmd->c;
		cmd = cmd->next;
		len++;
	}
	return (line);
}

static int		empty_cmd(t_cmd *cmd)
{
	while (cmd->c)
	{
		if (cmd->c != 32)
			return (0);
		cmd = cmd->next;
	}
	return (1);
}

static char 	*return_line(t_read **read_std)
{
	char		*line;

	if (*read_std)
	{
		(*read_std)->finish = 0;
		if (get_len_prompt(-42) != -2)
			make_list_hist((*read_std));
		line = convert_to_str(first_cmd((*read_std)->cmd, 1));
		if (get_len_prompt(-42) == -2)
			memdel_read(read_std);
		return (line);
	}
	return (NULL);
}

char			*finish_read_std(t_read **read_std)
{
	t_cmd		*tmp;

	tmp = first_cmd(gbl_save_read(NULL), 1);
	reset_history();
	memdel_outstanding();
	if ((*read_std)->history)
		memdel_cmd(&tmp);
	if (signal_reception(-1) || empty_cmd(first_cmd((*read_std)->cmd, 1)))
	{
		if (signal_reception(-1))
			NL;
		memdel_read(read_std);
	}
	return (return_line(read_std));
}
