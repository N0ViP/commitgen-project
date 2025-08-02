#include "stdfcts.h"

static size_t	ft_words_len(char **arr, size_t delemiter_len)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (*arr)
	{
		j = 0;
		while (arr[j])
		{
			j++;
		}
		i += j;
		arr++;
		if (arr)
		{
			i += delemiter_len;
		}
	}
	return (i);
}

static void	fill_str(char **arr, char *delemiter, char *str)
{
	size_t	i;

	i = 0;
	while (arr[i])
	{
		ft_strcat(str, arr[i]);
		i++;
		if (delemiter && arr[i])
		{
			ft_strcat(str, delemiter);
		}
	}
}

char	*ft_strjoin(char **arr, char *delemiter)
{
	size_t	words_len;
	size_t	delemiter_len;
	char	*str;

	if (!delemiter)
	{
		delemiter_len = 0;
	}
	else
	{
		delemiter_len = ft_strlen(delemiter);
	}
	words_len = ft_words_len(arr, delemiter_len);
	str = malloc(words_len + 1);
	str[0] = '\0';
	fill_str(arr, delemiter, str);
	return (str);
}