#include "minishell.h"

int			cmd_len(t_command *cmd)
{
	int		i;

	i = 0;
	while (cmd)
	{
		i++;
		cmd = cmd->next;
	}
	return (i);
}


void		del_line(t_shell *shell)
{
	int			i;

	i = cmd_len(shell->cmd);
	while (i > shell->winsize.ws_col - 3)
	{
		tputs(tgetstr("dl", NULL), 1, ft_poutchar);
		i = i - shell->winsize.ws_col;
		tputs(tgetstr("up", NULL), 1, ft_poutchar);
	}
	tputs(tgetstr("dl", NULL), 1, ft_poutchar);
}




void		make_hist(t_shell *shell, int all)
{
	t_hist *hist;
	int		i;

	i = 1;
	hist = shell->history;
	shell->k_up_set = 0;

	shell->tmp = new_history(shell->line , -1);
	add_history(&(shell->tmp), shell->line);
	while (hist)
	{
		if (all == 1 && hist->pos != 0)
		{
			add_history(&(shell->tmp), hist->line);
			i++;

		}
		else if (ft_strstr(hist->line, shell->line) != NULL)
		{
			add_history(&(shell->tmp), hist->line);
			i++;
		}
		hist =  hist->next;
	}
}



int			last_history(t_hist *hist, char *line)
{
	int		last;

	last = hist->pos;
	while (hist)
	{
		if (ft_strstr(line, hist->line) != NULL &&  hist->pos < last)
			last = hist->pos;
		hist = hist->next;
	}
	return (last);
}

int			first_history(t_hist *hist, char *line, int pos)
{
	int		first;

	while (hist && hist->pos > pos)
	{
		if (ft_strstr(line, hist->line) != NULL)
			first = hist->pos;
		hist = hist->next;
	}
	return (first);
}

void		key_leftt(t_shell *shell)
{
	t_command *tmp;

	tmp = shell->cmd;
	if (tmp == NULL || tmp->c == 0)
		return ;
	while (tmp && tmp->pos != 1)
		tmp = tmp->next;
	if (tmp->prev)
	{
		tmp->pos = 0;
		tmp->prev->pos = 1;
	}
}

void		key_rightt(t_shell *shell)
{
	t_command *tmp;


	tmp = shell->cmd;
	if (tmp == NULL || tmp->c == 0)
		return;
	while (tmp && tmp->pos != 1)
		tmp = tmp->next;
	if (tmp->next)
	{
		tmp->pos = 0;
		tmp->next->pos = 1;
	}
}

void		key_up_down(t_shell *shell, long k)
{
	if (shell->history == NULL)
		return;
	else if (shell->line == NULL && shell->k_up_set == 0)
		make_hist(shell, 1);
	if (shell->tmp == NULL)
		make_hist(shell, 0);
	if (k == (int)K_UP)
		key_uupp(shell);
	else
		key_downn(shell);
}


void		key_uupp(t_shell *shell)
{
	t_hist *tmp;

	tmp = shell->tmp;
	if (shell->k_up_set == tmp->pos)
		return ;
	else
	{
		shell->k_up_set++;
		while (tmp && tmp->pos != shell->k_up_set)
			tmp = tmp->next;
		shell->tmp_line = ft_strdup(tmp->line);
	}
}

void		key_downn(t_shell *shell)
{
	t_hist *tmp;

	tmp = shell->tmp;
	if (shell->k_up_set == 0)
	{
		while (tmp->next)
			tmp = tmp->next;
		if (tmp->line != NULL)
			shell->tmp_line = ft_strdup(tmp->line);
		else
		{
			shell->tmp_line = NULL;
			shell->line = NULL;
			shell->cmd = NULL;
		}
		return;
	}
	else if (shell->k_up_set == 1)
	{
		shell->k_up_set = 0;
		while (tmp && tmp->pos != shell->k_up_set)
			tmp = tmp->next ;
		if (tmp->line == NULL)
			shell->tmp_line = NULL;
		else
			shell->tmp_line = ft_strdup(tmp->line);
	}
	else
	{
		shell->k_up_set--;
		while (tmp && tmp->pos != shell->k_up_set)
			tmp = tmp->next;
		shell->tmp_line = ft_strdup(tmp->line);
	}

}



