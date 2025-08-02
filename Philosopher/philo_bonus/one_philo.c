/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_philo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:21:00 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/31 17:05:33 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	one_philo(int t_to_die)
{
	pid_t	pid;
	sem_t	*sem;
	int		ex_status;

	sem_unlink("/forks");
	sem = sem_open("/forks", O_CREAT, 0777, 1);
	pid = fork();
	if (pid == 0)
	{
		printf("%d\t%d\tis thinking\n", 1, 0);
		sem_wait(sem);
		printf("%d\t%d\thas taken a fork\n", 1, 0);
		usleep(1000 * t_to_die);
		printf("%d\t%d\tdied\n", 1, t_to_die);
		sem_close(sem);
	}
	else
	{
		waitpid(pid, &ex_status, 0);
		sem_close(sem);
		sem_unlink("/forks");
	}
	exit(EXIT_SUCCESS);
}
