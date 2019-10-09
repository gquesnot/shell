#include "minishell.h"


static void		end(int sig)
{
	t_termios	term;

	(void)sig;
	tcgetattr(0, &term);
	term.c_lflag |= (ICANON | ECHO);
	tcsetattr(0, TCSANOW, &(term));
	tputs(tgetstr("cl", NULL), 1, ft_poutchar);
	tputs(tgetstr("ve", NULL), 1, ft_poutchar);
	//free(&term);
	if ((close(0) == -1))
		ft_putendl_fd("close_fd", 0);
	exit(0);
}


static void		size(int sig)
{
	t_termios	term;

	(void)sig;
	tcgetattr(0, &term);
	tputs(tgetstr("rc", NULL), 1, ft_poutchar);
	tputs(tgetstr("cd", NULL), 1, ft_poutchar);
	tputs(tgetstr("cl", NULL), 1, ft_poutchar);
}

static void		stop(int sig)
{
	t_termios tmp;
	char pause[2];
	
	(void)sig;
	tcgetattr(0, &tmp);
	pause[0] = tmp.c_cc[VSUSP];
	pause[1] = 0;
	tmp.c_lflag |= (ICANON | ECHO);
	tcsetattr(0, TCSANOW, &(tmp));
	tputs(tgetstr("cd", NULL), 1, ft_poutchar);
	tputs(tgetstr("ve", NULL), 1, ft_poutchar);
	tputs(tgetstr("te", NULL), 1, ft_poutchar);
	signal(SIGTSTP, SIG_DFL);
	ioctl(0, TIOCSTI, pause);

}

static void		restart(int sig)
{
	t_termios	tmp;

	(void)sig;
	tcgetattr(0, &tmp);
	tmp.c_lflag &= ~(ICANON | ECHO);
	tmp.c_cc[VMIN] = 1;
	tmp.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &tmp);
	tputs(tgetstr("vi", NULL), 1, ft_poutchar);
	tputs(tgetstr("cl", NULL), 1, ft_poutchar);
	signal(SIGTSTP, stop);
	signal_handler();
}

void		update(void)
{
	tputs(tgetstr("vi", NULL), 1, ft_poutchar);

}
void			signal_out(void)
{
	signal(SIGSEGV, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
	signal(SIGCONT, SIG_DFL);
	signal(SIGWINCH, SIG_DFL);
}

void			signal_handler(void)
{
	if (signal(SIGSEGV, end) == SIG_ERR)
		ft_putstr("\ncant catch SIGSEGV\n");
	if (signal(SIGINT, end) == SIG_ERR)
		ft_putstr("\ncan't catch SIGINT\n");
	if (signal(SIGTSTP, stop) == SIG_ERR)
		ft_putstr("\ncan't catch SIGSTOP\n");
	if (signal(SIGQUIT, end) == SIG_ERR)
		ft_putstr("\ncan't catch SIGQUIT\n");
	if (signal(SIGTERM, end) == SIG_ERR)
		ft_putstr("\ncan't catch SIGTERM\n");
	if (signal(SIGCONT, restart) == SIG_ERR)
		ft_putstr("\ncan't catch SIGCONT\n");
	if (signal(SIGWINCH, size) == SIG_ERR)
		ft_putstr("\ncan't catch SIGWINCH\n");
}
