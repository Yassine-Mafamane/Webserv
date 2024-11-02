/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:08:35 by ymafaman          #+#    #+#             */
/*   Updated: 2024/11/02 19:08:25 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "../Contexts/HttpContext.hpp"
#include "../Utils/utils.hpp"


/// include only needed hraders ! 

#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <arpa/inet.h>
#include <map>
#include <netdb.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>


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

    int     get_sock_fd( void )
    {
        return this->sock_fd;
    }

    void    set_sock_fd( int fd )
    {
        this->sock_fd = fd;
    }

    std::vector<const ServerContext*>&   get_servers( void )
    {
        return this->servers;
    }

    void    set_servers( std::vector<const ServerContext*>& servers )
    {
        this->servers.insert(this->servers.end(), servers.begin(), servers.end());
    }

    void    add_server( const ServerContext* new_server )
    {
        this->servers.push_back(new_server);
    }

private : 

    int                                 sock_fd;
    char                                type;       // 'L' for ListenerSocket | 'C' for client socket
    std::vector<const ServerContext*>   servers;
};

struct ListenerSocket : public Socket
{
    struct in_addr                      host;       // In the host representation
    unsigned short                      port;       // In the host representation

    ListenerSocket& get_instance( void )
    {
        return *this;
    }
};

struct ClientSocket : public Socket
{
    ClientSocket& get_instance( void )
    {
        return *this;
    }
};


void    setup_servers(const HttpContext& http_config, std::vector<struct ListenerSocket>&  activeListners);
int     create_kqueue( void );
void    register_listeners_in_kqueue(int kqueue_fd, std::vector<struct ListenerSocket> & activeListners);
void    poll_events(int kqueue_fd, std::vector<struct ListenerSocket> & activeListners);

#endif