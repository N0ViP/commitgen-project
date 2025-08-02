/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 08:29:06 by yjaafar           #+#    #+#             */
/*   Updated: 2025/08/02 19:40:08 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <sys/time.h>
# include <stddef.h>

typedef struct s_stuff
{
	pthread_t		*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	lock;
	struct timeval	tv_start;
	int				number_of_philos;
	int				t_to_die;
	int				t_to_eat;
	int				t_to_think;
	int				t_to_sleep;
	int				must_eat;
}	t_stuff;

typedef struct t_philo
{
	pthread_mutex_t	eat_protection;
	pthread_mutex_t	alive_protection;
	pthread_mutex_t	time_protection;
	struct timeval	tv_beg;
	t_stuff			*stuff;
	int				first_fork;
	int				second_fork;
	int				eat;
	bool			alive;
}	t_philo;

int			ft_abs(int x);
int			ft_atoi(char *s);
void		*monitoring(void *arg);
bool		is_alive(t_philo *philo);
bool		one_philo(t_stuff *stuff);
void		*run_simulation(void *arg);
void		take_forks(t_philo *philo);
long long	time_ms(struct timeval *tv);
void		print(t_philo *philo, char *str);
void		put_fork(t_philo *philo, int fork);
bool		init_mutex(pthread_mutex_t *mtx[4]);
void		take_fork(t_philo *philo, int fork);
bool		init_philo(t_philo *philos, t_stuff *stuff);
void		join_philos(t_philo *philos, int n_of_philos);
void		kill_philos(t_philo *philos, int n_of_philos);
void		destroy_mutex(t_philo *philos, int n_of_philos);
bool		init_each_philo(t_philo *philo, t_stuff *stuff, int i);

#endif
