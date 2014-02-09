/*
** get_next_line.c for gnl in /home/chapui_s/travaux/get_next_line/
**
** Made by chapui_s
** Login   <chapui_s@epitech.net>
**
** Started on  Wed Nov 20 21:02:19 2013 chapui_s
** Last update Sun Feb  9 17:37:26 2014 chapui_s
*/

#include <unistd.h>
#include <stdlib.h>
#include "get_next_line.h"

t_fd	*find_fd_or_create_it(t_fd **pile_fd, int fd)
{
  t_fd	*tmp;
  t_fd	*new;

  tmp = *pile_fd;
  if (tmp && tmp->fd == fd)
    return (tmp);
  while (tmp && (tmp = tmp->next) != *pile_fd)
    if (tmp->fd == fd)
      return (tmp);
  new = NULL;
  if ((new = (t_fd*)malloc(sizeof(t_fd))) == NULL)
    return (NULL);
  new->str = NULL;
  new->fd = fd;
  if (*pile_fd)
  {
    new->next = *pile_fd;
    tmp = *pile_fd;
    while (tmp->next != *pile_fd)
      tmp = tmp->next;
    tmp->next = new;
  }
  else
    new->next = new;;
  return (new);
}

int	is_backslash_n(char *str)
{
  int	i;

  i = 0;
  if (str == NULL)
    return (0);
  while (str[i])
  {
    if (str[i] == '\n')
      return (1);
    i = i + 1;
  }
  return (0);
}

int	add_str_to(char **dest, char *src)
{
  char	*tmp;
  char	*tmp2;
  int	i;
  int	j;

  i = 0;
  j = 0;
  src[BUF_SIZE] = '\0';
  while (*dest && (*dest)[i])
    i = i + 1;
  while (src[j++]);
  i = i + j;
  if ((tmp = (char*)malloc(sizeof(char) * (i + 1))) == NULL)
    return (1);
  i = 0;
  while (*dest && (*dest)[i])
    tmp[i] = (*dest)[i++];
  j = 0;
  while (src[j])
    tmp[i++] = src[j++];
  tmp[i] = 0;
  tmp2 = *dest;
  *dest = tmp;
  free(tmp2);
  return (0);
}

char	*return_it(char **str, char **to_return)
{
  int	i;
  int	j;
  char	*new_str;

  i = 0;
  j = 0;
  while ((*str)[i] != '\n')
    i = i + 1;
  while ((*str)[i] && (*str)[i + j])
    j = j + 1;
  if ((*to_return = (char*)malloc(sizeof(char) * (i + 1))) == NULL)
    return (NULL);
  if ((new_str = (char*)malloc(sizeof(char) * (j + 1))) == NULL)
    return (NULL);
  (*to_return)[i] = 0;
  i = 0 ;
  while ((*str)[i] != '\n' && (*str)[i])
    (*to_return)[i] = (*str)[i++];
  new_str[j] = 0;
  j = 0;
  while ((*str)[++i])
    new_str[j++] = (*str)[i];
  new_str[j] = 0;
  free(*str);
  return (new_str);
}

char		*get_next_line(const int fd)
{
  static t_fd	*pile;
  char		*str_tmp;
  char		*to_return;
  int		s_read;

  if ((pile = find_fd_or_create_it(&pile, fd)) == NULL || BUF_SIZE <= 0)
    return (NULL);
  if ((str_tmp = (char*)malloc(sizeof(char) * (BUF_SIZE + 1))) == NULL)
    return (NULL);
  while (is_backslash_n(pile->str) == 0)
  {
    if ((s_read = read(fd, str_tmp, BUF_SIZE)) < 0)
      return (NULL);
    (s_read < BUF_SIZE) ? (str_tmp[s_read] = 0) : (0);
    if (s_read < BUF_SIZE && str_tmp[s_read - 1] != '\n')
      str_tmp[s_read] = '\n';
    if (s_read < BUF_SIZE && str_tmp[s_read - 1] != '\n')
      str_tmp[s_read + 1] = 0;
    if (((add_str_to(&(pile->str), str_tmp)) > 0)
	|| (s_read == 0 && pile->str[0] == '\n'))
      return (NULL);
  }
  free(str_tmp);
  pile->str = return_it(&(pile->str), &to_return);
  return ((pile->str == NULL) ? (NULL) : (to_return));
}
