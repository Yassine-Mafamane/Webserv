/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerContext.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 16:37:22 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/12 21:08:59 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerContext.hpp"

ServerContext::ServerContext( void )
{
    this->index = "index.html"; // Setting "index.html" as default index.
}

ServerContext::~ServerContext()
{
    
}

void    ServerContext::set_error_page(const std::pair <unsigned short, std::string>& error_info)
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

void   ServerContext::set_new_location( void )
{
    LocationContext   new_location;

    std::vector<std::pair <unsigned short, std::string> >::iterator it;

    /* Making the new location inherit the serverContext directives. */
    for (it = this->error_pages.begin(); it < this->error_pages.end(); it++)
        new_location.set_error_page(*it);

    new_location.set_allowed_methods(this->allowed_methods);
    new_location.set_root_directory(this->root_directory);
    new_location.set_cgi_extension(this->cgi_extension);
    new_location.set_upload_dir(this->upload_dir);
    new_location.set_index(this->index);
    new_location.set_auto_index(this->auto_index);

    this->locations.push_back(new_location);

    // Testing     
    std::cout << "New LocationContext has been created!" << std::endl;
}

void    ServerContext::set_port( unsigned short port )
{
    this->port = port;

    // Testing
    std::cout << "Server's port number : " << port << " has been set!" << std::endl;
}

void    ServerContext::set_root_directory( std::string root )
{
    this->root_directory = root;

    // Testing
    std::cout << "Server's root : " << root << " has been set!" << std::endl;
}

void    ServerContext::set_cgi_extension(const std::string& extension)
{
    this->cgi_extension = extension;
    
    // Testing 
    std::cout << "New cgi_extension has been set: " << extension << std::endl;
}

void    ServerContext::set_upload_dir( std::string directory )
{
    this->upload_dir = directory;

    // Testing 
    std::cout << "New directory has been set: " << directory << std::endl;
}

void    ServerContext::set_index( std::string index )
{
    this->index = index;

    // Testing 
    std::cout << "New index has been set: " << index << std::endl;
}

void    ServerContext::set_server_names( std::vector<std::string> names )
{
    this->server_names.assign(names.begin(), names.end());

    // Testing 
    std::cout << "New server_names have been set: " << std::endl;
}

void    ServerContext::set_auto_index(bool on_off)
{
    if (on_off)
        this->auto_index = true;
    else
        this->auto_index = false;

    // Testing 
    std::cout << "autoindex value has been stored! as " << this->auto_index << std::endl;
}

void    ServerContext::set_allowed_methods( std::vector<std::string> methods )
{
    this->allowed_methods.assign(methods.begin(), methods.end());

    // Testing 
    std::cout << "New allowed-methods have been set: " << std::endl;
}

LocationContext&  ServerContext::get_latest_location( void )
{
    return this->locations.back();
}