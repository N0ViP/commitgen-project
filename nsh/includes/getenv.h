#ifndef FT_GETENV_H
# define FT_GETENV_H

# define GET_ENV			1
# define ADD_IN_ENV			2
# define REMOVE_FROM_ENV	3

#include <stddef.h>
#include "stdfcts.h"

extern char	**environ;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

t_env	*init_env();
char	*get_var_value(char *key);
void	remove_from_env(t_env **env, char *key);
t_env	*ft_getenv(int flag, char *key, char *value);
void	add_in_env(t_env **env, char *key, char *value);

#endif
