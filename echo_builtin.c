#include "minishell.h"
#include "stdio.h"

void		echo_builtin(t_shell *shell, t_arg *arg)
{
	int		i;
	int		j;

	i = 1;
	while (arg->argv[i])
	{
		if (ft_strchr(arg->argv[i], '$'))
		{
			j = 0;
			while (arg->argv[i][j] != '$')
			{
				ft_putchar_fd(arg->argv[i][j], 0);
				j++;
			}
			if ((int)ft_strlen(arg->argv[i]) != j + 1)
				ft_putstr_fd(get_env(shell->envp, (arg->argv[i]) + j + 1), 0);
		}
		else
		{
			ft_putstr_fd(arg->argv[i], 0);
			ft_putchar_fd(' ', 0);
		}
		i++;
	}
	ft_putchar_fd('\n', 0);
}
