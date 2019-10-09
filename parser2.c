#include "minishell.h"

char		*get_path(void)
{
	char *tmp;

	tmp = ft_strnew(50);
	tmp = getcwd(tmp, 50);
	return (tmp);

}


t_chev		*new_chev()
{
	t_chev *new;

	new = malloc(sizeof(t_chev));
	new->str_chev_in = NULL;
	new->str_chev_out = NULL;
	new->str_chev_err = NULL;
	new->set.in = 0;
	new->set.out = 0;
	new->set.err = 0;
	return (new);
}


void		reset_arg(t_arg **arg)
{
	t_arg *tmp;

	tmp = *arg;
	while (tmp)
	{
		*arg = tmp->next;
		tmp = tmp->next;
		free(tmp->chev);
		free(tmp->pipe);
		free(tmp->argv);
		tmp = *arg;
	}
	free(arg);
	arg = NULL;
}

int			nb_delim(char *line)
{
	int		i;
	int		j;

	i = 0;
	j = 1;
	while (line[i])
	{
		if (line[i] == '|' || line[i] == ';' || line[i] == '<' || line[i] == '>')
		{
			if (line[i - 1] == ' ')
				return (j);
			else
				return (j + 1);
		}
		else if (line[i] == ' ' && line[i + 1] == '\0')
			return (j + 1);
		else if (line[i] == ' ')
			j++;
		i++;
	}
	return (j + 1);
}

char		**malloc_tab(int n)
{
	char **new;
	int		i;

	i = 0;
	new = malloc(sizeof(char*) * n);
	while (i < n)
	{
		new[i] = NULL;
		i++;
	}
	return (new);
}

char	**tab_dup(char **tabb)
{
	char	**tmp;
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


t_arg		*new_arg(char **fct, char *command, char token , t_chev *chev)
{
	t_arg*new;

	new = malloc(sizeof(t_arg));

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
		new->chev->str_chev_out = NULL;
		new->chev->str_chev_in = NULL;
		new->chev->str_chev_err = NULL;
		if (chev->str_chev_out)
			new->chev->str_chev_out = ft_strdup(chev->str_chev_out);
		if (chev->str_chev_in)
			new->chev->str_chev_in = ft_strdup(chev->str_chev_in);
		if (chev->str_chev_err)
			new->chev->str_chev_err = ft_strdup(chev->str_chev_err);
		new->chev->set.in = chev->set.in;
		new->chev->set.out = chev->set.out;
		new->chev->set.err = chev->set.err;
	}
	new->token = token;
	new->pipe = NULL;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}


void		add_arg(t_arg **arg_lst, char **fct, char *command, char token, t_chev *chev)
{
	t_arg*tmp;
	t_arg*new;

	tmp = *arg_lst;
	if (!		tmp)
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


void		add_chev(t_chev *chev , char *line, int start, int i)
{
	if ((chev->set.err ==  1 || chev->set.err == 3) && chev->str_chev_err == NULL)
		chev->str_chev_err = ft_strsub(line, start, i);
	if (chev->set.in != 0 && chev->str_chev_in == NULL)
		chev->str_chev_in = ft_strsub(line, start, i);
	if (chev->set.out != 0 && chev->str_chev_out == NULL)
		chev->str_chev_out = ft_strsub(line, start, i);
}

char		*find_command_access(char *pwd, t_list **tabb, char *cmd)
{
	if (cmd[0] == '.' && pwd != NULL)
		return (find_access_char(pwd, cmd));
	else
		return (find_access(tabb, cmd));
}

void		reset_p(t_chev **chev, char ***fct, char **line)
{

	free(*line);
	*chev = NULL;
	*line = NULL;
	*fct = NULL;
}

void		add_arg_tmp(t_arg **arg, t_arg **new)
{
	t_arg *tmp;

	tmp = *arg;
	if (!tmp)
		*arg = *new;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = *new;
	}
	*new = NULL;
}

int		fill_chev(t_chev *chev, char *line, int start, int i)
{
	int		j;

	j = start + i;
	if (line[j] == '>')
	{
		if (line[j + 1] == ' ' && line[j - 1] == ' ')
			chev->set.out = 1;
		else if (line[j + 1] == '>' && line[j - 1] == ' ')
		{
			chev->set.out = 2;
			j += 1;
		}
		else if (line[j - 1] == '2')
		{
			if (line[j + 1] == ' ')
				chev->set.err = 1;
			else if (line[j + 1] == '>')
			{
				chev->set.err = 3;
				j += 1;
			}
			else if (line[j + 1] == '&' && line[j + 2] == '1')
			{
				chev->set.err = 2;
				j += 1;
			}
		}
		j += 1;
	}
	if (line[j] == '<')
	{
		if (line[j + 1] == ' ')
		{
			chev->set.in = 1;
		}
		if (line[j + 1] == '<')
		{
			if (line[j + 1] == '-')
			{
				chev->set.in = 3;
				j += 1;
			}
			else
				chev->set.in = 2;
			j += 1;

		}
		j += 1;
	}
	return (j + 1);
}

int			end_of_quote(char *str, int start)
{
	int		i;
	char	c;

	i = 0;
	c = str[start + i - 1];
	while (str[start + i])
	{
		if ((str[start + i] == 34 && c == 34) || (str[start +  i] == 39 && c == 39))
			return (i);
		i++;
	}
	return (0);
}

void		clean_space(t_shell *shell)
{
	t_command	*tmp;
	t_command	*del;

	tmp = shell->cmd;

	while (tmp && tmp->next)
	{
		if (tmp->c == ' ' && tmp->next->c == ' ')
		{
			del = tmp->next;
			tmp->next = tmp->next->next;
			tmp->next->prev = tmp;
			free(del);
			tmp = tmp->prev;
		}
		tmp = tmp->next;
	}
}

