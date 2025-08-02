#include "getenv.h"

static bool	check_modify_key(t_env *ptr, char *key, char *value)
{
	if (!ft_strcmp(ptr->key, key))
	{
		free(ptr->value);
		free(key);
		ptr->value = value;
		return (true);
	}
	return (false);
}

static t_env	*add_key_value(char *key, char *value)
{
	t_env	*tmp;

	tmp = malloc(sizeof(t_env));
	tmp->key = key;
	tmp->value = value;
	tmp->next = NULL;
	return (tmp);
}

void	add_in_env(t_env **env, char *key, char *value)
{
	t_env	*ptr;

	if (!*env)
	{
		*env = add_key_value(key, value);
		return ;
	}
	ptr = *env;
	if (check_modify_key(ptr, key, value))
	{
		return ;
	}
	while (ptr->next)
	{
		if (check_modify_key(ptr, key, value))
		{
			return ;
		}
		ptr = ptr->next;
	}
	if (!ptr->next)
	{
		ptr->next = add_key_value(key, value);
	}
}
