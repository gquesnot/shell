#include "minishell.h"


int		hist_len(t_hist *hist)
{
	t_hist *tmp;
	int		i;
	i = 0;

	tmp = hist;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}


t_hist		*new_history(char *line , int set)
{
	t_hist *new;

	new = malloc(sizeof(t_hist));
	if (line == NULL)
		new->line = NULL;
	else
		new->line = ft_strdup(line);
	new->pos = set;
	new->next = NULL;
	return (new);

}

void		add_history(t_hist **history, char *line)
{
	t_hist	*tmp;

	tmp = *history;
	if (tmp == NULL)
		*history = new_history(line, 1);
	else 
	{
		*history = new_history(line, tmp->pos + 1);
		(*history)->next = tmp;
	}
}

void		show_history(t_hist *hist)
{
	t_hist *tmp;

	tmp = hist;
	while (tmp)
	{
		ft_putnbr_fd(tmp->pos, 0);
		ft_putchar_fd(' ', 0);
		ft_putendl_fd(tmp->line, 0);
		tmp = tmp->next;
	}
}
