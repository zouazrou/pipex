/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouazrou <zouazrou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 16:35:15 by zouazrou          #+#    #+#             */
/*   Updated: 2025/04/27 09:11:03 by zouazrou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size);

static void	ft_increment(size_t *v1, size_t *v2)
{
	(*v1)++;
	(*v2)++;
}

static size_t	ft_count_words(char const *s, char c)
{
	size_t	i;
	size_t	count;
	size_t	lens;
	size_t	inword;

	i = 0;
	count = 0;
	lens = ft_strlen(s);
	inword = 0;
	while (i < lens)
	{
		if (inword == 0)
		{
			if (s[i] != c)
				ft_increment(&count, &inword);
		}
		else
		{
			if (s[i] == c || !s[i])
				inword = 0;
		}
		i++;
	}
	return (count);
}

static void	ft_skip_c(char const *s, char c, size_t *w)
{
	while (s[*w] == c)
		(*w)++;
}

char	**ft_split(char const *s, char c)
{
	char	**p;
	size_t	countwords;
	size_t	i;
	size_t	w;
	size_t	lenw;

	if (!s)
		return (NULL);
	countwords = ft_count_words(s, c);
	p = malloc((countwords + 1) * sizeof(char *));
	w = 0;
	i = 0;
	if (!p)
		return (NULL);
	while (i < countwords)
	{
		lenw = 0;
		ft_skip_c(s, c, &w);
		while (s[w] != c && s[w])
			ft_increment(&w, &lenw);
		*(p + i) = malloc(lenw + 1);
		ft_strlcpy(*(p + i++), &s[w - lenw], lenw + 1);
	}
	*(p + i) = NULL;
	return (p);
}
