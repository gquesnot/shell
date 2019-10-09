#include "minishell.h"
 
void		show_quote(t_shell *shell, int set, int show)
{
	t_command *tmp;

	tmp = shell->cmd;
	ft_putstr_fd(":>", 0);
	if (tmp)
	{
		while (tmp)
		{
			if ((tmp->pos == 1 || tmp->cpy == 1) && show == 1)
				ft_putstr_fd(BLINK, 0);
			if (tmp->c == 0)
				ft_putchar_fd(' ', 0);
			else
				ft_putchar_fd(tmp->c, 0);
			if ((tmp->pos == 1 || tmp->cpy == 1) && show == 1)
				ft_putstr_fd(RESET, 0);
			if (tmp->c == '\n' && set == 1)
				ft_putstr_fd("dquote> ", 0);
			else if (tmp->c == '\n' && set == 0)
				ft_putstr_fd("quote> ", 0);
			tmp = tmp->next;
		}
	}
	else
	{
		ft_putstr_fd(BLINK, 0);
		ft_putchar_fd(' ', 0);
		ft_putstr_fd(RESET, 0);

	}
	if (show == 0)
		ft_putchar_fd('\n', 0);
}


int		get_cmd_line(t_shell *shell, t_command *cmd, int k)
{
	t_command *t;
	int		i;
	int		res;
	int		set;

	i = 0;
	res = 0;
	t = cmd;
	set = 1;
	while (t)
	{
		if (t->c == '\n')
		{
			if (set == 1)
			{
				while (i > shell->winsize.ws_col - 3)
				{
					i = i - shell->winsize.ws_col;
					res++;
				}
				set = 0;
			}
			else
			{
				while (i > shell->winsize.ws_col - 6 + k)
				{
					i = i - shell->winsize.ws_col;
					res++;
				}
			}
			res++;
			i = 0;
		}
		else
			i++;
		t = t->next;
	}
	if (set == 1)
	{
		while (i > shell->winsize.ws_col - 3)
		{
			i -= shell->winsize.ws_col;
			res++;
		}

	}
	if (set == 0)
	{
		while (i > shell->winsize.ws_col - 6 + k)
		{
			i -= shell->winsize.ws_col;
			res++;
		}
	}
	return (res + 1);
}


void		del_quote(t_shell *shell, int k)
{
	int		i;

	i = get_cmd_line(shell, shell->cmd , k);
	while (i > 0)
	{
		tputs(tgetstr("dl", NULL), 1, ft_poutchar);
		i--;
		if (i != 0)
			tputs(tgetstr("up", NULL), 1, ft_poutchar);
	}
}

int		get_pos(t_command *cmd)
{
	int		i;

	i = 0;
	while (cmd && cmd->pos != 1)
	{
		cmd = cmd->next;
		i++;
	}
	return (i);
}


int		key_b_is_quote(t_command *cmd, int quote)
{
	t_command *tmp;

	tmp = cmd;
	while (tmp && tmp->pos != 1)
		tmp = tmp->next;
	if (tmp->prev && tmp->prev->c == 39 && quote == 0)
		return (1);
	if (tmp->prev && tmp->prev->c == 34 && quote == 1)
		return (1);
	return (0);
}



void	quote(t_shell *shell, int quote)
{
	long	c;
	int		set;
	int		close;
	c = 0;
	close = 1;
	while (1)
	{
		read(0, &c, sizeof(long));
		if (c == K_RIGHT)
		{
			key_rightt(shell);
		}
		else if (c == K_LEFT)
		{
			key_leftt(shell);
		}
		if ((int)c >= 32 && (int)c <= 126 && (int)c != 39 && c != 34)
		{
			add_command(&(shell->cmd), (char)c);
			if (set == 1)
				set = 0;
		}
		else if ((int)c == 9)
		{
			add_command(&(shell->cmd), (char)c);
		}
		else if ((int)c == 10 && close == 1)
			add_command(&(shell->cmd), (char)c);
		else if ((int)c == K_RET && close == 0)
			return;
		else if ((int)c == 39 || (int)c == 34)
		{
			if (close == 0)
			{
				if ((int)c == 39)
					quote = 0;
				else
					quote = 1;
				close = 1;
			}
			else if (close == 1)
			{
			if (quote == 1 && (int)c == 34)
				close = 0;
			else if (quote == 0 && (int)c == 39)
				close = 0;
			}
			add_command(&(shell->cmd), (char)c);
		}
		else if (c == K_BSP)
		{
			if (key_b_is_quote(shell->cmd, quote))
			{
			if (close == 1)
					close = 0;
				else
					close = 1;
			}
			key_back(&(shell->cmd), shell);

		}
		del_quote(shell, quote);
		show_quote(shell, quote, 1);
		c = 0;
	}
}

int		quote_open(t_command *cmd)
{
	t_command *tmp;
	int		quote;

	tmp = cmd;
	quote = 0;
	while (tmp)
	{
		if (quote == 0 && tmp->c == 34)
			quote = 2;
		else if (quote == 0 && tmp->c == 39)
			quote = 1;
		else if (quote == 1 && tmp->c == 39)
			quote = 0;
		else if (quote == 2 && tmp->c == 34)
			quote = 0;
		tmp = tmp->next;
	}
	return (quote);
}


void		key_quote(t_shell *shell, int k)
{

	del_line(shell);
	//	show_curs();
	//	process_term(shell);
	//add_command(&(shell->cmd), (char)k);
	add_cmd_to_shell(shell->cmd);
	tputs(tgetstr("dl", NULL), 1, ft_poutchar);
	if (k == 34)
	show_quote(shell, 1, 1);
	else
	show_quote(shell, 0, 1);
	if (k == 34)
		quote(shell, 1);
	else
		quote(shell, 0);
	add_cmd_to_shell(shell->cmd);
	//	update_term(shell);
}
