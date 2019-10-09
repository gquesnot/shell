#ifndef MINISHELL_H_
# define MINISHEL_H_

# include "./libft/libft.h"
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <unistd.h>
# include <curses.h>
# include <term.h>
# include <sys/ioctl.h>
# include <errno.h>
# include <dirent.h>


typedef struct s_shell t_shell;
typedef struct s_hist t_hist;
typedef struct s_command t_command;
typedef struct termios t_termios;
typedef struct s_arg t_arg;
typedef struct s_chev t_chev;
typedef struct s_coor t_coor;

# define BUFFER *(unsigned int *)buffer;
# define K_CUP 71696882162459
# define K_CDOWN 72796393790235
# define K_CLEFT 74995417045787
# define K_CRIGHT 73895905418011
# define K_DEL 2117294875
# define K_RIGHT 4414235
# define K_LEFT 4479771
# define K_DOWN 4348699
# define K_UP 4283163
# define K_BSP 127
# define K_RET 10
# define K_ESC 27
# define K_SP 32
# define K_TAB 9
# define K_HOME 4741915
# define K_END 4610843
# define K_CPYMODE 25371
# define K_PASTE 30235
# define BLINK "\033[7m"
# define HIGHLIGHT "\033[7m"
# define UNDERLINE "\033[4m"
# define MAGENTA "\033[35m"
# define YELLOW "\033[33m"
# define GREEN "\033[32m"
# define WHITE "\033[37m"
# define RESET "\033[00m"
# define BLACK "\033[30m"
# define BLUE "\033[34m"
# define CYAN "\033[36m"
# define RED "\033[31m"

typedef struct winsize t_win;
typedef struct termios t_tcpy;
typedef struct s_lst t_lst;
typedef struct s_size t_size;

struct s_lst
{
	int				pos;
	int				len;
	int				select;
	char			*name;
	struct s_lst	*next;
	struct s_lst	*prev;
};


struct				s_shell
{
	int				fd;
	t_termios		termios;
	char			**tabb;
	char			**envp;
	char			**fct;
	t_arg			*arg;
	char			*line;
	char			*tmp_line;
	t_command		*tmp_cmd;
	int				k_up_set;
	char			**dir_bin;
	t_list			**bin_hash;
	t_hist		*tmp;
	char			*cpy_line;
	t_hist			*history;
	t_command		*cmd;
	t_shell			*next;
	struct winsize		winsize;
	t_size				*size;
};

struct			s_size
{
	int nb_elem;
	int maxsize;
	int nb_col;
	int nb_row;
	int size_ok;
};


struct			s_hist
{
	int			pos;
	char		*line;
	t_hist		*next;
};

struct			s_command
{
	int c;
	int pos;
	int cpy;
	t_command *next;
	t_command *prev;
};

struct			s_arg
{
	char	*path;
	char	**argv;
	char	token;
	t_chev	*chev;
	t_arg	*pipe;
	t_arg	*next;
	t_arg	*prev;
};

struct			s_coor
{
	int		in;
	int		out;
	int		err;
};


struct			s_chev
{
	char		*str_chev_out;
	char		*str_chev_in;
	char		*str_chev_err;
	t_coor			set;
	int			save_in;
	int			save_out;
	int			save_err;
};


void		update();
char		*ft_straddchr(char *str, char c);
char		*remove_double(char *str, char c);
void		free_command(t_command **command);
char		*get_env(char	**env, char *name);
void		cd_builtin(t_shell *shell, t_arg *arg);
char		**set_env(char **envp, char *elem, char *value);
void		exit_builtin(t_shell *shell);
void		echo_builtin(t_shell *shell, t_arg *arg);
void		print_tab(char **str);
char		**set_env(char **envp, char *elem, char *value);
char		**unset_env(char **envp, char *elem);
void		add_history(t_hist **history, char *line);
void		show_history(t_hist *hist);
void		get_key(t_shell *shell);
void		show_line(t_command *cmd, int set);
void		init_term(t_shell *shell);
void		exit_term(t_shell *shell);

