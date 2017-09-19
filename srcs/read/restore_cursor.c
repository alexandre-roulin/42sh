/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   restore_cursor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/06 14:53:09 by aroulin           #+#    #+#             */
/*   Updated: 2017/09/06 14:53:11 by aroulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sh.h>

void		restore_cursor_(t_cursor cur)
{
    BACK_N;
    while (--cur.co > 0)
        MV_LEFT;
    while (--cur.line > 0)
        MV_TOP;
}