#include "utils.h"

void	free_list(t_list_info **list_info, void (*free_content)())
{
	t_list	*node;

	while ((*list_info)->list)
	{
		node = (*list_info)->list;
		(*list_info)->list = (*list_info)->list->next;
		free_content(node);
		free(node);
	}
	free(*list_info);
	*list_info = NULL;
}

void    _free(char **str)
{
	char	**first;

	first = str;
	while (*str)
	{
		free(*str);
		str++;
	}
	free(first);
}
