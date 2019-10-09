#include "minishell.h"

void	key_cpy_left(t_command *cmd)
{
	t_command *tmp;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->pos == 1)
		{
			if (tmp->prev && tmp->prev->cpy == 1)
			{
				while (tmp->prev && tmp->prev->cpy == 1)
					tmp = tmp->prev;
				if (tmp->prev)
					tmp->prev->cpy = 1;

			}
			else if (tmp->next && tmp->next->cpy == 1)
			{
				while (tmp->next && tmp->next->cpy == 1)
					tmp = tmp->next;
				if (tmp->next)
					tmp->cpy = 0;
				else if (tmp->next == NULL)
					tmp->cpy  = 0;
			}
			else if (tmp->prev)
				tmp->prev->cpy = 1;
			return;
		}
		tmp = tmp->next;
	}
}


void		key_cpy_right(t_command *cmd)
{
	t_command *tmp;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->pos == 1)
		{
			if (tmp->prev && tmp->prev->cpy == 1)
			{
				while (tmp->prev && tmp->prev->cpy == 1)
					tmp = tmp->prev;
				if (tmp->prev)
					tmp->cpy = 0;
				else if (tmp->prev == NULL)
					tmp->cpy = 0;
			}
			else if (tmp->next && tmp->next->cpy == 1)
			{
				while (tmp->next && tmp->next->cpy == 1)
					tmp = tmp->next;
				if (tmp->next)
					tmp->next->cpy = 1;
			}
			else if (tmp->next)
				tmp->next->cpy = 1;
			return;
		}
		tmp = tmp->next;
	}
}


void		key_cpy_cright(t_command *cmmd)
{
	t_command *cmd;
	
	cmd = cmmd;
	while (cmd && cmd->pos != 1)
		cmd = cmd->next;
	if (cmd->prev && cmd->prev->cpy == 1)
	{
		if (cmd->next)
		{
			cmd->pos = 0;
			cmd->next->pos = 1;
			cmd->next->cpy = 1;
		}
	}
	else if (cmd->next && cmd->next->cpy == 1)
	{
		cmd->pos = 0;
		cmd->cpy = 0;
		cmd->next->pos = 1;
	}
	else if (((cmd->prev && cmd->prev->cpy == 0) || cmd->prev == NULL) && (cmd->next && cmd->next->cpy == 0))
	{
		if (cmd->next)
		{
			cmd->pos = 0;
			cmd->cpy = 0;
			cmd->next->pos = 1;
			cmd->next->cpy = 1;
		}
	}


}


void		key_cpy_cleft(t_command *cmmd)
{
	t_command *cmd;

	cmd = cmmd;
	while (cmd && cmd->pos != 1)
		cmd = cmd->next;
	if (cmd->next && cmd->next->cpy == 1)
	{
		if (cmd->prev)
		{
			cmd->pos = 0;
			cmd->prev->pos = 1;
			cmd->prev->cpy = 1;
		}
	}
	else if (cmd->prev && cmd->prev->cpy == 1)
	{
		cmd->pos = 0;
		cmd->cpy = 0;
		cmd->prev->pos = 1;
	}
	else if ((cmd->prev && cmd->prev->cpy == 0) && ((cmd->next && cmd->next->cpy == 0) || cmd->next == NULL))
	{
		if (cmd->prev)
		{
			cmd->pos = 0;
			cmd->cpy = 0;
			cmd->prev->pos = 1;
			cmd->prev->cpy = 1;
		}
	}
}



char *ft_straddchr(char *str, char c)
{
	int		i;
	char *new;

	i = 0;
	if (str == NULL)
	{
		new = ft_strnew(2);
	}
	else
	{
		new = ft_strnew(ft_strlen(str) + 1);
		while (str[i] != '\0')
		{
			new[i] = str[i];
			i++;
		}
	}
	new[i] = c;
	new[i + 1] = '\0';
	free(str);
	return (new);
}


void		key_cpy_return(t_shell *shell, t_command *cmd)
{
	char *str;
	t_command *tmp;

	tmp = cmd;
	str = NULL;
	while (tmp && tmp->cpy != 1)
		tmp = tmp->next;
	while (tmp && tmp->cpy == 1)
	{

		str = ft_straddchr(str, tmp->c);
		tmp = tmp->next;
	}
	shell->cpy_line = str;
}

void		key_insert_cpy(t_shell *shell)
{
	int			i;

	i = 0;
	if (shell->cpy_line != NULL)
	{
		if (!(shell->cmd))
		{
			add_command(&(shell->cmd), shell->cpy_line[0]);
			i++;

		}
		while (shell->cpy_line[i])
		{
			add_command(&(shell->cmd), shell->cpy_line[i]);
			i++;
		}
	}
}

t_command *cmd_cpy(t_command *cmd)
{
	t_command *cpy;
	t_command *cpy_tmp;
	t_command *tmp;

	cpy = NULL;
	tmp = cmd;
	while (cmd->next)
	{
		add_command(&cpy, cmd->c);
		cmd = cmd->next;
	}
	cpy_tmp = cpy;
	while (tmp && cpy)
	{
		if (tmp->pos == 1)
		{
			cpy->pos = 1;
			cpy->cpy = 1;
		}
		else if (cpy->pos == 1)
			cpy->pos = 0;
		tmp = tmp->next;
		cpy = cpy->next;
	}
	return (cpy_tmp);
}



void			key_cpymode(t_shell *shell)
{
	long		buffer;
	t_command	*tmp;

	tmp = cmd_cpy(shell->cmd);
	while (1)
	{
		del_line(shell);
		show_line(tmp, 1);
		buffer = 0;
		read(0, &buffer, sizeof(long));
		if (buffer == K_RIGHT)
			key_cpy_right(tmp);
		else if (buffer == K_LEFT)
			key_cpy_left(tmp);
		else if (buffer == K_CLEFT)
			key_cpy_cleft(tmp);
		else if (buffer == K_CRIGHT)
			key_cpy_cright(tmp);
		else if (buffer == 10 || buffer == 13)
		{
			key_cpy_return(shell, tmp);
			return ;
		}
	}
}
