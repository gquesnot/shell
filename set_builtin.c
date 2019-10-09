#include "minishell.h"


char		**realloc_tab(char **envp, int max)
{
	char	**tmp;
	int		i;

	i = 0;
	tmp = malloc((max + 2) * sizeof(char*));
	while (envp[i])
	{
		tmp[i] = ft_strdup(envp[i]);
		i++;
	}
	tmp[i] = NULL;
	tmp[i + 1] = NULL;
	return (tmp);
}


char		*ft_strccdup(char *s)
{
	char	*tmp;
	int		i;

	i = 0;
	while (s[i] != '=' && s[i])
		i++;
	if (!s[i])
		return (s);
	tmp = malloc((i + 1) * sizeof(char));
	i = 0;
	while (s[i] != '=' && s[i])
	{
		tmp[i] = s[i];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}




int			env_pos(char **envp, char *elem)
{
	int		i;
	

	i = 0;
	while (envp[i])
	{
		if (ft_strstr(envp[i], elem) == envp[i])
			return (i);
		i++;
	}
	return (i);
}





char		*get_env(char **envp, char *elem)
{
	int		i;
	int		j;
	if (!elem)
		return (NULL);
	j = 0;
	i = env_pos(envp, ft_strccdup(elem));
	if (envp[i])
	{
		while (envp[i][j] != '=')
			j++;
		return (ft_strdup(envp[i] + j + 1));
	}
	return (NULL);
}

char		**set_env(char **envp, char *elem, char *value)
{
	int		i;
	char	*tmp;

	if (elem == NULL && value == NULL)
	{
		print_tab(envp);
		return (envp);
	}
	i = env_pos(envp, ft_strccdup(elem));
	tmp = ft_strdup(elem);
	if (envp[i])
	{
		if (value)
			tmp = ft_strjoin(tmp, value);
		envp[i] = tmp;
	}
	else
	{
			envp = realloc_tab(envp, i);
		if (value)
			envp[i] = ft_strjoin(tmp, value);
		else
			envp[i] = tmp;
	}
	return (envp);
}

char		**unset_env(char **envp, char *elem)
{
	int		i;

	i = env_pos(envp, elem);
	if (envp[i])
	{
		free(envp[i]);
		i++;
		while (envp[i])
		{
			envp[i - 1] = envp[i];
			i++;
		}
		envp[i] = NULL;
		return (envp);
	}
	else
		return (envp);
}