void		key_leftt(t_shell *shell);
void		key_rightt(t_shell *shell);
int			ft_poutchar(int c);
long		key_complete(t_shell *shell);
void		key_back(t_command **command, t_shell *shell);
void		key_uupp(t_shell *shell);
void		key_cleft(t_command *cmd);
void		key_cright(t_command *cmd);
void		key_cup(t_command *cmd, t_shell *shell);
void		key_cdown(t_command *cmd, t_shell *shell);
void		key_endd(t_shell *shell);
void		key_homee(t_shell *shell);

void		key_cpy_right(t_command *cmd);
void		key_cpy_left(t_command *cmd);
void		key_cpy_return(t_shell *shell, t_command *cmd);
void		key_cpymode(t_shell *shell);
void		key_cpy_cleft(t_command *cmd);
void		key_cpy_cright(t_command *cmd);
void		key_insert_cpy(t_shell *shell);

char		**tabcopy(char **str);
char		*add_cmd_to_shell(t_command *cmd);
void		add_command(t_command **cmd, char c);
t_command	*add_line_to_cmd(char *str);

int			hist_len(t_hist *hist);
void		key_downn(t_shell *shell);
t_hist		*new_history(char *line, int set);
void		add_history(t_hist **hist, char *line);
void		key_up_down(t_shell *shell, long k);


void		key_up_comp(t_lst *lst, t_size size);
void		key_down_comp(t_lst *lst, t_size size);
void		key_right_comp(t_lst *lst);
void		key_left_comp(t_lst *lst);
void		key_back_comp(t_lst *lst);
void		key_space_comp(t_lst *lst);
void		key_right_comp(t_lst *lst);
void		key_echap_comp(void);


void		take_winsize(t_shell *shell);
void		del_line(t_shell *shell);
void		signal_handler();
t_shell		*ft_stock(t_shell **shell, int set);
char		*find_access_char(char *dir, char *bin);
char		*find_access(t_list **tabb, char *line);
//t_arg		*ft_parser(t_shell *shell, char *line);
void		ft_pipe(t_shell *shell,t_arg *pip);
void		print_arg(t_arg *arg);
int			is_builtin(t_shell *shell , t_arg *arg);

void		redirect(t_chev *chev);
void		redirect_i(t_chev *chev);
void		redirect_in(t_chev *chev);
void		redirect_err(t_chev *chev);
void		redirect_err_b(t_chev *chev);

void		signal_out(void);
t_list		*get_filename(char *directory, int set);

char		**ft_wildcard(char **fct, int *n);
char		**single_char(char **fct, int *n, char *line, t_list *lst);
char		**ft_wildcard(char **fct, int *n);
char		**metachar(char **fct, t_shell *shell);

void		update_term(t_shell *shhell);
void		process_term(t_shell *shell);

t_arg		*ft_parser2(t_shell *shell, char *line);
int			write_in_file(char *eof, int set);
char		*delete_char(char *str, int pos);

void		quote(t_shell *shell, int quote);
void		key_quote(t_shell *shell, int quote);
void		show_curs(void);
int			quote_open(t_command *cmd);
void		show_quote(t_shell *shell, int set, int show);
void		del_quote(t_shell *shell, int k);

t_command		*cmd_new(char c, int pos);
t_list		**create_hash(char **bin);
char			*get_bin_hash(t_list **bin_hash, char *cmd);
void		lst_add(t_lst **lst, char *s);
t_lst		*lstnew(char *s, int pos);
int			take_winsize_2(t_shell *shell, t_lst *lst);
void		printlst(t_shell *shell, t_lst *lst);
t_command	*cmd_dup(t_command *cmd);
int			cmd_len(t_command *cmd);
void		cmd_insert(t_command *cmd, t_command *new);
void		add_command2(t_command **cmd, t_command *new);
int			if_pos(t_lst *lst);


#endif
