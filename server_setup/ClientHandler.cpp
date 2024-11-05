/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:37:00 by ymafaman          #+#    #+#             */
/*   Updated: 2024/11/05 21:41:52 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void delete_client(std::vector<ClientSocket *>& activeClients, int fd)
{
    std::vector<ClientSocket *>::iterator it = activeClients.begin();
    std::vector<ClientSocket *>::iterator end = activeClients.end();

    for ( ; it != end; it++)
    {
        if ((*it)->get_sock_fd() == fd)
        {
            delete (*it);
            activeClients.erase(it);
            std::cout << "client deleted ! " << std::endl;
            return ;
        }
    }
}

void    accept_client_connection(ListenerSocket *listener, int kqueue_fd, std::vector<ClientSocket*>& activeClients)
{
    ClientSocket    *new_client = new ClientSocket;
    int             client_sock_fd;

    if ((client_sock_fd = accept(listener->get_sock_fd(), NULL, 0)) == -1)
        throw std::runtime_error("accept() failed!");

    fcntl(client_sock_fd, F_SETFL, O_NONBLOCK);

    new_client->set_type('C');
    new_client->set_sock_fd(client_sock_fd);
    new_client->set_servers(listener->get_servers());

    activeClients.push_back(new_client);
    register_socket_in_kqueue(kqueue_fd, activeClients.back(), EVFILT_READ);
}

void    handle_client_request(ClientSocket* client_info)
{
    std::string rcvdMsg;
    char        buffer[READ_BUFFER_SIZE];
    size_t      rcvdSize;
    size_t      crlf_pos;

    if ((rcvdSize = recv(client_info->get_sock_fd(), (void *) buffer, READ_BUFFER_SIZE - 1, 0)) < 0)
        throw std::runtime_error(std::string("Webserv : recv() failed, reason : ") + strerror(errno));
    buffer[rcvdSize] = '\0';
    
    rcvdMsg = buffer;
    
    if (!client_info->request.hasParsedStartLine())
    {
        if ((crlf_pos = rcvdMsg.find(CRLF)) != std::string::npos)
        {
            parse_start_line(client_info->request, rcvdMsg.substr(0, crlf_pos));
        }
        
    }
    
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