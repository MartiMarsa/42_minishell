/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarsa-s <mmarsa-s@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 11:59:29 by mmarsa-s          #+#    #+#             */
/*   Updated: 2023/05/09 11:59:31 by mmarsa-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

/*
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	char	*op;
	size_t	i;

	op = (char *)s;
	i = 0;
	if (start >= ft_strlen((char *)s))
	{
		return (ft_strdup(""));
	}
	if (len <= ft_strlen(op) - (size_t)start)
		substr = malloc(len + 1);
	else
		substr = malloc(ft_strlen(op) - (size_t)start + 1);
	if (!substr)
		return (NULL);
	s += start;
	while (i < len && *s)
	{
		substr[i] = *s;
		i++;
		s++;
	}
	substr[i] = '\0';
	return (substr);
}*/

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	len_str;
	size_t	i;

	if (!s)
		return (0);
	len_str = ft_strlen(s);
	if (start > len_str)
		return (ft_strdup(""));
	if ((start + len) > len_str)
		len = len_str - start;
	substr = malloc(sizeof(char) * (len + 1));
	if (!substr)
		return (NULL);
	i = 0;
	while (i < len && s[i])
	{
		substr[i] = s[i + start];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

/*int	main(void)
{
	const char s[] = "Somos perroflautas";
	unsigned int	start;
	size_t	len;
	char	*ptr;

	start = 10;
	len = 20;
	ptr = ft_substr(s, start, len);
	printf("%p\n", ptr);
	return (0);
}*/