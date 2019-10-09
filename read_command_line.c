#include "minishell.h"
#include <stdio.h>

t_command *cmd_new(char c, int pos);

int  ft_poutchar(int c)
{
	return (write(0, &c, 1));
}

char		*add_cmd_to_shell(t_command *cmd)
{
	t_command	*tmp;
	int			i;
	char	*line;


	i = 0;
	tmp = cmd;
	while (tmp)
	{
		tmp = tmp->next;
		i++;
	}
	line = ft_strnew(i + 1);
	i = 0;
	tmp = cmd;
	while (tmp)
	{
		line[i] = tmp->c;
		i++;
		tmp = tmp->next;
	}
	return (line);
}

t_command		*add_line_to_cmd(char *str)
{
	t_command *tmp;
	int			i;

	i = 0;
	tmp = NULL;
	if (!str)
		return (tmp);
	else
	{
		while (str[i])
		{
			add_command(&tmp, str[i]);
			i++;
		}
	}
	return (tmp);
}


void		free_command(t_command **command)
{
	t_command *save;
	t_command *tmp;

	tmp = *command;
	while (tmp->next != NULL)
	{
		save = tmp->next;
		free(tmp);
		tmp = save;
	}
	*command = NULL;
}




t_command *cmd_new(char c, int pos)
{
	t_command *t;

	t = malloc(sizeof(t_command));
	t->c = c;
	if (pos == 1)
		t->pos = 1;
	else
		t->pos = 0;
	t->cpy = 0;
	t->next = NULL;
	t->prev = NULL;
	return (t);
}

void		add_command2(t_command **cmd, t_command *new)
{
	t_command *tmp;

	tmp = *cmd;
	if (!tmp)
	{
	*cmd = new;
		(*cmd)->next = cmd_new(0, 1);
		(*cmd)->next->prev = *cmd;
	}
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		if (tmp->prev == NULL)
		{
			*cmd = new;
			(*cmd)->next = tmp;
			tmp->prev = *cmd;
		}
		else
			cmd_insert(tmp->prev, new);
	}
}


t_command		*cmd_dup(t_command *cmd)
{
	t_command	*new;
	t_command	*tmp;
	t_command	*res;

	new = NULL;
	tmp = cmd;
	res = NULL;
	while (tmp)
	{
		new = cmd_new(tmp->c, tmp->pos);
		add_command2(&res, new);
		tmp = tmp->next;
	}
	return (res);
}

void		cmd_insert(t_command *t, t_command *new)
{
	t_command *tmp;

	tmp = t->next;
	t->next = new;
	t->next->prev = t;
	t->next->next = tmp;
	tmp->prev = new;
}

void		add_command(t_command **cmd, char c)
{
	t_command *tmp;

	tmp = *cmd;
	if (!tmp)
	{
		*cmd = cmd_new(c, 0);
		(*cmd)->next = cmd_new(0, 1);
		(*cmd)->next->prev = *cmd;
	}
	else
	{
		while (tmp && tmp->pos != 1)
			tmp = tmp->next;

		if (tmp->prev == NULL)
		{
			*cmd = cmd_new(c, 0);
			(*cmd)->next = tmp;
			tmp->prev = *cmd;

		}
		else
			cmd_insert(tmp->prev, cmd_new(c, 0));
	}
}



void		get_key(t_shell *shell)
{
	long	k;
	int quote;

	tputs(tgetstr("mi", NULL), 1, ft_poutchar);
	shell->tmp = NULL;
	shell->tmp_line = NULL;
	shell->k_up_set = 0;
	signal_handler();
	while (1)
	{
		//	if (shell->k_up_set > 0)
		//		show_line(shell->tmp_cmd, 1);
		//	else
		quote = quote_open(shell->cmd);
		if (quote != 0)
		{
			if (quote == 1)
				key_quote(shell, 39);
			else
				key_quote(shell, 34);
			return;
		}
		show_line(shell->cmd, 1);
		k = 0;
		read(0, &k, sizeof(long));
		if (k == (int)K_RIGHT)
			key_rightt(shell);
		if (k == K_TAB)
			key_complete(shell);
		else if (k == (int)K_LEFT)
			key_leftt(shell);
		else if (k == (int)K_UP || k == (int)K_DOWN)
			key_up_down(shell, k);
		else if (k == (int)K_BSP)
			key_back(&(shell->cmd), shell);
		else if (k == K_CLEFT)
			key_cleft(shell->cmd);
		else if (k == K_CRIGHT)
			key_cright(shell->cmd);
		else if (k == K_CUP)
			key_cup(shell->cmd, shell);
		else if (k == K_CDOWN)
			key_cdown(shell->cmd, shell);
		else if (k == K_END)
			key_endd(shell);
		else if (k == K_HOME)
			key_homee(shell);
		else if (k == K_CPYMODE)
			key_cpymode(shell);
		else if (k == K_PASTE)
			key_insert_cpy(shell);
		else if (k == 34 || k == 39)
		{
			add_command(&(shell->cmd), (char)k);
			if (quote_open(shell->cmd) != 0)
			{
				key_quote(shell, (char)k);
				return;
			}
		}
		else if (k == 10 || k == 13)
		{
//			del_line(shell);
			if (shell->cmd == NULL)
				shell->line = NULL;
			if (shell->tmp_line != NULL)
				shell->line = shell->tmp_line;
			return ;
		}
		else if (k >= 25 && k <= 126)
			add_command(&(shell->cmd), (char)k);
		del_line(shell);
		if (k != K_UP && k != K_DOWN)
		{
			shell->tmp = NULL;
			shell->k_up_set = 0;
			shell->tmp_line = NULL;
		}
		if (shell->k_up_set == 0 && shell->tmp_line != NULL)
		{

			shell->line = ft_strdup(shell->tmp_line);
			//	shell->tmp_line = NULL;
		}
		else if (shell->k_up_set == 0 && shell->tmp_line == NULL && shell->cmd)
		{
			shell->line = add_cmd_to_shell(shell->cmd);
		}
		else if (shell->cmd == NULL && shell->tmp_line == NULL && shell->line == NULL)
		{
		}
		//	if (shell->tmp_line == NULL)
		//		shell->cmd = add_line_to_cmd(shell->line);
		if (shell->tmp != NULL)
			shell->cmd = add_line_to_cmd(shell->tmp_line);
		else if (shell->tmp == NULL && shell->cmd == NULL)
		{
			shell->cmd = add_line_to_cmd(shell->line);

		}
	}
}

void		show_line(t_command *cmd, int set)
{
	t_command *t;

	t = cmd;
	ft_putstr_fd(":>", 0);
	if (t)
	{
		while (t)
		{
			if ((t->pos == 1 && set == 1) || t->cpy == 1)
				ft_putstr_fd(BLINK, 0);
			if (t->c == 0)
				ft_putchar_fd(' ', 0);
			else
				ft_putchar_fd(t->c, 0);
			if ((t->pos == 1 && set == 1) || t->cpy == 1)
				ft_putstr_fd(RESET, 0);
			t = t->next;
		}
	}
	else
	{
		ft_putstr_fd(BLINK, 0);
		ft_putchar_fd(' ', 0);
		ft_putstr_fd(RESET, 0);
	}
	if (set == 0)
		ft_putchar_fd('\n', 0);
}

