/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs_control.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aroulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/11 19:09:46 by aroulin           #+#    #+#             */
/*   Updated: 2017/10/11 19:09:48 by aroulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sh.h>

t_jobs				*jobs_table(void)
{
	static t_jobs		jobs[MAX_CHILD];

	return (jobs);
}

void			update_jobs(t_process *process)
{
	log_trace("/!\\  [PROCESS LL BE UPDATE] /!\\");

	while (process)
	{
		if (process->pid == process->pgid)
		{
			if (WIFEXITED(process->status))
				var_return(WEXITSTATUS(process->status));
			else if (WIFSIGNALED(process->status))
				var_return(WTERMSIG(process->status) + 128);
			else if (WIFSTOPPED(process->status))
				var_return(WSTOPSIG(process->status) + 128);
			else if (WIFCONTINUED(process->status))
				;
		}
		process = process->next;
	}
}

int				update_status(t_process *process)
{
	int				ret;

	ret = 0;
	while (process)
	{
		if ((waitpid(process->pid, &process->status, WCONTINUED | WNOHANG | WUNTRACED)) > 0)
			ret = 1;
		log_fatal("%d %d",process->pid, process->status);
		process = process->next;
	}
	log_trace("Return update_status %d", ret);
	return (ret);
}

int					terminate_process(t_process *process)
{
	while (process)
	{
		log_trace("In terminated process for %d [%d.%d]", process->pid, WIFSIGNALED(process->status), WIFEXITED(process->status));
		if (!WIFSIGNALED(process->status) && !WIFEXITED(process->status))
			return (0);
		process = process->next;
	}
	log_trace("Return terminated_status %d", 1);
	return (1);
}

void				handler_sigchld(int sig)
{
	t_jobs			*jobs;
	int				index;

	log_trace("/!\\  [SIGCHLD RECEPTION %d] /!\\", sig);
	index = MAX_CHILD -1;
	jobs = jobs_table();
	while (index >= 0)
	{
		if (jobs[index].process && jobs[index].process->foreground == false)
		{
			log_trace("%d UPDATE", jobs[index].process->pid);
			first_process(jobs);
			if (update_status(jobs[index].process))
			{
				if (terminate_process(jobs[index].process))
				{
					print_status(jobs[index].process, jobs->index);
					reset_process(jobs + index);
				}
				else
				{
					modify_runing(jobs[index].process, false);
					modify_foreground(jobs[index].process, false);
				}
			}
			pjt(jobs + index);
		}
		index--;
	}
}
;