#include "stdfcts.h"

bool	ft_isalnum(int a)
{
	return (ft_isalpha(a) || ft_isdigit(a));
}