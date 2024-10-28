/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:07:44 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/28 22:10:13 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Contexts/HttpContext.hpp"
#include "../Utils/helper_functions.hpp"

/// include only needed hraders ! 
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <arpa/inet.h>
#include <map>
#include <netdb.h>


static bool already_binded(const std::multimap<struct sockaddr, unsigned short>& sock_addr, struct sockaddr ai_addr, unsigned short port)
{
    std::pair<std::multimap<struct sockaddr, unsigned short>::const_iterator,std::multimap<struct sockaddr, unsigned short>::const_iterator>   range = sock_addr.equal_range(ai_addr);
    std::multimap<struct sockaddr, unsigned short>::const_iterator                                                                             it = range.first;


    for (it = sock_addr.begin(); it != sock_addr.end(); it++)
    {
        if (it->second == port)
            return true;
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
        err_throw(gai_strerror(ec));
    }

    return res;
}


std::vector<int>    initialize_sockets_on_port(struct addrinfo *list, std::multimap<struct sockaddr, unsigned short>& sock_addr,unsigned short port) // sockets with s because for every address in the list a socket will be created!
{
    std::vector<int>    s_fds;
    int                 fd;
    unsigned int        n_sock = 0; // number of sockets;
    const char          *cause = NULL;
    struct sockaddr_in  s_info;
    
    
    for ( ; list ; list = list->ai_next) // change that list to give a meaning to a single node!
    {
        if (already_binded(sock_addr, *(list->ai_addr), port)) // this step has to be done in order to link other servers to the same socket.
        {
            // link the current server with the socket !
            continue ;
        }

        fd = socket(list->ai_family, list->ai_socktype, list->ai_protocol);
        if (fd == -1)
        {
            cause = "socket";
            continue ;
        }

        ft_memset(&s_info, 0, sizeof(s_info));
        
        s_info.sin_family = list->ai_family;
        s_info.sin_addr.s_addr = htonl(((struct sockaddr_in *) list)->sin_addr.s_addr); // a question to answer : what is the relation between the two structs ?
        s_info.sin_port = htons(port);
        if (bind(fd, (struct sockaddr *) &s_info, sizeof(s_info)) == -1)
        {
            cause = "bind";
            continue ;
        }

        n_sock++;

        s_fds.push_back(fd);

        // maby listen on the created sockets here
    }
    
}


void    setup_servers(const HttpContext& http_config)
{
    std::multimap<struct sockaddr, unsigned short>  sock_addr;  // this represents a multimap of host:port list already binded with a socket
    struct addrinfo                                 *res;
    std::vector<int>                                s_fds, new_fds;      // sockets fds, this is gonna be changed and it will be remplaced with a data structer that holds the sockets and the servers related to it. new fds represent the newly created sockets for a single port
    int                                             nsock;      // number of sockets successfully created

    // A socket will be defined with it s host::port thus theire should be a struct 

    std::vector<ServerContext>::const_iterator it = http_config.get_servers().begin();
    std::vector<ServerContext>::const_iterator end = http_config.get_servers().end();    

    for ( ; it < end; it++)
    {
        res = my_get_addrinfo(it->get_host().c_str());
        // try to bind every interface ip in the list with the port given and add it to the sick addr mulimap ...

        // A problem might occure where in the configuration file a host was provided with it s name like my_host for example and in an other server the host was provided as an ip address which refere to the same first host my_host. // this problem wont occure if a bind sockets to each element returned by the getaddrinfo

        // loop on all server ports and do the following :
        new_fds = initialize_sockets_on_port(res, sock_addr, port);
            // add the new_fds to the s_fds vector

        freeaddrinfo(res);
    }

    
}