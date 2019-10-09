#include "minishell.h"

void		print_lst(t_list *lst)
{
	while (lst)
	{
		ft_putendl_fd(lst->content, 0);
		lst = lst->next;
	}
}

t_list		*get_filename(char *directory, int set)
{
	char	*path;
	DIR *dir;
	struct dirent	*tmp;
	t_list	*lst;

	lst = NULL;
	if (!directory)
	{
	path = ft_strnew(50);
	path = getcwd(path, 50);
	}
	else
		path = directory;
	dir = opendir(path);
	while ((tmp = readdir(dir)) != NULL)
	{
		if (ft_strequ(tmp->d_name, ".") == 1 || ft_strequ(tmp->d_name, "..") == 1)
			tmp = NULL;
		else if (tmp->d_type == DT_DIR)
			ft_lstadd_to_end(&lst, ft_lstnew(ft_strjoin(tmp->d_name, "/"), ft_strlen(tmp->d_name)));
		else if (set == 0)
			ft_lstadd_to_end(&lst, ft_lstnew(ft_strdup(tmp->d_name), ft_strlen(tmp->d_name)));
		else if (set == 1)
			ft_lstadd_to_end(&lst, ft_lstnew(ft_strdup(tmp->d_name), ft_strlen(tmp->d_name)));
	}
	return (lst);
}


void		del_one_str(char **tabb, int n)
{
	int		i;

	i = 0;
	tabb[n] = NULL;
	while (tabb[n + i + 1])
	{
		tabb[n + i] = tabb[n + i + 1];
		i++;
	}
	tabb[n + i] = NULL;
}


int		match_2(char *line, char c)
{
	int		i;

	i = 0;
	if (line[2] == '-')
	{
		while ((char)(line[1] + i) <= line[3])
		{
			if ((char)(line[1] + i) == c)
				return (2);
			i++;
		}
	}
	else
	{
		if (c == line[1] || c == line[2])
			return (1);
	}
	return (0);
}


int		match(char *s1, char *s2)
{
	int		k;

	if (*s1 == '\0' && *s2 == '\0')
		return (1);
	else if (*s1 == '\0' || *s2 == '\0')
		return (0);
	else if (*s1 == *s2)
		return (match(s1 + 1, s2 + 1));
	else if (*s2 == '*')
		return (match(s1 + 1, s2) || match(s1 + 1 , s2 + 1));
	else if (*s2 == '?')
		return (match(s1 + 1 , s2 + 1));
	else if (*s2 == '[')
	{
		k = match_2(s2, *s1);
		if (k == 1)
			return (match(s1 + 1, s2 + 4));
		if (k == 2)
			return (match(s1 + 1, s2 + 5));
		else 
			return (0);
	}
	else
		return (0);
}

char			**inser_in_tab(char **fct, char *line)
{
	int			i;
	char		**tmp;

	i = 0;
	while (fct[i])
		i++;
	tmp = malloc(sizeof(char*) * (i + 1));
	i = 0;
	while (fct[i])
	{
		tmp[i] = ft_strdup(fct[i]);
		i++;
	}
	tmp[i] = ft_strdup(line);
	tmp[i + 1] = NULL;
	return (tmp);

}


char		**ft_wildcard(char **fct, int *n)
{
	t_list	*res;
	char	*line;
	int		close;

	close = 0;
	res = get_filename(NULL, 0);
	line = ft_strdup(fct[*n]);
	del_one_str(fct, *n);
	while (res && res->content)
	{
		if (match((char*)res->content, line) != 0)
		{
			fct = inser_in_tab(fct,(char*)res->content);
			close++;
		}
		res = res->next;
	}
	if (close == 0)
	{
		fct = inser_in_tab(fct, ft_strdup("no match"));
		ft_putendl_fd("sh: no match", 0);
	}
	return (fct);
}


