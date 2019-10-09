#include "minishell.h"

int is_directory(char *dir)
{
	struct stat statbf;

	stat(dir, &statbf);
	if (S_ISDIR(statbf.st_mode) || S_ISLNK(statbf.st_mode))
		return (1);
	else
		return (0);
}

char		*remove_point(char *str)
{

	if (ft_strlen(str) <= 3)
		return (ft_strdup("/\0"));
	else
		return (ft_strdup((str + 3)));
}



char		*remove_last(char *str, char c, char *dir)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (dir)
	{
	while (str[i])
	{
		if (str[i] == c && str[i + 1] != '\0')
			j = i;
		i++;
	}
	{
		str[j] = '\0';
		j++;
	}
	dir = remove_point(dir);
	if (dir)
	{
	if (dir[0] == '.' && dir[1] == '.')
			dir = remove_last(str, c,dir);
	}
	}
	ft_putendl_fd(dir, 0);
	return (dir);
}

char		*remove_double(char *str, char c)
{
	char *tmp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	tmp = ft_strnew(ft_strlen(str));
	while (str[i])
	{
		tmp[j] = str[i];
		if (str[i] == c && str[i + 1] == c)
			i++;
		i++;
		j++;
	}
	return (tmp);
}



char		*make_dir(t_shell *shell, t_arg *arg)
{
	char *dir;
	char *tmp;

	tmp = ft_strnew(50);
	dir = ft_strdup(arg->argv[1]);
	if (dir[0] == '~')
		tmp = get_env(shell->envp, "HOME");
	else
		tmp = getcwd(tmp, 50);
	if ((dir[0] == '.' && dir[1] != '.') || dir[0] == '~')
		dir = ft_strjoin(tmp, dir + 1);
	else if (dir[0] == '.' && dir[1] == '.')
	{
		dir = remove_last(tmp, '/', dir);
		if (dir[1] != '\0')
		{
			tmp = ft_strjoin(tmp, "/");
			dir = ft_strjoin(tmp, dir);
		}
		else
			return (tmp);
	}
	else if (dir[0] == '/')
		return (dir);
	else
	{
		tmp = ft_strjoin(tmp, "/");
		dir = ft_strjoin(tmp, dir);
	}
	dir = remove_double(dir, '/');
	return (dir);
}


void	cd_builtin(t_shell *shell, t_arg *arg)
{
	char *dir;

	dir = ft_strnew(50);
	if (arg->argv[1] == NULL)
	{
		ft_putendl_fd(getcwd(dir, 50), 0);
		return ;
	}
	dir = make_dir(shell, arg);
	ft_putendl_fd(dir, 0);
	if (is_directory(dir) == 1)
	{
			chdir(dir);
			shell->envp = set_env(shell->envp, "PWD", dir);
	}
	else
	{
		ft_putstr_fd(dir, 0);
		ft_putendl_fd(" is not a directory", 0);
	}
}
