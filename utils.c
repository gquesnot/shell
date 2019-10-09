#include "minishell.h"

t_shell *ft_stock(t_shell **shell, int i)
{
	static t_shell *tmp;

	tmp = NULL;
	if (i == 0)
		tmp = *shell;
	return (tmp);
}

char		*delete_char(char *str, int pos)
{
	int		i;

	while (str[i + 1])
	{
		if (i >= pos)
		{
			str[i] = str[i + 1];
		}
		i++;
	}
	str[i] = '\0';
	return (str);
}

