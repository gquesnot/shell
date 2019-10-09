#include "minishell.h"

char		**tab_dup(char **tabb);

t_chev		*new_chev(void)
{
	t_chev *new;

	new = malloc(sizeof(t_chev));
	new->set = 0;
	new->str_chev = NULL;
	new->str_chev_in = NULL;
	return (new);
}
void		malloc_tab(char ***tabb, int n)
{
	int		i;

	i = 0;
		*tabb = malloc(sizeof(char * ) * (n - 1));
		*tabb = malloc(sizeof(char *) * n);
	while (i < n)
	{
		(*tabb)[i] = NULL;
		i++;
	}
}


t_arg		*new_arg(char **fct, char *command, char token ,t_chev *chev)
{
	t_arg	*new;

	new = malloc(sizeof(t_arg));

	ft_putendl_fd(command, 0);
	if (command)
		new->path = ft_strdup(command);
	else
		new->path = NULL;
	if (fct != NULL)
		new->argv = tab_dup(fct);
	else
		new->argv = NULL;
	if (chev)
	{
		new->chev = malloc(sizeof(t_chev));
		new->chev->str_chev = NULL;
		new->chev->str_chev_in = NULL;
		if (chev->str_chev)
		new->chev->str_chev = ft_strdup(chev->str_chev);
		if (chev->str_chev_in)
			new->chev->str_chev_in = ft_strdup(chev->str_chev_in);
		new->chev->set->in = chev->set->in;
		new->chev->set->out = chev->set->out;
	}
	new->token = token;
	new->pipe = NULL;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void		add_arg(t_arg **arg_lst, char **fct, char *command, char token, t_chev *chev)
{
	t_arg	*tmp;
	t_arg	*new;

	tmp = *arg_lst;
	if (!tmp)
		*arg_lst = new_arg(fct, command, token, chev);
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_arg(fct, command, token, chev);
		new  = tmp->next;
		new->prev = tmp;
	}
}

char		**tab_dup(char **tabb)
{
	char **tmp;
	int		i;

	i = 0;

	while (tabb[i])
		i++;
	tmp = malloc(sizeof(char*) * i + 1);
	i = 0;
	while (tabb[i])
	{
		tmp[i] = ft_strdup(tabb[i]);
		i++;
	}
	tmp[i] = NULL;
	return (tmp);
}

int			nb_delim(char *line, char c)
{
	int		i;
	int		j;

	i = 0;
	j = 1;
	while (line[i])
	{
		if (line[i] == c)
			j++;
		else if (line[i] == '|')
			return (j);
		i++;
	}
	return (j);
}




void		print_tabb(char **tabb);



