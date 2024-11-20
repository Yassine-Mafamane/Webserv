/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:37:00 by ymafaman          #+#    #+#             */
/*   Updated: 2024/11/20 08:51:16 by ymafaman         ###   ########.fr       */
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
            std::cerr << "client deleted ! " << std::endl;
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

void    determine_parsing_stage(Request & request, std::string & rcvdMsg)
{
    size_t      crlf_pos = rcvdMsg.find(CRLF);

    if ((!request.hasParsedStartLine() || !request.hasParsedHeaders()) && crlf_pos == std::string::npos)
        return request.storeUnparsedMsg(rcvdMsg); 

    rcvdMsg.insert(0, request.getUnparsedMsg());
    request.resetUnparsedMsg();
    crlf_pos = rcvdMsg.find(CRLF);

    if (!request.hasParsedStartLine())
    {
        parse_start_line(request, rcvdMsg.substr(0, crlf_pos));
        rcvdMsg.erase(0, crlf_pos + 2);
        return determine_parsing_stage(request, rcvdMsg);
    }

    if (request.get_method() != "POST")
    {
        request.markBodyParsed(true);
        request.markAsReady(true);
        return; 
    }

    if (!request.hasParsedHeaders())
    {
        parse_headers(request, rcvdMsg);
        return determine_parsing_stage(request, rcvdMsg);
    }

    // only read the body if the method is not get or head! or if content length is not 0...
    if (!request.hasParsedBody())
        parse_body(request, rcvdMsg);
}

void    parse_client_request(Request & request, std::string & rcvdMsg)
{
    try
    {
        // if (!request.hasParsedHeaders())
        // {
        //     size_t null_term_pos = rcvdMsg.find('\0');
        //     for (size_t i = null_term_pos; i < rcvdMsg.length() ; ++i)
        //     {
        //         if ( rcvdMsg[i] != '\0' )
        //             request.markAsBad(true);
        //     }
        // }

        determine_parsing_stage(request, rcvdMsg);  
    }
    catch(const std::exception & e)
    {
            std::cerr << e.what() << std::endl;
            request.markAsBad(3334);
        return ;
    }
}

void    handle_client_request(ClientSocket* client_info)
{
    std::string rcvdMsg;
    char        buffer[READ_BUFFER_SIZE];
    size_t      rcvdSize;

    if ((rcvdSize = recv(client_info->get_sock_fd(), (void *) buffer, READ_BUFFER_SIZE - 1, 0)) < 0)
        throw std::runtime_error(std::string("Webserv : recv() failed, reason : ") + strerror(errno));

    buffer[rcvdSize] = '\0';
    rcvdMsg.append(buffer, rcvdSize);

    std::cerr << rcvdMsg ;
    std::cerr.flush();
    
    // size_t crlf_pos;
    // crlf_pos = rcvdMsg.find(CRLF);
    // while (crlf_pos != std::string::npos)
    // {
    //     rcvdMsg.insert(crlf_pos, ".");
    //     crlf_pos = rcvdMsg.find(CRLF, crlf_pos + 3);
    // }
    // std::cerr << rcvdMsg << std::endl;
    // std::cout << "--------------" << std::endl;
    // return ;
    parse_client_request(*(client_info->request), rcvdMsg);
    
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
    client_info->request = NULL;
    client_info->request = new Request();
}
