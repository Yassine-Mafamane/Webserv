/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpContext.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 13:01:34 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/09 13:17:07 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpContext.hpp"

HttpContext::HttpContext( void )
{
    this->auto_index = false;
    this->index = "index.html";
    this->root_directory = "/";
    this->upload_dir = this->root_directory;
}

HttpContext::~HttpContext()
{
    
}

