/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:37:00 by ymafaman          #+#    #+#             */
/*   Updated: 2024/12/06 02:36:26 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
    {
        parse_body(request, rcvdMsg);
        if (!rcvdMsg.empty())
            request.storeUnparsedMsg(rcvdMsg); // no need ?
    }
    
    
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
    std::string rcvdMsg = "";
    char        buffer[READ_BUFFER_SIZE];
    size_t      rcvdSize;

    if ((rcvdSize = recv(client_info->get_ident(), (void *) buffer, READ_BUFFER_SIZE - 1, 0)) < 0)
        throw std::runtime_error(std::string("Webserv : recv() failed, reason : ") + strerror(errno));

    buffer[rcvdSize] = '\0';
    rcvdMsg.append(buffer, rcvdSize);
    parse_client_request(*(client_info->get_request()), rcvdMsg);    
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

    if (send(client_info->get_ident(), (void *) rsp.c_str(), rsp.length(), 0) == -1)
        throw std::runtime_error("send failed");

    client_info->delete_request();
    client_info->set_request(new Request);
}