t_arg		*ft_parser(t_shell *shell, char *line)
{
	char *command;
	char **fct;
	int		i;
	int		j;
	char	tok;
	int		tmp;
	char	*tmp2;
	t_arg	*arg;
	t_chev		*chev;

	i = 0;
	j = 0;
	tmp = 0;
	arg = NULL;
	fct = NULL;
	chev = NULL;
	chev = 0;
	tok = 0;
	tmp2 = ft_strnew(50);
	tmp2 = getcwd(tmp2, 50);
	if (shell->dir_bin)
		j = 0;
	while (line[tmp + i] != '\0')
	{
		if (line[tmp + i] == ' ' || line[tmp + i + 1] == '\0')
		{
			if (line[tmp + i + 1] == '\0')
				i++;
			if (fct == NULL && line[tmp + i] != '\0')
				malloc_tab(&fct, nb_delim(line + tmp + i, ' ') + 1);
			else if (fct == NULL && line[tmp + i] == '\0')
				malloc_tab(&fct, nb_delim(line + tmp + i, ' '));
			if (chev && (chev->set == 1 || chev->set == 2 || chev->set == 5 || chev->set == 6))
				chev->str_chev = ft_strsub(line, tmp, i);
			else if (chev && chev->set >= 4)
				chev->str_chev_in = ft_strsub(line, tmp, i);
			else
				fct[j] = ft_strsub(line, tmp, i);
			tmp = tmp + i + 1;
 		j++;
			i = 0;
			if (line[tmp - 1] == '\0')
			{
			fct[j] = NULL;
			break;
			}
		}
		if (line[tmp + i] == '|')
		{
			if (fct[0][0] == '.')
				command = find_access_char(tmp2, fct[0]);
			else
				command = find_access(shell->dir_bin, fct[0]);
			fct[j] = NULL;
			if (!arg)
				add_arg(&arg, NULL, NULL, '|', chev);
			if (arg->token != '|')
				add_arg(&arg, NULL, NULL, '|', chev);
			add_arg(&(arg->pipe), fct, command, 0, chev);
			fct = NULL;
			chev = NULL;
			tmp = tmp + i + 1;
			if (line[tmp] == ' ')
				tmp++;
			i = 0;
			j = 0;
			command = NULL;
		}
		if (line[tmp + i] == ';' && line[tmp + i + 1] != '\0')
		{
			if (fct[0][0] == '.')
				command = find_access_char(tmp2, fct[0] + 2);
			else
				command = find_access(shell->dir_bin, fct[0]);
			fct[j] = NULL;
			add_arg(&arg, fct, command, ';', chev);
			fct = NULL;
			tmp = tmp + i + 1;
			if (line[tmp] == ' ')
				tmp++;
			i++;
			j = 0;
			i = 0;
			chev = NULL;
			command = NULL;
		}
		if (line[tmp + i] == '>' || (line[tmp + i] == '>' && line[tmp + i + 1] == '>'))
		{
			if (chev == NULL)
				chev = new_chev();
			if (line[tmp + i] == '>')
				chev->set = 1;
			if (line[tmp + i + 1] == '>')
			{
				chev->set = 2;
				i++;
			}
			if (line [tmp + i + 1] == ' ')
				i++;
			tmp = tmp + i + 1;
		}
		if (line[tmp + i] == '<')
		{
			if (chev == NULL)
				chev = new_chev();
			chev->set += 4;
			tmp = tmp + i + 2;
		}
	i++;
	}
	if (i != tmp)
	{
		fct[j] = NULL;
		if (fct[0][0] == '.')
			command = find_access_char(getcwd(tmp2, 50), fct[0]);
		else
			command = find_access(shell->dir_bin, fct[0]);
		if (arg)
		{
			if (arg->token == '|' || tok = '|')
				add_arg(&(arg->pipe), fct, command, 0, chev);
			else
				add_arg(&arg, fct, command, 0, chev);
		}
		else
			add_arg(&arg, fct, command, 0, chev);
		fct = NULL;
	}
	return (arg);
}

void		print_tabb(char **tabb)
{
	int		i;

	i = 0;
	while (tabb[i])
	{
		ft_putendl_fd(tabb[i], 0);
		i++;
	}

}

void		print_arg(t_arg *arg)
{
	t_arg	*tmp;

	if (arg)
	{
		while (arg)
		{
			if (arg->path && arg->argv)
			{
				ft_putendl_fd("main:", 0);
				ft_putendl_fd(arg->path, 0);
				print_tabb(arg->argv);
				if (arg->chev)
				{
					ft_putendl_fd("chev=", 0);
					if (arg->chev->str_chev)
						ft_putendl_fd(arg->chev->str_chev, 0);
					if (arg->chev->str_chev_in)
						ft_putendl_fd(arg->chev->str_chev_in, 0);
				}
			}
			if (arg->pipe)
			{
				ft_putendl_fd("pip:", 0);
				tmp = arg->pipe;
				while (tmp)
				{
					ft_putendl_fd(tmp->path, 0);
					print_tabb(tmp->argv);
					if (tmp->chev)
					{
						ft_putendl_fd("chev=", 0);
						if (tmp->chev->str_chev)
							ft_putendl_fd(tmp->chev->str_chev, 0);
						if (tmp->chev->str_chev_in)
							ft_putendl_fd(tmp->chev->str_chev_in, 0);
					}
					tmp = tmp->next;
				}
			}
			arg = arg->next;
		}
	}



}


