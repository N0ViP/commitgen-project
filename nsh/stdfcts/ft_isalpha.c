#include "stdfcts.h"

bool	ft_isalpha(int a)
{
	return ((a >= 'A' && a <= 'Z') || (a >= 'a' && a <= 'z'));
}