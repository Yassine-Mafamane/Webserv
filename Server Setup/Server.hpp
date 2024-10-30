/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:08:35 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/30 18:38:22 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <unistd.h>
#include <stdio.h>
#include "../Contexts/HttpContext.hpp"
#include "../Utils/helper_functions.hpp"


/// include only needed hraders ! 
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <arpa/inet.h>
#include <map>
#include <netdb.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

#define TMOUT_SEC 1 
#define TMOUT_NSEC 2000 
#define READ_BUFFER_SIZE 1024

struct Socket
{
    void    set_type( char type )
    {
        this->type = type;
    }

    char    get_type( void )
    {
        return this->type;
    }

    virtual Socket* get_instance( void ) = 0;

private : 

    char    type; /* 'L' for ListenerSocket | 'C' for client socket */
};

struct ListenerSocket : public Socket
{
    /* Only one single socket will be created for each host:port */
    int                                 sock_fd;
    std::vector<const ServerContext&>   servers;    // The list of servers that can be used to answer to HTTP requests comming on the socket.
    struct in_addr                      host;       // The host address will be stored in the host representation!
    unsigned short                      port;

    Socket* get_instance( void )
    {
        return this;
    }
};

struct ClientSocket : public Socket
{
    int                                 sock_fd; /* This two attributes can be inherited from the Socket struct */
    std::vector<const ServerContext&>   servers;

    Socket* get_instance( void )
    {
        return this;
    }
};


void    setup_servers(const HttpContext& http_config);
int     create_kqueue( void );
void    register_listeners_in_kqueue(int kqueue_fd, std::vector<struct ListenerSocket&> & activeListners);

#endif