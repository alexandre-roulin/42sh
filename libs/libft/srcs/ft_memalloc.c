/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgraille <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/11 15:42:51 by jgraille          #+#    #+#             */
/*   Updated: 2017/08/22 13:58:20 by aroulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_memalloc(size_t len)
{
	char	*mem;

	mem = malloc(len);
	if (mem == NULL)
		return (NULL);
	while (len-- != 0)
		*(mem + len) = 0;
	return ((void *)mem);
}
