#include "minishell.h"

void			key_right_comp(t_lst *lst)
{
	t_lst *tmp;
	
	tmp = lst;
	while (tmp)
	{
		if (tmp->pos == 1)
			{
				tmp->pos = 0;
				if (tmp->next != NULL)
					tmp->next->pos = 1;
				else
					lst->pos = 1;
				return ;
			}
		tmp = tmp->next;
	}
}

void		key_left_comp(t_lst *lst)
{
	t_lst *tmp;
	
	tmp = lst;
	if (tmp->pos == 1)
	{
		tmp->pos = 0;
		while (tmp->next)
			tmp = tmp->next;
		tmp->pos = 1;
		return ;
	}
	while (tmp->next)
	{
		if (tmp->next->pos == 1)
		{
			tmp->next->pos = 0;
			tmp->pos = 1;
			return ;
		}
		tmp = tmp->next;
	}
}

void			key_up_comp(t_lst *lst, t_size size)
{
	t_lst *tmp;
	t_lst *tmp2;
	int		i;
	int		j;

	i = 0;
	j = 1;
	tmp = lst;
	tmp2 = tmp;
	while (tmp->pos != 1 && tmp)
		{
		tmp = tmp->next;
		j++;
		}
	tmp->pos = 0;
	if (j <= size.nb_col)
		{
				if (j == 1)
					 if (size.nb_elem % size.nb_col == 0 || size.nb_col == 1)
						j = size.nb_elem - 1;
					else
						j = size.nb_elem - (size.nb_elem % size.nb_col)- 1;
				else if (size.nb_elem % size.nb_col >= j - 1)
					j = size.nb_elem - (size.nb_elem % size.nb_col) + j - 2;
				else
					j = size.nb_elem - (size.nb_elem % size.nb_col) - size.nb_col + j - 2;
				while (tmp2 && i < j)
			{
				tmp2 = tmp2->next;
				i++;
			}
		tmp2->pos = 1;
		}
 else
	{
		while (tmp)
		{
		if (i == size.nb_col)
		{
			tmp->pos = 1;
			return ;
		}
		tmp = tmp->prev;
		i++;
		}
	}
}

void			key_down_comp(t_lst *lst, t_size size)
{
	t_lst		*tmp;
	t_lst		*tmp2;
	int			i;
	int			j;

	i = 0;
	j = 1;

	tmp = lst;
	tmp2 =tmp;
	while (tmp->pos != 1 && tmp)
		{
			tmp = tmp->next;
			j++;
		}
	tmp->pos = 0;
	if (j > size.nb_elem - size.nb_col)
	{
		if (j == size.nb_elem - (size.nb_elem % size.nb_col) || j % size.nb_col == size.nb_col)
			j = 0;
		else
		{
			j = (j % size.nb_col);
		}
		while (tmp2 && i < j)
		{
			tmp2 = tmp2->next;
			i++;
		}
		tmp2->pos = 1;

	}
	else
	{
	while (tmp)
		{
		if (i == size.nb_col)
			tmp->pos = 1;
		tmp = tmp->next;
		i++;
		}
	}

}




void		key_space_comp(t_lst *lst)
{
	t_lst *tmp;

	tmp = lst;
	while (tmp->pos != 1)
		tmp = tmp->next;
	if (tmp->select == 1)
		tmp->select = 0;
	else if (tmp->select == 0)
		tmp->select = 1;
	tmp->pos = 0;
	if (tmp->next)
		tmp->next->pos = 1;
	else
		lst->pos = 1;
}


void		remove_elem(t_lst **lst)
{
	t_lst *tmp;

	tmp = *lst;
	(*lst) = tmp->next;
	(*lst)->pos = 1;
	tmp = *lst;
	(*lst)->prev = NULL;
}


void		key_back_comp(t_lst *lst)
{
	t_lst *tmp;
	t_lst *tmp2;

	tmp = lst;
	while (tmp->pos != 1 && tmp)
		tmp = tmp->next;
	tmp->pos = 0;
	if (tmp->next && tmp->prev)
	{
		tmp->next->pos = 1;
		tmp2 = tmp->prev;
		tmp2->next = tmp->next;
		tmp2->next->prev = tmp2;
	
	}
	else if (tmp->next == NULL && tmp->prev)
	{
		tmp->prev->next = NULL;
		lst->pos = 1;
	}
	else if (tmp->prev == NULL && tmp->next)
		{
			remove_elem(&(lst));
		}
	else
		return;
}


void		key_echap_comp()
{
	exit(1);
}


