/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 21:38:05 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/29 21:14:17 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "helper_functions.hpp"

bool is_all_digits(const std::string& str)
{
    for (size_t i = 0; i < str.length(); i++)
    {
        if (!std::isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

bool    is_space(char c)
{
    return (c == 32 || (c >= 9 && c <= 13));
}

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned int	i;

	i = 0;
	while (i < len)
	{
		*((unsigned char *)(b + i)) = (unsigned char) c;
		i++;
	}
	return (b);
}

void    err_throw( const char * message )
{
    throw (std::string("Webserv : ") + message).c_str();
}

void    close_sockets_on_error(std::vector<struct SocketListener>& active_socks)
{
    std::vector<struct SocketListener>::iterator it = active_socks.begin();
    std::vector<struct SocketListener>::iterator end = active_socks.end();

    for ( ; it != end; it++ )
    {
        close(it->sock_fd);   
    }
}