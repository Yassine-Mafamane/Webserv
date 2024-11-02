/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polling.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:08:50 by ymafaman          #+#    #+#             */
/*   Updated: 2024/11/02 20:09:59 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int create_kqueue( void )
{
    int fd;
    if ((fd = kqueue()) == -1)
        throw std::runtime_error(std::string("Webserv : kqueue failed, reason : ") + strerror(errno)); // TODO : close all sockets on error
    return fd;
}

void delete_client(std::vector<ClientSocket>& activeClients, int fd)
{
    std::vector<ClientSocket>::iterator it = activeClients.begin();
    std::vector<ClientSocket>::iterator end = activeClients.end();

    for ( ; it != end; it++)
    {
        if (it->get_sock_fd() == fd)
        {
            activeClients.erase(it);
            std::cout << "client deleted ! " << std::endl;
            return ;
        }
    }
}

void    register_socket_in_kqueue(int kqueue_fd, Socket * sock_data, short filter)
{
    struct kevent ev;

    ft_memset(&ev, 0, sizeof(struct kevent));
    EV_SET(&ev, sock_data->get_sock_fd(), filter, EV_ADD | EV_ENABLE, 0, 0, (void *) sock_data);

    if (kevent(kqueue_fd, &ev, 1, NULL, 0, NULL) == -1)
        throw std::runtime_error(std::string("Webserv : kevent() failed, reason : ") + strerror(errno));
}

void    register_listeners_in_kqueue(int kqueue_fd, std::vector<struct ListenerSocket> & activeListners)
{
    std::vector<struct ListenerSocket>::iterator it = activeListners.begin();
    std::vector<struct ListenerSocket>::iterator end = activeListners.end();

    for ( ; it != end; it++)
        register_socket_in_kqueue(kqueue_fd, &it->get_instance(), EVFILT_READ);
}

void    accept_client_connection(ListenerSocket *listener, int kqueue_fd, std::vector<ClientSocket>& activeClients)
{
    ClientSocket    new_client;
    int             client_sock_fd;

    if ((client_sock_fd = accept(listener->get_sock_fd(), NULL, 0)) == -1)
        throw std::runtime_error("accept() failed!");

    fcntl(client_sock_fd, F_SETFL, O_NONBLOCK);

    new_client.set_type('C');
    new_client.set_sock_fd(client_sock_fd);
    new_client.set_servers(listener->get_servers());

    activeClients.push_back(new_client);
    register_socket_in_kqueue(kqueue_fd, &(activeClients.back().get_instance()), EVFILT_READ);
}

void    handle_client_request(ClientSocket* client_info)
{
    char    buffer[READ_BUFFER_SIZE];
    size_t  rcvdSize;

    if ((rcvdSize = recv(client_info->get_sock_fd(), (void *) buffer, READ_BUFFER_SIZE - 1, 0)) < 0)
        throw std::runtime_error(std::string("Webserv : recv() failed, reason : ") + strerror(errno));
    buffer[rcvdSize] = '\0';

    std::cout << buffer << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
}

void    respond_to_client(ClientSocket* client_info)
{
    std::string rsp;

    rsp += "HTTP/1.1 200 OK\r\n";
    rsp += "Server: WebServer\r\n";
    rsp += "Content-Type: text/html\r\n";
    rsp += "Content-Length: 3\r\n";
    rsp += "Connection: keep-alive\r\n";
    rsp += "\r\n";
    rsp += "123";

    if (send(client_info->get_sock_fd(), (void *) rsp.c_str(), rsp.length(), 0) == -1)
        throw std::runtime_error("send failed");
}

void    switch_interest(ClientSocket* client_info, int kqueue_fd,short old_filter, short new_filter)
{
    struct kevent   ev;

    /* Deleting old event, event filter might be either EVFILT_READ or EVFILT_WRITE */
    ft_memset(&ev, 0, sizeof(struct kevent));
    EV_SET(&ev, client_info->get_sock_fd(), old_filter, EV_DELETE, 0, 0, NULL);
    if (kevent(kqueue_fd, &ev, 1, NULL, 0, NULL) == -1)
        throw std::runtime_error("kevent() failed!");

    /* Setting the new event we are interested in */
    ft_memset(&ev, 0, sizeof(struct kevent));
    EV_SET(&ev, client_info->get_sock_fd(), new_filter, EV_ADD | EV_ENABLE, 0, 0, (void *) client_info);
    if (kevent(kqueue_fd, &ev, 1, NULL, 0, NULL) == -1)
        throw std::runtime_error("kevent() failed!");
}

void    poll_events(int kqueue_fd, std::vector<struct ListenerSocket> & activeListners)
{
    std::vector<ClientSocket>   activeClients;
    struct kevent               events[100]; // TODO
    int                         n_events;

    while (true)
    {
        ft_memset(events, 0, sizeof(events));

        if ((n_events = kevent(kqueue_fd, NULL, 0, events, 100, 0)) == -1)
        {
            close_sockets_on_error(activeListners);
            close_client_sockets_on_error(activeClients);
            throw std::runtime_error(std::string("Webserv : kevent() failed, reason : ") + strerror(errno)); 
        }

        for (int i = 0; i < n_events; i++)
        {
            if((events[i].flags & EV_EOF || events[i].flags & EV_ERROR)
             || (events[i].fflags & EV_EOF || events[i].fflags & EV_ERROR))
            {
                std::cout << "Client disconected!" << std::endl;
                delete_client(activeClients, events[i].ident);
                close(events[i].ident); // The events related to the socket closed get romevoed automatically from the kqueue.
            }
            else if (((Socket *) events[i].udata)->get_type() == 'L')
            {
                ListenerSocket * listener = (ListenerSocket *) events[i].udata;

                try
                {
                    accept_client_connection(listener, kqueue_fd, activeClients);
                }
                catch(const std::exception& e)
                {
                    close_sockets_on_error(activeListners);
                    close_client_sockets_on_error(activeClients);
                    throw ;
                }                
            }
            else if (((Socket *) events[i].udata)->get_type() == 'C' && events->filter == EVFILT_READ)
            {
                ClientSocket  * client_info = (ClientSocket *) events[i].udata;

                try
                {
                    handle_client_request(client_info);
                    switch_interest(client_info, kqueue_fd, EVFILT_READ, EVFILT_WRITE); // Interest is gonna be switched only if the request has been entirely rood
                }
                catch(const std::exception& e)
                {
                    close_sockets_on_error(activeListners);
                    close_client_sockets_on_error(activeClients);
                    throw ;
                }
            }
            else if (((Socket *) events[i].udata)->get_type() == 'C' && events->filter == EVFILT_WRITE)
            {
                ClientSocket  * client_info = (ClientSocket *) events[i].udata;

                try
                {
                    respond_to_client(client_info);
                    switch_interest(client_info, kqueue_fd, EVFILT_WRITE, EVFILT_READ); // Interest is gonna be switched only if the response has been entirely sent.
                }
                catch(const std::exception& e)
                {
                    close_sockets_on_error(activeListners);
                    close_client_sockets_on_error(activeClients);
                    throw ;
                }
            }
        }
    }   
}