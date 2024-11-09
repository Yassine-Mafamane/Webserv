/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:37:00 by ymafaman          #+#    #+#             */
/*   Updated: 2024/11/09 17:01:57 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void delete_client(std::vector<ClientSocket *>& activeClients, int fd)
{
    std::vector<ClientSocket *>::iterator it = activeClients.begin();
    std::vector<ClientSocket *>::iterator end = activeClients.end();

    for ( ; it != end; it++)
    {
        if ((*it)->get_sock_fd() == fd) // TODO : why dont i store clients in a map instead of a vector like that the key will be the client fd so i dont have to loop over all clients to delete on of them
        {
            delete (*it)->request;
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
    new_client->request = new Request();

    activeClients.push_back(new_client);
    register_socket_in_kqueue(kqueue_fd, activeClients.back(), EVFILT_READ);
}

void    determine_parsing_stage(ClientSocket* client_info, std::string & rcvdMsg)
{
    Request&    client_request = *(client_info->request);     
    size_t      crlf_pos = rcvdMsg.find(CRLF);
    static int i;

    i++;
    // Ensure CRLF terminator is present before parsing the start line or headers
    if ((!client_request.hasParsedStartLine() || !client_request.hasParsedHeaders()) && crlf_pos == std::string::npos)
    {
        std::cout << i << std::endl;
        return client_request.storeUnparsedMsg(rcvdMsg); //  here we might have a problem which is the client can keep sending long messages without sending a CRLF terminator ! Thats why at somepoint i have to mark the request as bad if he passes a certain length in the uri or in a header.
    }

    rcvdMsg.insert(0, client_request.getUnparsedMsg());
    client_request.resetUnparsedMsg();

    if (!client_request.hasParsedStartLine())
    {
        parse_start_line(client_request, rcvdMsg.substr(0, crlf_pos)); // what if the request has been marked as bad
        rcvdMsg.erase(0, crlf_pos + 2);
        return determine_parsing_stage(client_info, rcvdMsg);
    }
    // what if the crlf is the last thing in the msg ?  also have to check if there is more crlf to use in header parsing
    
    if (!client_request.hasParsedHeaders())
    {
        parse_headers(client_request, rcvdMsg);
        return determine_parsing_stage(client_info, rcvdMsg);
    }

    if (!client_request.hasParsedBody())
        parse_body(client_request, rcvdMsg);
}

void    handle_client_request(ClientSocket* client_info)
{
    std::string rcvdMsg;
    char        buffer[READ_BUFFER_SIZE];
    size_t      rcvdSize;

    if ((rcvdSize = recv(client_info->get_sock_fd(), (void *) buffer, READ_BUFFER_SIZE - 1, 0)) < 0)
        throw std::runtime_error(std::string("Webserv : recv() failed, reason : ") + strerror(errno));
    buffer[rcvdSize] = '\0';

    rcvdMsg = buffer;

    // std::cout << "------------------------------" << std::endl;
    // std::cout << rcvdMsg << std::endl;
    // std::cout << "------------------------------" << std::endl;
    determine_parsing_stage(client_info, rcvdMsg);
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
    
                        delete client_info->request;
                    client_info->request = new Request();
}