/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memdel_jobs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelazou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/19 11:55:12 by sbelazou          #+#    #+#             */
/*   Updated: 2017/11/20 18:38:27 by jle-quel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sh.h>

static void		reset_process(t_jobs **jobs)
{
	t_process	*process;

	while ((*jobs)->process)
	{
		process = (*jobs)->process;
		(*jobs)->process = (*jobs)->process->next;
		ft_memdel((void **)&process->command);
		ft_memdel((void **)&process);
	}
	ft_memdel((void **)jobs);
}

static void		connect_use(t_jobs *jobs)
{
	if (jobs->next_use)
		jobs->next_use->prev_use = jobs->prev_use;
	if (jobs->prev_use)
		jobs->prev_use->next_use = jobs->next_use;
}

void			memdel_jobs(t_jobs *jobs)
{
	t_jobs		*temp_jobs;

	addr_jobs = jobs_table();
	connect_use(jobs);
	temp_jobs = *addr_jobs;
	if (*addr_jobs == jobs)
	{
		dprintf(fd_log, "[%s] Memdel from addr\n", __FILENAME__);
		dprintf(fd_log, "[%s] memdel [%d]\n", __FILENAME__, temp_jobs == jobs);
		*addr_jobs = (*addr_jobs)->next;
		if ((*addr_jobs) && (*addr_jobs)->prev)
			(*addr_jobs)->prev = NULL;
	}
	else
	{
		while (jobs && temp_jobs && temp_jobs != jobs)
		{
			dprintf(fd_log, "[%s] memdel [%d]\n", __FILENAME__, temp_jobs == jobs);
			temp_jobs = temp_jobs->next;
		}
		if (temp_jobs == jobs)
		{
			if (temp_jobs->next)
				temp_jobs->next->prev = temp_jobs->prev;
			if (temp_jobs->prev)
				temp_jobs->prev->next = temp_jobs->next;
		}
	}
	reset_process(&jobs);
}
