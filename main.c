#include "minishell.h"
#include <unistd.h>

int			find_nb_delim(char *str, char delim)
{
	int		i;
	int		res;

	i = 0;
	res = 1;
	while (str[i])
	{
		if (str[i] == delim)
			res++;
		i++;
	}
	return (res);
}


void		take_winsize(t_shell *shell)
{
	ioctl(0, TIOCGWINSZ, &(shell->winsize));
}


char	**tabcpy(char **src)
{
	int		i;
	char	**dst;

	i = 0;
	while (src[i])
		i++;
	dst = malloc(sizeof(i));
	i = 0;
	while (src[i])
	{
		dst[i] = ft_strdup(src[i]);
		i++;
	}
	dst[i] = NULL;
	return (dst);
}



char		**add_str_to_tab(char **tabb, char *str)
{
	int		i;
	char **dst;

	i = 0;
	dst = tabcpy(tabb);
	str = ft_strjoin("/", str);
	while (dst[i])
	{
		dst[i] = ft_strjoin(dst[i], str);
		i++;
	}
	return (dst);
}


char		**str_to_wordtab(char *str, char delim)
{
	int		i;
	int		j;
	int		k;
	char	**res;

	i = 0;
	j = 0;
	k = 0;
	if (!str)
		return (NULL);
	if (!(res = malloc((find_nb_delim(str, delim) + 1) * sizeof(char*))))
		return (NULL);
	while (str[i])
	{
		if (str[i] == delim || str[i] == '\0')
		{
			if (i != j)
			{
				res[k] = ft_strsub(str, j, i - j);
				k++;
				j = i + 1;
			}
		}
		i++;
	}
	if (str[i] == '\0' && j != i)
	{
		res[k] = ft_strsub(str, j, i - j);
		k++;
	}
	res[k] = NULL;
	return (res);
}


void		print_tab(char **tabb)
{
	int i;

	i = 0;
	while (tabb[i] != NULL)
	{
		ft_putendl(tabb[i]);
		i++;
	}
}

char		*find_access(t_list **tabb, char *line)
{
	char	*tmp;

	tmp = get_bin_hash(tabb, line);
	if (tmp)
	return (ft_strdup(tmp));
	else
	return (NULL);
}

void			free_tab(char **str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
}

char		*find_access_char(char *dir, char *bin)
{

	ft_putendl_fd("aces2", 0);

	dir = ft_strjoin(dir, "/");
	dir = ft_strjoin(dir, bin);
	dir = remove_double(dir, '/');
	if (access(dir, F_OK) != -1)
		return (ft_strdup(dir));
	else
	{
		ft_putendl_fd(strerror(errno), 0);
		return (NULL);
	}
}

void		kill_child(pid_t pid)
{
	kill(pid, SIGTERM);
}

void		take_sigint(int sig)
{
	(void)sig;
}

void my_fork(t_shell *shell, t_arg *arg)
{
	pid_t pid;
	int		status;

	signal(SIGINT, take_sigint);
	pid = fork();
	if (pid == 0)
	{
		if (arg->path == NULL)
		{
			ft_putendl("Command no found!!!");
			exit(1);
		}
		if (arg->chev)
		{
			if (arg->chev->set.out != 0)
				redirect(arg->chev);
			if (arg->chev->set.in != 0)
				redirect_in(arg->chev);
			if (arg->chev->set.err != 0)
				redirect_err(arg->chev);
		}
		execve(arg->path, arg->argv, shell->envp);
		if (arg->chev)
			redirect_i(arg->chev);
	}
	else if (pid < 0)
		return ;
	else
	{	
		waitpid(pid, &status, WCONTINUED);
		if (WIFEXITED(status) || WIFSIGNALED(status))
			kill_child(pid);
	}
}

int			nb_args(char **fct)
{
	int		i;

	i = 0;
	while (fct[i])
		i++;
	return (i);
}



