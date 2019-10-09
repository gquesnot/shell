#include "minishell.h"

int			start_match(char *s, char *s1);
int			end_match(char *s, char *s2);


void		delete(void *content, size_t content_size)
{
	free(content);
	(void)content_size;
}



int		double_match(char *s, char *s1, char *s2)
{
	if (start_match(s, s1) == 1 && end_match(s, s2) == 1)
		return (1);
	else
		return (0);
}

int			char_match(char a, char b)
{
	if (a == b)
		return (1);
	else if (ft_islower(a) && ft_islower(b))
		return (0);
	else if (ft_isupper(a) && ft_isupper(b))
		return (0);
	else if (ft_islower(a) && b == a - 32)
		return (1);
	else if (ft_isupper(a) && b == a + 32)
		return (1);
	else
		return (0);
}


int			start_match(char *s, char *s1)
{
	int		i;

	i = 0;
	while (s[i] && s1[i])
	{
		if (char_match(s[i], s1[i]) == 0)
			return (0);
		i++;
	}
	if (s1[i] == '\0')
		return (1);
	else
		return (0);
}

int			end_match(char *s, char *s2)
{
	int		i;
	int		j;

	i = 0;
	j = ft_strlen(s) - ft_strlen(s2);
	if (s[ft_strlen(s) - 1] == '/' && s2[ft_strlen(s2) - 1] != '/')
		j--;
	while (s[j] && s2[i])
	{
		if (char_match(s[j], s2[i]) == 0)
			return (0);
		i++;
		j++;
	}
	if (s2[i] == '\0')
		return (1);
	else
		return (0);
}



char		*ncmd_before(t_command *cmd)
{
	t_command	*tmp;
	int		i;
	char	*new;

	i = 0;
	tmp = cmd;
	while (tmp && tmp->c != ' ')
	{
		i++;
		tmp = tmp->prev;
	}
	i--;
	new = malloc(sizeof(char) * (i));
	new[i] = '\0';
	i--;
	cmd = cmd->prev;
	while (cmd && i >= 0)
	{
		new[i] = cmd->c;
		i--;
		cmd = cmd->prev;
	}
	return (new);
}

char		*ncmd_after(t_command *cmd)
{
	t_command	*tmp;
	int			i;
	int			j;
	char		*new;

	i = 0;
	j = 0;
	tmp = cmd;
	while (tmp && tmp->c != ' ')
	{
		i++;
		tmp = tmp->next;
	}
	i--;
	new = malloc(sizeof(char) * (i));
	new[i] = '\0';
	i--;
	while (cmd && i >= 0)
	{
		new[j] = cmd->c;
		j++;
		i--;
		cmd = cmd->next;
	}
	return (new);
}

char		*str_before(t_command *cmd)
{
	if (!(cmd->prev) || (cmd->prev && cmd->prev->c == ' '))
		return (NULL);
	else
		return (ncmd_before(cmd));
}

char		*str_after(t_command *cmd)
{
	if (cmd->c == 0 || cmd->c == ' ')
		return (NULL);
	else
		return (ncmd_after(cmd));
}

int		is_first(t_command *cmd)
{
	cmd = cmd->prev;
	while (cmd)
	{
		if (cmd->c == ' ')
			return (0);
		if (cmd->prev == NULL)
			return (1);
		cmd = cmd->prev;
	}
	return (0);
}

void		show_lst(t_lst *lst)
{
	while (lst)
	{
		ft_putendl_fd((char*)lst->name, 0);
		lst = lst->next;
	}

}

char		*str_after_char(char *str)
{
	int		i;

	i = ft_strlen(str);
	while (i > 0)
	{
		if (str[i] == '/')
			return (ft_strdup(str + i + 1));
		i--;
	}
	return (ft_strdup(str));
}



t_list		*get_bin(t_list **hash)
{
	t_list		*new;
	t_list		*tmp;
	int			i;

	i = 0;
	new = NULL;
	while (i < 10000)
	{

		if (hash[i] != NULL)
		{
			tmp = hash[i];
			while (tmp)
			{
				ft_lstadd_to_end(&new, ft_lstnew((void*)str_after_char((char*)tmp->content), 10));

				tmp = tmp->next;
			}

		}
		i++;
	}

	return (new);
}