void		add_space(t_command *cmd)
{
	t_command *tmp;

	tmp = cmd->next;
	cmd->next = cmd_new(' ', 0);
	cmd->next->next = tmp;
	tmp->prev = cmd->next->next;
}


void			space(t_shell *shell)
{
	t_command	*tmp;
	t_command	*del;

	clean_space(shell);
	tmp = shell->cmd;
	//ft_putchar_fd('\n', 0);
	while (tmp)
	{
	//	ft_putnbr_fd(tmp->c, 0);
	//	ft_putchar_fd(' ', 0);
		if (tmp->c == '|' || tmp->c == ';')
			{
				if (tmp->prev && tmp->prev->c != ' ')
					add_space(tmp->prev);
				if (tmp->next && tmp->next->c != ' ')
					add_space(tmp);
				if (tmp->next && tmp->next->c == ' ')
				{
					if (tmp->next->next && tmp->next->next->c == 0)
					{
						del = tmp->next;
						tmp->next = tmp->next->next;
						tmp->next->prev = tmp;
						free (del);
					}
				}
 		}
		if (tmp-> c == ' ' && !tmp->next)
		{
			del = tmp;
			tmp = tmp->prev;
			tmp->next = NULL;
			free(del);
		}
		if (tmp->c == ' ' && tmp->next && tmp->next->c == 0)
		{
			del = tmp;
			tmp = tmp->prev;
			tmp->next = tmp->next->next;
			tmp->next->prev = tmp;
			free(del);
		}

		tmp = tmp->next;
	}
}



t_arg		*ft_parser2(t_shell *shell, char *line)
{
	char *command;
	char **fct;
	t_chev *chev;
	t_arg *arg;
	t_arg *tmp;
	char	*path;
	int		i;
	int		j;
	int		start;

	i = 0;
	j = 0;
	start = 0;
	arg = NULL;
	fct = NULL;
	chev = NULL;
	tmp = NULL;
	shell->cmd = add_line_to_cmd(line);

	space(shell);

	line = add_cmd_to_shell(shell->cmd);
	path = get_path();
while (line[start + i])
	{
		if (line[start + i - 1] == 34 || line[start + i - 1] == 39)
			i += end_of_quote(line, start + i);
		if ((line[start + i] == ' ' || (line[start + i + 1] == '\0')) && line[start + i] != ';')
		{
			if (line[start + i + 1] == '\0')
				i++;
			if (fct == NULL)
				fct = malloc_tab(nb_delim(line + start + i));
			if (chev)
				add_chev(chev, line, start, i);
			else if (line[start + i - 1] == 34 || line[start + i - 1] == 39)
				fct[j] = ft_strsub(line, start, i);
			else
				fct[j] = ft_strsub(line, start, i);
			if (line[start + i] != '\0')
			{
				if (line[start + i + 1] == '\0')
				{
					fct[j + 1] = NULL;
					j += 1;
					start = start + i + 1;
				}
				else
					start = start + i + 1;
				j++;
				i = 0;
			}
			else
			{
				start = start + 1 + i;
				i = 0;
				j++;
			}
		}
		if (line[start + i] == '>' || line[start + i] == '<')
		{
			if (chev == NULL)
				chev = new_chev();
			start = fill_chev(chev, line, start ,i);
			i = 0;
		}
		if (line[start + i] == '|')
		{
			command = find_command_access(path, shell->bin_hash, fct[0]);
			if (!tmp)
				add_arg(&tmp, NULL, NULL, '|', NULL);
			add_arg(&(tmp->pipe), fct, command, 0, chev);
			fct = NULL;
			if (line[start + i + 1] == ' ')
				start = start + i + 2;
			else
				start = start + i + 1;
			reset_p(&chev, &fct, &command);
			fct = NULL;
			i = 0;
			j = 0;
		}
		if (line[start + i] == ';' || line[start + i] == '\0')
		{
			command = find_command_access(path, shell->bin_hash , fct[0]);
			if (!tmp)
				add_arg(&arg, fct, command, ';', chev);
			else
			{
				add_arg(&(tmp->pipe), fct, command , '|', chev);
				add_arg_tmp(&arg, &tmp);
			}
			reset_p(&chev, &fct, &command);
			if (line[start + i] == '\0' || line[start + i + 1] == '\0')
				break;
			else if (line[start + i + 1] == ' ')
				start = start + i + 2;
			else
				start = start + i + 1;
			i = 0;
			j = 0;
			fct = NULL;
		}
		i++;
	}
	return (arg);
}

void	print_tabb(char **tabb)
{
	int		i;

	i = 0;
	while (tabb[i])
	{
		ft_putendl_fd(tabb[i], 0);
		i++;
	}

}


void	print_arg(t_arg *arg)
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
					if (arg->chev->set.err == 1)
						ft_putendl_fd(arg->chev->str_chev_err, 0);
					else if (arg->chev->set.err == 2)
						ft_putendl_fd("err_in_out", 0);
					if (arg->chev->str_chev_out)
						ft_putendl_fd(arg->chev->str_chev_out, 0);
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
						if (tmp->chev->set.err == 1)
							ft_putendl_fd(tmp->chev->str_chev_err, 0);
						else if (tmp->chev->set.err == 2)
							ft_putendl_fd("err_in_out", 0);
						if (tmp->chev->str_chev_out)
							ft_putendl_fd(tmp->chev->str_chev_out, 0);
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


