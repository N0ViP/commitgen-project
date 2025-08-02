# include "stdfcts.h"

char	*ft_strndup(const char *s, size_t n)
{
	char *dup = malloc(n + 1);
	size_t i = 0;
	while (i < n)
	{
		dup[i] = s[i];
		i++;
	}
	dup[n] = '\0';
	return dup;
}
