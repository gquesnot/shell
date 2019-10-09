#include "minishell.h"

int		hash(char *str)
{
	int		i;
	int		res;

	res = 1;
	i = 0;
	while (str[i])
	{
		res = str[i] + (res << 7) + (res << 15) - res;

	i++;
	}
	res %= 10000;
	if (res < 0)
		res *= -1;
	return (res);
}

int		check_hash(int k, t_list **tabb)
{
	if (tabb[k] != NULL)
		return (1);
	else
	return (0);

}

void		init_tab(t_list **tabb)
{
	int		i;
	
	i = 0;
	while (i < 10000)
	{
		
		tabb[i] = NULL;
		i++;
	}

}


int		read_tabb(t_list **tabb)
{
	t_list		*tmp;
	int		i;
	int		j;
	int		k;
	
	
	j = 0;
	i = 0;
	k = 0;
	while (i < 10000)
	{
		if (tabb[i] != NULL)
		{
			ft_putnbr_fd(k, 0);
			ft_putstr_fd(" : ", 0);
			k++;
			ft_putstr_fd((char*)tabb[i]->content, 0);
		if (tabb[i]->next)
		{
			tmp = tabb[i]->next;
			while (tmp)
			{
				ft_putstr_fd("/ ", 0);
				ft_putstr_fd((char*)tmp->content, 0);
				tmp = tmp->next;
				j++;
			}
		}
		j++;
		ft_putchar_fd('\n', 0);
		}
		i++;
	}
	return (j);
}

char		*get_bin_hash(t_list **bin_hash, char *cmd)
{

	t_list	*tmp;
	int	hash1;

	hash1 = hash(cmd);
	if (bin_hash[hash1])
	{
		tmp = bin_hash[hash1];
		if (ft_strstr(tmp->content, cmd) != NULL)
			return (tmp->content);
		while (tmp)
		{
			
			if (ft_strstr(tmp->content, cmd) != NULL)
				return (tmp->content);
			tmp = tmp->next;
		}
	}
	return (NULL);
}





t_list		**create_hash(char **bin)
{
	t_list		**res;
	char		*tmp;
	DIR		*dir;
	struct dirent	*dirent;
	int				i;
	int				hash1;

	i = 0;
	res = malloc(sizeof(t_list*) * 10000);
	init_tab(res);
	while (bin[i] != NULL)
	{
		if ((dir = opendir(bin[i])) != NULL)
		{
		while ((dirent = readdir(dir)) != NULL)
		{
			tmp = ft_strjoin(bin[i], "/");
			tmp = ft_strjoin(tmp, dirent->d_name);
			if (access(tmp, X_OK) == 0 && dirent->d_type == DT_REG)
			{
			hash1 = hash(dirent->d_name);
			ft_lstadd_to_end(&(res[hash1]), ft_lstnew(tmp, ft_strlen(tmp)));
			}
		}
		}
		closedir(dir);
		i++;
	}
	return (res);
}