char	*replace_str(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*cpy;

	i = 0;
	j = 0;
	if (s2)
		cpy = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)));
	else
		cpy = malloc(sizeof(char) * ft_strlen(s1) + 1);
	while (s1[i] && s1[i] != '$')
	{
		cpy[i] = s1[i];
		i++;
	}
	if (s2)
	{
		while (s2[j])
		{
			cpy[i + j] = s2[j];
			j++;
		}
		j = j + i;
	}
	i++;
	if (j == 0)
		j = i - 1;
	while (s1[i]  && ft_isalpha(s1[i]) == 1)
		i++;
	while (s1[i])
	{
		cpy[j] = s1[i];
		i++;
		j++;
	}
	cpy[j] = '\0';
	return (cpy);
}



char		*get_dollar(char *str,t_shell *shell)
{
	int		i;
	int		j;
	int		set;

	i = 0;
	set = 0;
	while (str[i])
	{
		if (str[i] == 39)
			set = 1;
		else if (str[i] == 39 && set == 1)
			set = 0;
		if (str[i] == '$' && set == 0)
		{
			i++;
			j = i;
			while (str[i])
			{
				if (ft_isalpha(str[i]) != 1)
				{
					str = replace_str(str, get_env(shell->envp, ft_strsub(str, j, i - j)));
					i = j + 1;
					break;
				}
				i++;
			}
			if (str[i] == '\0')
				str = replace_str(str, get_env(shell->envp, ft_strsub(str, j , i - j)));

		}
		i++;
	}
	return (str);
}

int		is_metachar(char c)
{
	if (c == '*' || c == '?' || c == '[' || c == ']')
		return (1);
	return (0);
}


int		strchr_in_quote(char *str)
{
	int		i;
	int		quote;
	int		res;


	i = 0;
	res = 0;
	quote = 0;
	while (str[i])
	{
		if (quote == 0 && is_metachar(str[i]) == 1)
			res++;
		if ((str[i] == 34 || str[i] == 39) && quote != 0)
		{
			if (quote == 2 && str[i] == 34)
				quote = 0;
			else if (quote == 1 && str[i] == 39)
				quote = 0;

		}
		else if ((str[i] == 34 || str[i] == 39) && quote == 0)
			{
				if (str[i] == 34)
					quote = 2;
				else
					quote = 1;
			}
		i++;
	}
	return (res);
}


int		metachar_in_without_quote(char *str)
{
	int		i;
	int		res;

	i = 0;
	res = 0;
	while (str[i])
	{
		if (str[i] == 34 || str[i] == 39)
			return (0);
		if (is_metachar(str[i]))
			res++;
	i++;
	}
	return (res);
}

int		dollar_witout_quote(char *str)
{
	int		i;
	int		quote;
	int		res;

	quote = 0;
	res = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == 34 && quote == 1)
			quote = 0;
		if (str[i] == 34 && quote == 0)
			quote = 1;
		if (str[i] == 39 && quote == 0)
			return (0);
		if (str[i] == '$')
			res++;
	i++;
	}

	return (res);
}

char		*remove_char(char *str, int n)
{
	int		i;

	i = 0;
	while (str[i + n + 1])
	{
		str[i + n] = str[i + n + 1];
		i++;
	}
	str[i + n] = '\0';
	return (str);
}



char		*remove_quote_line(char *str)
{
	int		i;
	int		quote;

	quote = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == 34 || str[i] == 39)
		{
		if (str[i] == 34 && quote == 2)
			quote = 0;
		else if (str[i] == 39 && quote == 1)
			quote = 0;
		else if (str[i] == 34 && quote == 0)
			quote = 2;
		else if (str[i] == 39 && quote == 0)
			quote = 1;
		if ((str[i] == 39 && (quote == 0 || quote == 1)) || (str[i] == 34 && (quote == 0 || quote == 2)))
			str = remove_char(str, i);

		}
		i++;
	}
	return (str);
}


void		remove_quote(char **fct)
{
	int		i;

	i = 0;
	while (fct[i])
	{
		fct[i] = remove_quote_line(fct[i]);
		i++;
	}
}

char		**metachar(char **fct, t_shell *shell)
{
	int		i;

	i = 0;
	while (fct[i])
	{
 	if (metachar_in_without_quote(fct[i]) != 0)
		{
			fct = ft_wildcard(fct, &i);
			i = 0;
		}
		else if (dollar_witout_quote(fct[i]) != 0)
			fct[i] = get_dollar(fct[i], shell);
		else
			i++;
	}
	remove_quote(fct);
	return (fct);
}
