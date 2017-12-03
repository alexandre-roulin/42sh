/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-quel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/01 16:57:04 by jle-quel          #+#    #+#             */
/*   Updated: 2017/12/03 14:03:25 by jle-quel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sh.h>

/*
*************** PUBLIC *********************************************************
*/

uint8_t			print_alias(const int fd, uint8_t mode)
{
	size_t		index;
	t_alias		**ptr;
	t_alias		*temp;

	index = 0;
	ptr = get_alias();
	while (index < USHRT_MAX)
	{
		temp = ptr[index];
		while (temp)
		{
			if (mode & DEFAULT)
			{
				NBR_FD(temp->times, fd);
				CHAR_FD(':', fd);
			}
			STR_FD(temp->variable, fd);
			CHAR_FD('=', fd);
			STR_FD(temp->value, fd);
			CHAR_FD('\n', fd);
			temp = temp->next;
		}
		index++;
	}
	return (0);
}

uint8_t			remove_alias(void)
{
	size_t		index;
	t_alias		**ptr;
	t_alias		*temp;
	t_alias		*memory;

	index = 0;
	ptr = get_alias();
	while (index < USHRT_MAX)
	{
		temp = ptr[index];
		while (temp)
		{
			memory = temp;
			temp = temp->next;
			ft_memdel((void **)&(memory->variable));
			ft_memdel((void **)&(memory->value));
			ft_memdel((void **)&(memory));
		}
		ptr[index] = NULL;
		index++;
	}
	return (0);
}

uint8_t			interactive_alias(char **command)
{
	char		**temp;

	if (arraylen(command) > 1)
		return (error_msg(S42H, "too many arguments", NULL));
	if (ft_strchr(command[0], '=') && (temp = ft_strsplit(command[0], '=')))
	{
		if (arraylen(temp) == 2)
			alias_exist(temp[0]) == NULL ? add_alias(temp) : 0;
		else
		{
			arraydel(&temp);
			return (error_msg(S42H, "Usage: alias [variable=value]", NULL));
		}
		arraydel(&temp);
		return (0);
	}
	else
		return (error_msg(S42H, "Usage: alias [variable=value]", NULL));
}

uint8_t			append_alias(char **command)
{
	if (arraylen(command) > 1)
		return (error_msg(S42H, "too many arguments", NULL));
	else if (command[0])
		read_rc(command[0]);
	else
		read_rc(".42sh_rc");
	return (0);
}

uint8_t			append_rc(char **command)
{
	if (arraylen(command) > 1)
		return (error_msg(S42H, "too many arguments", NULL));
	else if (command[0])
		init_rc(command[0]);
	else
		init_rc(".42sh_rc");
	return (0);
}
