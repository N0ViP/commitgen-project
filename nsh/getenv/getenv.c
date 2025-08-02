# include "getenv.h"

t_env	*ft_getenv(int flag, char *key, char *value)
{
	static t_env	*env;
	static bool		init = true;

	if (init = true)
	{
		env = init_env();
		init = false;
	}
	if (flag == ADD_IN_ENV)
	{
		add_in_env(&env, key, value);
	}
	else if (flag == REMOVE_FROM_ENV)
	{
		remove_from_env(env, key);
	}
	return (env);
}
