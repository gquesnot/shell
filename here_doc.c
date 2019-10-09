#include "minishell.h"

char		*remove_tab(char *line)
{
	
	return (line);
}


int		write_in_file(char *eof, int set)
{
	int		fd;
	char	*line;
	char	c;

	c = '\n';
	fd = open("/tmp/tmp.tmp", O_CREAT | O_TRUNC | O_RDWR);
	if (set == 1)
		ft_putstr_fd("heredocd> ", 0);
	else
		ft_putstr_fd("heredoc> ", 0);
	get_next_line(0, &line);
	while ((ft_strcmp(line,  eof)) != 0)
	{
		if (set == 1)
			line = remove_tab(line);
		write(fd, line, ft_strlen(line));
		write(fd, &c, 1);
		if (set == 1)
			ft_putstr_fd("heredocd", 0);
		else
			ft_putstr_fd("heredoc> ", 0);
		get_next_line(0, &line);

	}
	close(fd);
	open("/tmp/tmp.tmp", O_CREAT | O_RDWR);
	return (fd);
}