t_lst		*autocomplete_param(char *s1, char *s2, t_list *tmp, char *path)
{
	t_lst		*res;


	res = NULL;
	ft_putendl_fd(s1, 0);
	ft_putendl_fd(s2, 0);
	ft_putendl_fd(path, 0);
	if (s1 && s2)
	{
		while (tmp)
		{
			if (double_match((char*)tmp->content, s1, s2) == 1)
				lst_add(&res, tmp->content);
			tmp = tmp->next;
		}
	}
	else if (s1 && !s2)
	{
		while (tmp)
		{
			if (start_match(ft_strdup(tmp->content), s1) == 1)
				lst_add(&res, tmp->content);
			ft_putendl_fd(tmp->content, 0);
			tmp = tmp->next;
		}
	}
	else if (!s1 && s2)
	{
		while (tmp)
		{
			if (end_match((char*)tmp->content, s2) == 1)
				lst_add(&res, tmp->content);
			tmp = tmp->next;
		}
	}
	else if (!s1 && !s2)
	{
		while (tmp)
		{
			lst_add(&res, ft_strdup(tmp->content));
			tmp = tmp->next;
		}


	}
	if (tmp)
		ft_lstdel(&tmp, delete);
	return (res);
}

int			last_slash(char *s)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (s[i])
	{

		if (s[i] == '/')
			j = i;
		i++;
	}
	return (j);
}

void		join_cmd_line(t_command *cmd, t_lst *lst)
{
	t_lst *tmp;
	int		i;

	i = 0;
	tmp = lst;
	while (tmp)
	{
		if (tmp->pos == 1)
			break;
		tmp = tmp->next;
	}
	while (cmd && cmd->pos != 1)
		cmd = cmd->next;
	while (tmp->name[i] != '\0')
	{
		cmd_insert(cmd->prev, cmd_new(tmp->name[i], 0));
		i++;
	}
}


long		get_key_comp(t_shell *shell, t_command *tmp, t_lst *lst)
{
	long	k;

	k = 0;
	read(0, &k, sizeof(long));
	if (if_pos(lst) == 1)
		key_down_comp(lst, *shell->size);
	else
		lst->pos = 1;
	if (k == K_UP)
		key_up_comp(lst, *shell->size);
	if (k == K_LEFT)
		key_left_comp(lst);
	if (k == K_RIGHT)
		key_right_comp(lst);
	if (k == K_DOWN)
		key_down_comp(lst, *shell->size);
	if (k == K_SP)
		key_space_comp(lst);
	if (k == K_ESC)
		key_echap_comp();
	if (k >= 32 && k <= 127)
		return (k);
	if (k == K_BSP || k == K_DEL)
		key_back_comp(lst);
	join_cmd_line(tmp, lst);
	if (k == K_RET)
		return (0);
	return (1);
}


char		*take_path(char *s1)
{
	int		i;
	int		end;
	char	*new;

	i = 0;
	end = last_slash(s1);
	new = ft_strnew(ft_strlen(s1));
	while (s1[i] && i <= end)
	{
		new[i] = s1[i];
		i++;
	}
	new[i] = '\0';
	i = 0;
	end++;
	while (s1[i + end])
	{
		s1[i] = s1[i + end];
		i++;
	}
	s1[i] = '\0';
	return (new);
}


void		del_line_complete(t_command *cmd, t_shell *shell)
{
	int		len;

	len = cmd_len(cmd);
	while (len > shell->winsize.ws_row - 2)
	{
		tputs(tgetstr("dl", NULL), 1, ft_poutchar);
		tputs(tgetstr("up", NULL), 1, ft_poutchar);
		len /= shell->winsize.ws_row;
	}
//	tputs(tgetstr("dl", NULL), 1, ft_poutchar);
//	tputs(tgetstr("up", NULL), 1, ft_poutchar);
	len = shell->size->nb_elem / shell->size->nb_col;
	while (len >= 0)
	{
		tputs(tgetstr("dl", NULL), 1, ft_poutchar);
		tputs(tgetstr("up", NULL), 1, ft_poutchar);
		len--;
	}
	tputs(tgetstr("dl", NULL), 1, ft_poutchar);
}

