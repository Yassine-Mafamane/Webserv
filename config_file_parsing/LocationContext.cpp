/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationContext.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 20:30:49 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/12 20:53:21 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationContext.hpp"

LocationContext::LocationContext( void )
{
    this->is_exact = false;
}

LocationContext::~LocationContext()
{
    
}

void    LocationContext::set_error_page(const std::pair <unsigned short, std::string>& error_info)
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

void    LocationContext::set_root_directory( std::string root )
{
    this->root_directory = root;

    // Testing
    std::cout << "Server's root : " << root << " has been set!" << std::endl;
}

void    LocationContext::set_cgi_extension(const std::string& extension)
{
    this->cgi_extension = extension;
    
    // Testing 
    std::cout << "New cgi_extension has been set: " << extension << std::endl;
}

void    LocationContext::set_upload_dir( std::string directory )
{
    this->upload_dir = directory;

    // Testing 
    std::cout << "New directory has been set: " << directory << std::endl;
}

void    LocationContext::set_index( std::string index )
{
    this->index = index;

    // Testing 
    std::cout << "New index has been set: " << index << std::endl;
}

void    LocationContext::set_auto_index(bool on_off)
{
    if (on_off)
        this->auto_index = true;
    else
        this->auto_index = false;

    // Testing 
    std::cout << "autoindex value has been stored! as " << this->auto_index << std::endl;
}

void    LocationContext::set_allowed_methods( std::vector<std::string> methods )
{
    this->allowed_methods.assign(methods.begin(), methods.end());

    // Testing 
    std::cout << "New allowed-methods have been set: " << std::endl;
}

void    LocationContext::make_exact( void )
{
    this->is_exact = true;
}

void    LocationContext::set_redirection( std::pair<unsigned short, std::string> redirection_info )
{
    this->redirection = redirection_info;
}