# include "stdfcts.h"

int	count_words(const char *s, const char *sep)
{
	int count = 0;
	while (*s)
	{
		while (ft_strchr(sep, *s))
			s++;
		if (*s)
			count++;
		while (*s && !ft_strchr(sep, *s))
			s++;
	}
	return count;
}

static void	cleanup(char **result, int i)
{
	while (i-- > 0)
		free(result[i]);
	free(result);
}

char	**ft_split(const char *s, const char *sep)
{
	if (!s)
		return NULL;
	int word_count = count_words(s, sep);
	char **result = malloc((word_count + 1) * sizeof(char *));
	if (!result)
		return NULL;
	int i = 0;
	while (*s && i < word_count)
	{
		while (ft_strchr(sep, *s))
			s++;
		int word_len = 0;
		while (s[word_len] && !ft_strchr(sep, s[word_len]))
			word_len++;
		result[i] = ft_strndup(s, word_len);
		if (!result[i])
		{
			cleanup(result, i);
			return NULL;
		}
		s += word_len;
		i++;
	}
	result[i] = NULL;
	return result;
}