int		is_builtin(t_shell *shell, t_arg *arg)
{
	t_arg	*t;

	t = arg;
	if (ft_strequ(t->argv[0], "setenv") || ft_strequ(t->argv[0], "export"))
	{
		if (nb_args(t->argv) < 4)
			shell->envp = set_env(shell->envp, t->argv[1], t->argv[2]);
		else
			ft_putendl("setenv VAR_NAME VAR_VALUE");
		return (1);
	}
	if (ft_strequ(t->argv[0], "unset"))
	{
		if (!t->argv[1])
			ft_putendl("unset VAR_NAME");
		else
			shell->envp = unset_env(shell->envp, t->argv[1]);
		return (1);
	}
	if (ft_strequ(t->argv[0], "cd"))
	{
		cd_builtin(shell, t);
		return (1);
	}
	if (ft_strequ(t->argv[0], "exit"))
	{
		exit_builtin(shell);
	}
	if (ft_strequ(t->argv[0] , "echo"))
	{
		if (!t->argv[1])
			ft_putendl("echo message/$VAR_NAME");
		else
			echo_builtin(shell, t);
		return (1);
	}
	if (ft_strequ(t->argv[0], "history"))
	{
		show_history(shell->history);
		return (1);
	}
	return (0);
}


void		exec_cmds(t_shell *shell, t_arg *arg)
{
	if (is_builtin(shell, arg) == 1)
		return;
	else
		my_fork(shell, arg);

}

int		cp_list(t_command *t)
{
	int		i;

	i = 0;
	while (t->next)
	{
		i++;
		t = t->next;
	}
	return (i);
}

char		*lst_to_line(t_shell *shell)
{
	char		*tmp;
	int		len;
	int		i;
	t_command *command;

	i = 0;
	len = cp_list(shell->cmd);
	tmp = malloc((len + 2 )* sizeof(char));
	command = shell->cmd;
	while (command)
	{
		tmp[i] = command->c;
		i++;
		command = command->next;
	}
	tmp[i] = '\0';
	return (tmp);
}

void	metachar_all(t_arg *arg, t_shell *shell)
{
	t_arg *tmp;



	if (arg)
	{
		while (arg)
		{
			if (arg->path && arg->argv)
				arg->argv = metachar(arg->argv, shell);
			if (arg->pipe)
			{
				tmp = arg->pipe;
				while (tmp)
				{
					if (tmp->argv)
						tmp->argv = metachar(tmp->argv, shell);
					tmp = tmp->next;
				}
			}
			arg = arg->next;
		}
	}
}

int		cmd_chr(t_command *cmd, char c)
{
	t_command *tmp;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->c == c)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}


int		cmd_quote(t_command *cmd)
{
	while (cmd)
	{
		if (cmd->c == 34)
			return (1);
		else if (cmd->c == 39)
			return (0);
		cmd = cmd->next;
	}
	return (3);
}


int		main(int ac, char **av)
{
	char		*line;
	t_shell		*shell;
	extern char **environ;
	t_arg		*tmp;

	if (ac != 1)
		return (0);
	else if (av[1] != NULL)
		return (0);
	shell = malloc(sizeof(t_shell));
	line = get_env(environ, "PATH");
	shell->dir_bin = str_to_wordtab(line, ':');
	shell->envp = environ;
	init_term(shell);
	take_winsize(shell);
	shell->cpy_line = NULL;
	shell->bin_hash = create_hash(shell->dir_bin);
	while (1)
	{
		signal_handler();
		shell->line = NULL;
		shell->cmd = NULL;
		shell->k_up_set = 0;
		shell->tmp_line = NULL;
		shell->arg = NULL;
		get_key(shell);
		if (shell->line != NULL && shell->cmd != NULL)
		{
			if ((cmd_chr(shell->cmd, '\n') == 1))
			{
				del_quote(shell, cmd_quote(shell->cmd));
				show_quote(shell, cmd_quote(shell->cmd), 0);
			}
			else
			{
				del_line(shell);
				show_line(shell->cmd , 0);
			}
			line = ft_strdup(shell->line);
			//get_next_line(0, &line);
			//fct = str_to_wordtab(line , ' ');
			//add_str_to_tab(tab, fct[0]);
			//		shell->fct = str_to_wordtab(line, ' ');
//			ft_putendl_fd("parseur:", 0);
			shell->arg = ft_parser2(shell, shell->line);
			add_history(&shell->history, line);

			metachar_all(shell->arg, shell);
//			print_arg(shell->arg);
//			ft_putendl_fd("command", 0);
			signal_out();
			process_term(shell);
			if (shell->arg)
			{
				tmp = shell->arg;
				while (tmp)
				{
					if (tmp->token == '|')
						ft_pipe(shell, tmp->pipe);
					else
						exec_cmds(shell, tmp);
					tmp = tmp->next;
				}
				tmp = NULL;
			}
			free_command(&(shell->cmd));
		}
		else
			ft_putendl_fd(":>", 0);
		update_term(shell);
		//free_tab(fct);
		signal_handler();
		update();
	}
	exit_term(shell);
	return (0);
}
