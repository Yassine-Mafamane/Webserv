/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:07:44 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/30 14:06:06 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

static bool already_binded(std::vector<struct SocketListener>& active_sockets, const ServerContext& server, struct in_addr host, unsigned short port)
{
    std::vector<struct SocketListener>::iterator  it = active_sockets.begin();
    std::vector<struct SocketListener>::iterator  end = active_sockets.end();

    for ( ; it != end; it++)
    {
        if ((it->host.s_addr == host.s_addr) && (it->port == port))
        {
            it->servers.push_back(server); // link the server to the socket already created for this host:port
            return true;
        }
    }
    return false;
}

struct addrinfo *my_get_addrinfo(const char * host)
{
    int             ec;             // error code;
    struct addrinfo *res;
    struct addrinfo hints;

    ft_memset(&hints, 0,sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((ec = getaddrinfo(host, NULL, &hints, &res)) != 0)
    {
        if (ec == 8)
            err_throw((std::string("Unknown host : ") + host).c_str());
        err_throw(gai_strerror(ec));
    }
    return res;
}


void    initialize_sockets_on_port(struct addrinfo *list, std::vector<struct SocketListener>& active_sockets, const ServerContext server, unsigned short port)
{
    int                 fd;
    unsigned int        n_sock = 0;
    const char          *cause = NULL;
    struct sockaddr_in  s_info;


    for (struct addrinfo *entry = list; entry ; entry = entry->ai_next)
    {
        if (already_binded(active_sockets, server, ((struct sockaddr_in *) entry)->sin_addr, port))
        {
            n_sock++;
            continue ;
        }

        fd = socket(entry->ai_family, entry->ai_socktype, entry->ai_protocol);
        if (fd == -1)
        {
            cause = "socket";
            continue ;
        }

        ft_memset(&s_info, 0, sizeof(s_info));

        s_info.sin_family = entry->ai_family;
        s_info.sin_addr.s_addr = htonl(((struct sockaddr_in *) entry)->sin_addr.s_addr); // a question to answer : what is the relation between the two structs ?
        s_info.sin_port = htons(port);

        if (bind(fd, (struct sockaddr *) &s_info, sizeof(s_info)) == -1)
        {
            close(fd);
            cause = "bind";
            continue ;
        }

        // link the current server to the newlly created socket and add the socket to the active sockets list:
        {
            struct SocketListener   new_s;
            new_s.sock_fd = fd;
            new_s.host = s_info.sin_addr;
            new_s.port = port;
            new_s.servers.push_back(server);

            active_sockets.push_back(new_s);
        }
        n_sock++;

        listen(fd, 128); // This tells the TCP/IP stack to start accept incoming TCP connections on the port the socket is binded to. 128 because in The mac im working on 128 is the maximum number of pending connections
    }

    if (n_sock == 0)
        throw cause;
}


void    setup_servers(const HttpContext& http_config)
{
    std::vector<struct SocketListener>  activeListners;
    struct addrinfo                     *res;

    std::vector<ServerContext>::const_iterator serv_it = http_config.get_servers().begin();
    std::vector<ServerContext>::const_iterator end = http_config.get_servers().end();    

    for ( ; serv_it < end; serv_it++)
    {
        try
        {
            res = my_get_addrinfo(serv_it->get_host().c_str());
        }
        catch(const std::string& err)
        {
            close_sockets_on_error(activeListners);
            throw err;
        }

        {
            std::vector<unsigned short>::const_iterator ports_it = serv_it->get_ports().begin();
            std::vector<unsigned short>::const_iterator p_end = serv_it->get_ports().end();
            
            for ( ; ports_it < p_end; ports_it++ )
            {
                try
                {
                    initialize_sockets_on_port(res, activeListners, *serv_it, *ports_it);
                }
                catch(const char* err)
                {
                    close_sockets_on_error(activeListners);
                    throw err;
                }
            }
        }
        freeaddrinfo(res);
    }
}