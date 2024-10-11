/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpContext.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 13:01:34 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/11 20:19:43 by ymafaman         ###   ########.fr       */
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
            return ;
        }
    }

    this->error_pages.push_back(error_info);
}

void    HttpContext::set_auto_index(bool auto_index_on)
{
    if (auto_index_on)
        this->auto_index = true;
    else
        this->auto_index = false;
}