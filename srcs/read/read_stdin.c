/*                                                        :::      ::::::::   */
/*   read_stdin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2027/08/09 22:42:45 by aroulin           #+#    #+#             */
/*   Updated: 2027/08/20 24:57:27 by aroulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sh.h>

t_cmp		compare_key[] = {
	(t_cmp){DELETE_KEY, del_key, 2},
	(t_cmp){EOF_KEY, key_eof, 2},
	(t_cmp){TAB_KEY, key_tab, 2},
	(t_cmp){ENTER_KEY, key_enter_, 2},
	(t_cmp){INTERRUPT_KEY, key_interrupt, 2},
	(t_cmp){CLEAR_KEY, key_clear_, 2},
	(t_cmp){HOME_KEY, home_key, 4},
	(t_cmp){END_KEY, end_key, 4},
	(t_cmp){DEL_KEY, del_key, 5},
	(t_cmp){PAGE_UP, del_key, 5},
	(t_cmp){PAGE_DOWN, del_key, 5},
	(t_cmp){ARROW_DOWN, arrow_down, 4},
	(t_cmp){ARROW_LEFT, arrow_left, 4},
	(t_cmp){ARROW_RIGHT, arrow_right, 4},
	(t_cmp){ARROW_UP, arrow_up, 4},
	(t_cmp){END_KEY, end_key, 4},
	(t_cmp){HOME_KEY, home_key, 4},
	(t_cmp){SHIFT_UP_KEY, shift_up_key, 7},
	(t_cmp){SHIFT_DOWN_KEY, shift_down_key, 7},
	(t_cmp){SHIFT_RIGHT_KEY, shift_right_key, 7},
	(t_cmp){SHIFT_LEFT_KEY, shift_left_key, 7},
	(t_cmp){NULL, NULL, 0}
};

int				init_buff(char *buff, int *i)
{
	ft_bzero(buff, LEN_BUFFER);
	*i = -1;
	return (1);
}

t_read					*init_struct_for_read(void)
{
	t_read		*read_std;

	if (!(read_std = (t_read *)ft_memalloc(sizeof(t_read))))
		return (NULL);
	if (!(read_std->cmd = create_element('\0')))
		return (NULL);
	read_std->cmd->s = 1;
	read_std->cur.co = tgetnum("co");
	return (read_std);
}

t_read			*read_stdin(void)
{
	char		buff[LEN_BUFFER];
	t_read		*read_std;
	int			c;
	int			i;
	
	if (!(read_std = init_struct_for_read()))
		return (NULL);
	init_buff(buff, &i);
	set_termios(SET_OUR_TERM);
	while ((c = -1) && read(STDIN_FILENO, &(buff[++i]), sizeof(char)))
	{
		if (PRINT_KEY(buff[0]))
			key_print_(&read_std, buff[0]) && init_buff(buff, &i);
		while (compare_key[++c].key)
			if (!ft_strncmp(compare_key[c].key, buff, compare_key[c].len) && init_buff(buff, &i))
				compare_key[c].function(&read_std);
		(i == 5) && init_buff(buff, &i);
		(!read_std->completion) ? print_struct(read_std) : 0;
	}
	set_termios(SET_OLD_TERM);
	return (read_std);
}
