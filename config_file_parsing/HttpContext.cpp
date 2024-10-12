/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpContext.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 13:01:34 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/12 21:05:21 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpContext.hpp"

HttpContext::HttpContext( void )
{
    this->auto_index = false;
}

HttpContext::~HttpContext()
{
    
}

void    HttpContext::set_error_page(const std::pair <unsigned short, std::string>& error_info)
{
    std::vector<std::pair <unsigned short, std::string> >::iterator it;

    for (it = this->error_pages.begin(); it != this->error_pages.end(); it++)
    {
        if ((*it).first == error_info.first)
        {
            (*it).second = error_info.second;

            // Testing 
            std::cout << "error code : " << error_info.first << " page's has been changed to : " << error_info.second << std::endl;

            return ;
        }
    }

    // Testing 
    std::cout << "new error page has got stored : " << error_info.first << ": " << error_info.second << std::endl;

    this->error_pages.push_back(error_info);
}

void    HttpContext::set_auto_index(bool on_off)
{
    if (on_off)
        this->auto_index = true;
    else
        this->auto_index = false;

    // Testing 
    std::cout << "autoindex value is getting stored! as " << this->auto_index << std::endl;
}

void    HttpContext::set_cgi_extension(const std::string& extension)
{
    this->cgi_extension = extension;
    
    // Testing 
    std::cout << "New cgi_extension has been set: " << extension << std::endl;
}

void   HttpContext::set_new_server( void )
{
    ServerContext   new_server;

    std::vector<std::pair <unsigned short, std::string> >::iterator it;

    /* Making the new server inherit the httpContext directives. */
    for (it = this->error_pages.begin(); it < this->error_pages.end(); it++)
        new_server.set_error_page(*it);

    new_server.set_cgi_extension(this->cgi_extension);
    new_server.set_auto_index(this->auto_index);
    
    /* Pushing the newly created server to the servers vector */
    this->servers.push_back(new_server);

    // Testing     
    std::cout << "New ServerContext has been created!" << std::endl;
}


ServerContext&  HttpContext::get_latest_server( void )
{
    return this->servers.back();
}