void		key_back(t_command **cmd, t_shell *shell)
{
	t_command *tmp;
	t_command *tmp2;

	tmp = *cmd;
	if (tmp)
	{
		while (tmp && tmp->pos != 1)
			tmp = tmp->next;
		if (tmp->prev  == NULL || tmp->prev->prev == NULL)
		{
			if (tmp->prev == NULL)
				return;
			else
			{
				if (tmp->next)
				{
					*cmd = tmp;
					(*cmd)->prev = NULL;
					//	add_cmd_to_shell(shell);
				}
				else
				{
					shell->line = NULL;
					*cmd = NULL;
				}
			}
		}
		else if (tmp->prev->prev)
		{
			tmp2 = tmp->prev->prev;
			tmp2->next = tmp;
			free(tmp->prev);
			tmp->prev = tmp2;
		}
	}
}

void		key_cleft(t_command *cmd)
{
	t_command *tmp;
	t_command *tmp2;
	int			i;

	i = 0;
	tmp = cmd;
	while (tmp && tmp->pos != 1)
		tmp = tmp ->next;
	tmp2 = tmp;
	while (tmp)
	{
		if (tmp->c == ' ')
		{
			i++;
			while (tmp && tmp->c == ' ' && i != 2)
				tmp = tmp->prev;
		}
		if (i == 2 || tmp->prev == NULL)
		{
			tmp2->pos = 0;
			if (i == 2)
				tmp->next->pos = 1;
			else
				tmp->pos = 1;
			return ;
		}
		tmp = tmp->prev;
	}
}

void		key_cright(t_command *cmd)
{
	t_command *tmp;
	t_command *tmp2;
	int		i;

	i = 0;
	tmp = cmd;
	while (tmp)
	{
		if (tmp->next == NULL && tmp2 != tmp && i == 1)
		{
			tmp2->pos = 0;
			tmp->pos = 1;
		}
		if (tmp && tmp->next && tmp->c == ' ' && i == 1)
		{
			while (tmp && tmp->c == ' ')
				tmp = tmp->next;
			tmp2->pos = 0;
			tmp->pos = 1;
			return ;
		}
		if (tmp->pos == 1 && i == 0)
		{
			tmp2 = tmp;
			i = 1;
		}
		tmp = tmp ->next;
	}
}

void		key_cup(t_command *cmd, t_shell *shell)
{
	int		len;
	int		j;
	t_command *tmp;
	t_command *tmp2;

	len =0;
	j = 0;
	tmp = cmd;
	tmp2 = tmp;
	while (tmp)
	{
		if (tmp->pos == 1)
			break;
		tmp = tmp->next;
		len++;
	}
	len = len - shell->winsize.ws_col;
	if (len >= 0)
	{
		while (tmp2)
		{
			if (j == len)
			{
				tmp->pos = 0;
				tmp2->pos = 1;
				return;
			}
			j++;
			tmp2 = tmp2->next;
		}
	}
	else
	{
		while (tmp2->prev)
			tmp2 = tmp2->prev;
		tmp->pos = 0;
		tmp2->pos = 1;
	}
}


void		key_cdown(t_command *cmd, t_shell *shell)
{
	int		len;
	int		pos;
	t_command *tmp;

	pos = 0;
	len = cmd_len(cmd);
	tmp = cmd;
	while (tmp)
	{
		if (tmp->pos == 1)
			break;
		pos++;
		tmp = tmp->next;
	}
	if (len > pos + shell->winsize.ws_col)
	{
		tmp->pos = 0;
		pos = 0;
		while (tmp && pos < shell->winsize.ws_col)
		{
			pos++;
			tmp = tmp->next;
		}
		tmp->pos = 1;
	}
	else
	{
		tmp->pos = 0;
		while (tmp->next)
			tmp = tmp->next;
		tmp->pos = 1;
	}
}

void		key_homee(t_shell *shell)
{
	t_command *tmp;

	tmp = shell->cmd;
	while (tmp)
	{
		if (tmp->pos == 1)
			break;
		tmp = tmp->next;
	}
	tmp->pos = 0;
	shell->cmd->pos = 1;
}

void		key_endd(t_shell *shell)
{
	t_command *tmp;

	tmp = shell->cmd;
	while (tmp->next)
	{
		if (tmp->pos == 1)
			tmp->pos = 0;
		tmp = tmp->next;
	}
	tmp->pos = 1;
}

