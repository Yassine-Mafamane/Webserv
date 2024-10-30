/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:08:35 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/29 22:27:03 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <unistd.h>
#include "../Contexts/HttpContext.hpp"
#include "../Utils/helper_functions.hpp"


/// include only needed hraders ! 
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <arpa/inet.h>
#include <map>
#include <netdb.h>

struct SocketListener
{
    /* Only one single socket will be created for each host:port */
    int                                 sock_fd;
    std::vector<const ServerContext&>   servers;
    struct in_addr                      host;   // The host address will be stored in the host representation!
    unsigned short                      port;
};




void    setup_servers(const HttpContext& http_config);


#endif