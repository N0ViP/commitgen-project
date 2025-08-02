# include "getenv.h"

static char **split_word(char *str)
{
	char	**res;
	size_t	i;

	i = 0;
	res = malloc(sizeof(char **) * 3);
	res[2] = NULL;
	while (str[i] != '=')
	{
		i++;
	}
	res[0] = ft_substr(str, 0, i);
	res[1] = ft_substr(str, i + 1, ft_strlen(str));
	return (res);
}

static t_env	*split_env(char *str)
{
	char	**p;
	t_env	*val;

	val = malloc(sizeof(t_env));
	p = split_var(str);
	val->key = p[0];
	val->value = p[1];
	val->next = NULL;
	free(p[0]);
	free(p[1]);
	free(p);
	return (val);
}

t_env	*init_env()
{
	int		i;
	t_env	*val;
	t_env	*tail;
	t_env	*env;

	i = 0;
	if (!environ[0])
	{
		return (NULL);
	}
	env = split_env(environ[i++]);
	tail = env;
	while (environ[i])
	{
		val = split_env(environ[i]);
		tail->next = val;
		tail = val;
		i++;
	}
	return (env);
}
