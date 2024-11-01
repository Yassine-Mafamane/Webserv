/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polling.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:08:50 by ymafaman          #+#    #+#             */
/*   Updated: 2024/11/01 22:18:49 by ymafaman         ###   ########.fr       */
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

void    register_listeners_in_kqueue(int kqueue_fd, std::vector<struct ListenerSocket> & activeListners)
{
    std::vector<struct ListenerSocket>::iterator it = activeListners.begin();
    std::vector<struct ListenerSocket>::iterator end = activeListners.end();

    for ( ; it != end; it++)
    {
        std::cout << it->sock_fd << std::endl;
        struct kevent ev;
        EV_SET(&ev, it->sock_fd, EVFILT_READ, EV_ADD, 0, 0, (void *) &(it->get_instance()));
        if (kevent(kqueue_fd, &ev, 1, NULL, 0, NULL) == -1)
        {
            close_sockets_on_error(activeListners);
            throw std::runtime_error(std::string("Webserv : kevent() failed, reason : ") + strerror(errno));
        }
    }
}

void    register_client_in_kqueue(int kqueue_fd, ClientSocket& client_info)
{
    /* When the client connects to the server the first time, he will be added to the Read events in the Kqueue in order to get his requests. */
    struct kevent ev;
    ft_memset(&ev, 0, sizeof(struct kevent));
    EV_SET(&ev, client_info.sock_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, (void *) &(client_info.get_instance()));

    if (kevent(kqueue_fd, &ev, 1, NULL, 0, NULL) == -1)
        throw std::runtime_error(std::string("Webserv : kevent() failed, reason : ") + strerror(errno));
}

void    set_new_client_info(int kqueue_fd, std::vector<ClientSocket>& activeClients, std::vector<const ServerContext *>& servers_list, int client_sock_fd)
{
    ClientSocket new_client;
    
    fcntl(client_sock_fd, F_SETFL, O_NONBLOCK);

    new_client.set_type('C');
    new_client.sock_fd = client_sock_fd;
    new_client.servers.insert(new_client.servers.end(), servers_list.begin(), servers_list.end());
    
    activeClients.push_back(new_client);

    register_client_in_kqueue(kqueue_fd, activeClients.back()); // Dont use the new_client variable because by storing it s address we'll get gar value after it gets destructed.
}

void    poll_events(int kqueue_fd, std::vector<struct ListenerSocket> & activeListners)
{
    std::vector<ClientSocket>   activeClients;
    struct timespec             timeout;
    struct kevent               events[100]; // TODO
    int                         n_events;
    int                         client_sock_fd;

    timeout.tv_sec = TMOUT_SEC;
    timeout.tv_nsec = TMOUT_NSEC;

    while (true)
    {
        n_events = kevent(kqueue_fd, NULL, 0, events, 100, 0);
        if (n_events == -1)
        {
            close_sockets_on_error(activeListners);
            close_client_sockets_on_error(activeClients);
            throw std::runtime_error(std::string("Webserv : kevent() failed, reason : ") + strerror(errno)); 
        }

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

                client_sock_fd = accept(sock_data->sock_fd, NULL, 0);

                try
                {
                    set_new_client_info(kqueue_fd, activeClients, sock_data->servers, client_sock_fd);
                }
                catch(const std::exception& e)
                {
                    close_sockets_on_error(activeListners);
                    close_client_sockets_on_error(activeClients);
                    throw ;
                }
            }
            else if (((Socket *) events[i].udata)->get_type() == 'C' && events[i].filter == EVFILT_WRITE)
            {
                ClientSocket  * sock_data = (ClientSocket *) events[i].udata;
                std::string     response;

                ft_memset(&response, 0, sizeof(response));
                response += "HTTP/1.1 200 OK\r\n";
                response += "Content-Type: text/*\r\n";
                response += "Content-Length: 10\r\n";
                response += "\r\n";
                response += "ABCDEFGHIJ\r\n";

                if (send(sock_data->sock_fd, (const void *) response.c_str(), response.length(), 0) == -1)
                    throw std::runtime_error("send failed");

                // enable the read event again and delete the write one for this client from the kqueue.
                struct kevent ev;
                ft_memset(&ev, 0, sizeof(struct kevent));
                EV_SET(&ev, sock_data->sock_fd, EVFILT_WRITE, EV_DELETE, 0, 0, 0);
                if (kevent(kqueue_fd, &ev, 1, NULL, 0, NULL) == -1)
                {
                    close_sockets_on_error(activeListners);
                    close_client_sockets_on_error(activeClients);
                    throw std::runtime_error(std::string("Webserv : kevent(1) failed, reason : ") + strerror(errno));
                }
                ft_memset(&ev, 0, sizeof(struct kevent));
                EV_SET(&ev, sock_data->sock_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, events[i].udata);
                if (kevent(kqueue_fd, &ev, 1, NULL, 0, NULL) == -1)
                {
                    close_sockets_on_error(activeListners);
                    close_client_sockets_on_error(activeClients);
                    throw std::runtime_error(std::string("Webserv : kevent(2) failed, reason : ") + strerror(errno));
                }
            }
            else if (((Socket *) events[i].udata)->get_type() == 'C' &&  events[i].filter == EVFILT_READ)
            {
                ClientSocket  * sock_data = (ClientSocket *) events[i].udata;
                char            buffer[READ_BUFFER_SIZE];
                size_t          rcvdSize = 0;

                if ((rcvdSize = recv(sock_data->sock_fd, (void *) buffer, READ_BUFFER_SIZE - 1, 0)) < 0)
                {
                    close_sockets_on_error(activeListners);
                    close_client_sockets_on_error(activeClients);
                    throw std::runtime_error(std::string("Webserv : recv() failed, reason : ") + strerror(errno));
                }
                buffer[rcvdSize] = '\0';
                std::cout << buffer << std::endl;
                std::cout << "---------------------------------------------------" << std::endl;

                // Change the event that we are interested on for this client's socket from reading to writing since we got a request so now we need to send a response. And thats what a webserver is about a request -> response
                struct kevent ev;
                ft_memset(&ev, 0, sizeof(struct kevent));
                EV_SET(&ev, events[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                if (kevent(kqueue_fd, &ev, 1, NULL, 0, NULL) == -1)
                {
                    close_sockets_on_error(activeListners);
                    close_client_sockets_on_error(activeClients);
                    throw std::runtime_error(std::string("Webserv : kevent(3) failed, reason : ") + strerror(errno));
                }
                ft_memset(&ev, 0, sizeof(struct kevent));
                EV_SET(&ev, events[i].ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, (void *) &sock_data->get_instance());
                if (kevent(kqueue_fd, &ev, 1, NULL, 0, NULL) == -1)
                {
                    close_client_sockets_on_error(activeClients);
                    close_sockets_on_error(activeListners);
                    throw std::runtime_error(std::string("Webserv : kevent(4) failed, reason : ") + strerror(errno));
                }
            }
        }
    }
}