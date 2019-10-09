#include "minishell.h"




t_lst *lstnew(char *s, int pos)
{
	t_lst *new;

	if (!(new = malloc(sizeof(t_lst))))
		return (NULL);
	new->pos = pos;
	new->name = ft_strdup(s);
	new->len = ft_strlen(s);
	new->pos = 0;
	new->select = 0;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}


void		lst_add(t_lst **lst, char *s)
{
	t_lst	*tmp;
	int		i;

	i = 1;
	if (!(*lst))
		*lst = lstnew(s, i);
	else
	{
		i++;
		tmp = *lst;
		while (tmp->next)
		{
			i++;
			tmp = tmp->next;
		}
		tmp->next = lstnew(s, i);
		tmp->next->prev = tmp;
	}
}


void			maxsize_nbelem(t_shell *shell, t_lst *lst)
{
	t_lst *tmp;

	tmp = lst;
	while (tmp->next)
	{
		if (shell->size->maxsize < tmp->len)
			shell->size->maxsize = tmp->len;
		shell->size->nb_elem += 1;
		tmp = tmp->next;
	}

}


int		take_winsize_2(t_shell *shell, t_lst *lst)
{
	ioctl(0, TIOCGWINSZ,&(shell->winsize));
	if (!shell->size)
		shell->size = malloc(sizeof(t_size));
	shell->size->maxsize = 0;
	shell->size->nb_elem = 0;
	shell->size->nb_row = 0;
	shell->size->nb_col = 0;
	maxsize_nbelem(shell, lst);
	shell->size->nb_col = shell->winsize.ws_row / shell->size->maxsize;
	if (shell->size && shell->winsize.ws_row > shell->size->nb_elem && shell->winsize.ws_col > shell->size->maxsize)
	{
		return (1);
	}
	else
		shell->size->nb_col = shell->winsize.ws_col / (shell->size->maxsize + 2);
	if (shell->size->nb_col * shell->winsize.ws_row < shell->size->nb_elem)
		return (0);
	return (1);
}




void		ft_putstr_space(char *str, int nb)
{
	int		i;

	i = 0;
	ft_putchar_fd('[', 0);
	ft_putstr_fd(str, 0);
	while (i < nb)
	{
		ft_putchar_fd(' ', 0);
		i++;
	}
	ft_putchar_fd(']', 0);
}


void		printlst(t_shell *shell, t_lst *lst)
{
	t_lst	*tmp;
	int		col;

	col = 0;
	tmp = lst;
	if (take_winsize_2(shell, lst) == 0)
	{
		ft_putendl_fd("Screen to small", 0);
		shell->size->size_ok = 0;
	}
	else
	{

		printf("%d;%d;%d;%d;%d\n", shell->size->maxsize, shell->size->nb_col, shell->size->nb_row,shell->winsize.ws_row, shell->winsize.ws_col);
		shell->size->size_ok = 1;
		while (tmp)
		{
			ft_putstr_fd(GREEN, 0);
			if (col == shell->size->nb_col)
			{
				col = 0;
				ft_putchar_fd('\n', 0);
			}
			if (tmp->select == 1 && tmp->pos == 1)
			{
				ft_putstr_fd(HIGHLIGHT, 0);
				ft_putstr_fd(UNDERLINE, 0);
			}
			else if (tmp->select == 1)
				ft_putstr_fd(HIGHLIGHT, 0);
			else if (tmp->pos == 1)
				ft_putstr_fd(UNDERLINE, 0);
			ft_putstr_space(tmp->name, shell->size->maxsize - tmp->len);
			ft_putstr_fd(RESET, 0);
			col++;
			tmp = tmp->next;
		}
	}
}

