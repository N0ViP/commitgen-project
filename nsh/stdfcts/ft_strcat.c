#include "stdfcts.h"

char	*ft_strcat(char *dest, char *src)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (dest[i])
	{
		i++;
	}
	while (src[j])
	{
		dest[i++] = src[j++];
	}
	dest[i] = '\0';
	return (dest);
}