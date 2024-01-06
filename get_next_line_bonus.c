/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgiovann <pgiovann@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:56:26 by pgiovann          #+#    #+#             */
/*   Updated: 2024/01/05 17:51:16 by pgiovann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

//	Reads the file, stores string in buffer and joins it to the stash

static int	read_buffer(int fd, char **stash, char *buffer)
{
	int		bytes;
	char	*temp;

	ft_memset(buffer, 0, BUFFER_SIZE + 1);
	bytes = read(fd, buffer, BUFFER_SIZE);
	if (bytes < 0)
	{
		free(*stash);
		*stash = NULL;
		return (-1);
	}
	if (bytes == 0)
		return (bytes);
	temp = ft_strjoin(*stash, buffer);
	if (!temp)
	{
		free(*stash);
		*stash = NULL;
		return (-1);
	}
	free(*stash);
	*stash = temp;
	return (bytes);
}

// Extracts a line of text from the stash and stores it in a new string

static void	get_result(char **stash, char **result)
{
	char	*newline;
	size_t	len;
	size_t	i;

	newline = ft_strchr(*stash, '\n');
	if (newline)
		len = newline - *stash + 2;
	else
		len = ft_strlen(*stash) + 1;
	*result = malloc(len);
	if (!*result)
		return ;
	i = 0;
	while (i < len - 1)
	{
		(*result)[i] = (*stash)[i];
		i++;
	}
	(*result)[i] = '\0';
}

//	Removes the string obtained in get_result from the stash

static void	remove_result(char **stash)
{
	char	*newline;
	char	*temp;
	size_t	i;
	size_t	j;

	newline = ft_strchr(*stash, '\n');
	if (!newline)
	{
		free(*stash);
		*stash = NULL;
		return ;
	}
	temp = malloc(ft_strlen(newline) + 1);
	i = 0;
	j = ft_strlen(*stash) - ft_strlen(newline) + 1;
	while (j < ft_strlen(*stash))
		temp[i++] = (*stash)[j++];
	temp[i] = '\0';
	free(*stash);
	*stash = temp;
	if (**stash == 0)
	{
		free(*stash);
		*stash = NULL;
	}
}

// Gets the next line
char	*get_next_line(int fd)
{
	static char	*stash[MAX_FILES_OPENED];
	char		*result;
	char		*buffer;
	int			bytes;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(BUFFER_SIZE + 1);
	bytes = 1;
	while (!ft_strchr(stash[fd], '\n') && bytes > 0)
		bytes = read_buffer(fd, &stash[fd], buffer);
	free(buffer);
	if (bytes == -1)
		return (NULL);
	if (ft_strlen(stash[fd]) == 0)
		return (NULL);
	get_result(&stash[fd], &result);
	remove_result(&stash[fd]);
	return (result);
}
