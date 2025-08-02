#include "getenv.h"

char	*get_var_value(char *key)
{
	t_env	*env;

	env = ft_getenv(GET_ENV, NULL, NULL);
	while (env)
	{
		if (!ft_strcmp(env->key, key))
		{
			return (env->value);
		}
		env = env->next;
	}
	return (NULL);
}