/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polling.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:08:50 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/30 18:41:47 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int create_kqueue( void )
{
    if (kqueue() == -1)
        throw std::runtime_error(std::string("Webserv : kqueue failed, reason : ") + strerror(errno)); // TODO : close all sockets on error
}

void    register_listeners_in_kqueue(int kqueue_fd, std::vector<struct ListenerSocket&> & activeListners)
{
    std::vector<struct ListenerSocket&>::iterator it = activeListners.begin();
    std::vector<struct ListenerSocket&>::iterator end = activeListners.end();

    for ( ; it != end; it++)
    {
        struct kevent ev;
        EV_SET(&ev, it->sock_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, (void *) it->get_instance());
        if (kevent(kqueue_fd, &ev, 1, NULL, 0, NULL) == -1)
            throw std::runtime_error(std::string("Webserv : kevent() failed, reason : ") + strerror(errno)); // TODO : close all sockets on error
    }
}

void    poll_events(int kqueue_fd)
{
    std::vector<ClientSocket>   activeClients;
    struct timespec             timeout;
    struct kevent               events[100]; // TODO
    int                         n_events;
    int                         client_sock_fd;
    char                        buffer[READ_BUFFER_SIZE];

    timeout.tv_sec = TMOUT_SEC;
    timeout.tv_nsec = TMOUT_NSEC;

    while (true)
    {
        n_events = kevent(kqueue_fd, NULL, 0, events, 100, &timeout);
        if (n_events == -1)
            throw std::runtime_error(std::string("Webserv : kevent() failed, reason : ") + strerror(errno)); // TODO : close all sockets on error

        for (int i = 0; i < n_events; i++)
        {
            if(events[i].flags & EV_EOF)
            {
                std::cout << "Client disconected!" << std::endl;
                close(events[i].ident);     /* By doing this the event related to this fd is automatically removed from the queue by the kernel. */
                // Here the client has to be removed from the activeClients list!
            }
            else if (((Socket *) events[i].udata)->get_type() == 'L')
            {
                /* Getting here means that the event is related to a Listener Socket */
                ListenerSocket * sock_data = (ListenerSocket *) events[i].udata;

                // TESTING, the peer_address is not needed the sockaddr * parameter in accept will be NULL.
                struct sockaddr_in  peer_addr;
                socklen_t           peer_addr_len = sizeof(struct sockaddr_in);
                
                client_sock_fd = accept(sock_data->sock_fd, (struct sockaddr *)&peer_addr, &peer_addr_len); /* Accepting incomming client connection. */
                
                std::cout << "Client connected at socket_fd : " << client_sock_fd << " from : " << inet_ntoa(peer_addr.sin_addr) << ":" << ntohs(peer_addr.sin_port) << std::endl;

                /* Constructing new client info, this can be done trough a parametrized constructor */
                ClientSocket new_client;
                new_client.set_type('C');
                new_client.sock_fd = client_sock_fd;
                // new_client.servers.insert(new_client.servers.end(), sock_data->servers.begin(), sock_data->servers.end()); // TODO : Add a method that stores servers
                activeClients.push_back(new_client);
            }
            else if (((Socket *) events[i].udata)->get_type() == 'C' && events[i].flags & EVFILT_READ)
            {
                ClientSocket * sock_data = (ClientSocket *) events[i].udata;
                
                size_t  rcvdSize = 0;
                if ((rcvdSize = recv(sock_data->sock_fd, (void *) buffer, READ_BUFFER_SIZE - 1, 0)) < 0)
                    throw std::runtime_error(std::string("Webserv : recv() failed, reason : ") + strerror(errno)); // TODO : close all sockets on error   
                
                buffer[rcvdSize] = '\0';
                std::cout << buffer;
                
            }
        }
    }
}