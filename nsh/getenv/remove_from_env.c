# include "getenv.h"

static void	free_var_node(t_env *ptr)
{
	free(ptr->key);
	free(ptr->value);
	free(ptr);
}

void	remove_from_env(t_env **env, char *key)
{
	t_env	*ptr;
	t_env	*tmp;

	if (!*env)
		return ;
	ptr = *env;
	if (!ft_strcmp(ptr->key, key))
	{
		*env = ptr->next;
		free_var_node(ptr);
		return ;
	}
	while (ptr->next)
	{
		if (!ft_strcmp(ptr->next->key, key))
		{
			tmp = ptr->next;
			ptr->next = ptr->next->next;
			free_var_node(tmp);
			return ;
		}
		ptr = ptr->next;
	}
}
