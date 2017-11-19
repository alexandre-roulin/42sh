/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_wait.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jle-quel <jle-quel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/17 16:40:09 by jle-quel          #+#    #+#             */
/*   Updated: 2017/11/19 01:31:45 by jle-quel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sh.h>

t_process		*place_status(pid_t pid, int status)
{
	t_process		*process;

	if ((process = get_process(pid)) != NULL)
		process->status = status;
	return (process);
}

int				wait_group(t_process *process, int option)
{
	pid_t		pid;
	int			status;

	while (process)
	{
		if ((pid = waitpid(-process->pgid, &status, option)) > 0)
			place_status(pid, status);
		process = process->next;
	}
	return (0);
}

void			set_fildes(pid_t pgid)
{
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	tcsetpgrp(STDIN_FILENO, pgid);
}

void			my_wait(t_jobs *jobs)
{
	log_fatal("Adress of {jobs} in Wait [%p]", jobs);
	close_fildes(jobs->process);
	if (jobs->process->foreground)
	{
		set_fildes(jobs->process->pgid);
		wait_group(jobs->process, WUNTRACED);
		set_fildes(getpgid(0));
	}
	else
	{
		print_info_jobs(jobs->process, jobs->index);
	}

}
