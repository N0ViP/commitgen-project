/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:21:19 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/31 17:01:16 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <fcntl.h>
# include <stdio.h>
# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <semaphore.h>

typedef struct s_stuff
{
	struct timeval	tv_start;
	struct timeval	tv_beg;
	sem_t			*forks;
	sem_t			*alive_protection;
	sem_t			*time_protection;
	sem_t			*eat_protection;
	sem_t			*lock;
	pid_t			*philos;
	char			*alive_protection_name;
	char			*time_protection_name;
	char			*eat_protection_name;
	bool			alive;
	int				n_eat;
	int				philo_id;
	int				number_of_philos;
	int				t_to_die;
	int				t_to_eat;
	int				t_to_think;
	int				t_to_sleep;
	int				must_eat;
}	t_stuff;

long long	time_ms(struct timeval *tv);
int			ft_abs(int x);
bool		is_alive(t_stuff *stuff);
void		clean_up(t_stuff *stuff);
void		clean_sems(t_stuff *stuff);
void		one_philo(int t_to_die);
void		kill_philos(t_stuff *stuff, int n_of_philos);
void		allocate_philos_forks(t_stuff *stuff);
void		run_philos(t_stuff *stuff);
void		philo_died(t_stuff *stuff, int pid);
void		wait_child(t_stuff *stuff);
void		thinking(t_stuff *stuff);
void		sleeping(t_stuff *stuff);
void		take_fork(t_stuff *stuff);
void		take_forks(t_stuff *stuff);
void		put_forks(t_stuff *stuff);
void		eating(t_stuff *stuff);
void		*start(void *arg);
void		check_alive(t_stuff *stuff, pthread_t philo);
void		check_eat(t_stuff *stuff, pthread_t philo);
void		monitor(t_stuff *stuff, pthread_t philo);
void		open_semaphores(t_stuff *stuff);
void		init_semaphores(t_stuff *stuff);
void		run_simulation(t_stuff *stuff);
void		init_philos(t_stuff *stuff);
bool		init_stuff(t_stuff *stuff, int ac, char *av[]);
char		*ft_strjoin(char *s1, char *s2);
int			ft_strlen(char *s);
int			ft_numlen(int n);
char		*ft_itoa(int n);
void		print(t_stuff *stuff, char *str);
int			ft_atoi(char *s);
void		jon_philo(t_stuff *stuff, pthread_t philo);

#endif