void		removedouble(t_command *cmd)
{
	while (cmd && cmd->c != 0)
		cmd = cmd->next;
	if (cmd->c == 0)
	{
		if (cmd->next && cmd->next->c == 0)
			cmd->next = NULL;
	}
}


int			if_pos(t_lst *lst)
{
	while (lst)
	{
		if (lst->pos == 1)
			return (1);
		lst = lst->next;
	}
	return (0);
}



void			remove_cmd(t_command *cmd, int n)
{
	t_command	*tmp;
	int			i;

	
	i = n;
	
	if (i != 0)
	{
	while (cmd && i >= 0)
	{
		cmd = cmd->prev;
		i--;
	}
	tmp = cmd->prev;
	i = n;
	while (cmd->next && i >= 0)
	{
		i++;
		cmd = cmd->next;
	}
	tmp->next = cmd;
	cmd->prev = tmp;
}
}


void			remove_str(t_command *cmd, char *s1, char *s2)
{
	int			i;
	int			j;

	i = 0;
	j = 0;
	if (s1)
	{
		while (s1[i] && cmd)
		{
			while (cmd && cmd->c != s1[0])
				cmd = cmd->next;
			while (cmd && s1[i] && cmd->c == s1[i])
			{
				i++;
				cmd = cmd->next;
			}
			if (s1[i] != '\0')
				i = 0;
		}
		remove_cmd(cmd, i);
	}
	if (s2)
	{
		while (s2[j] && cmd)
		{
			while (cmd && cmd->c != s2[0])
				cmd = cmd->next;
			while (cmd && s2[j] && cmd->c == s2[j])
			{
				cmd = cmd->next;
				j++;
			}
			if (s2[j] != '\0')
				j = 0;
		}
		remove_cmd(cmd, j);
	}


}

void		print_list(t_lst *lst)
{
	while (lst)
	{
		ft_putendl_fd(lst->name, 0);
		lst = lst->next;

	}

}


long		loop_complete(t_shell *shell, t_lst *lst, char *s1, char *s2)
{
	t_command	*tmp;
	long		k;

	show_line(shell->cmd, 1);
	ft_putchar_fd('\n', 0);
	printlst(shell, lst);
	while (1)
	{
		tmp = cmd_dup(shell->cmd);
		remove_str(tmp, s1, s2);
		removedouble(tmp);
		if ((k = get_key_comp(shell, tmp, lst)) != 1)
		{
			shell->cmd = tmp;
			
			del_line_complete(tmp, shell);
			return (k);
		}
		del_line_complete(tmp, shell);
		show_line(tmp, 1);
		ft_putchar_fd('\n', 0);
		printlst(shell, lst);

	}
	shell->cmd = tmp;
	del_line_complete(tmp, shell);
}

long		key_complete(t_shell *shell)
{
	t_command	*cmd;
	char		*s1;
	char		*s2;
	char		*path;
	t_lst		*res;

	s1 = NULL;
	s2 = NULL;
	res = NULL;
	path = NULL;
	cmd = shell->cmd;
	while (cmd)
	{
		if (cmd->pos == 1)
			break;
		cmd = cmd->next;
	}
	if (!(cmd->prev))
		add_command(&(shell->cmd), '\t');
	else
	{
		s1 = str_before(cmd); 
		s2 = str_after(cmd);
		if (is_first(cmd) == 0)
		{
			if (s1 && s1[0] == '/')
				path = take_path(s1);
			if (s1)
				ft_putendl_fd(s1, 0);
			if (s2)
				ft_putendl_fd(s2, 0);
			if (path)
				ft_putendl_fd(path, 0);
			if (path)
				res = autocomplete_param(s1, s2, get_filename(path, 1), path);
			else
				res = autocomplete_param(s1, s2,get_filename(path, 0), path);
		}
		else
		{
			res = autocomplete_param(s1 , s2 , get_bin(shell->bin_hash), path);
		}
	}
	ft_putendl_fd("printlst", 0);
	return (loop_complete(shell, res, s1, s2));
}
