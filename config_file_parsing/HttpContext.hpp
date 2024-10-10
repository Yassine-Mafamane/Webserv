/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpContext.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 11:31:07 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/09 12:48:30 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_CONTEXT_HTTP
# define HTTP_CONTEXT_HTTP

#include <vector>
#include <string>
#include "ServerContext.hpp"

class HttpContext {

    public :

        /* HttpContext Constructor */
        HttpContext ( void );

        /* HttpContext Destructor */
        ~HttpContext ();

    private :

        std::vector<std::pair <unsigned short, std::string> >   error_pages;

        std::vector<ServerContext>                              servers;

        std::string                                             root_directory;

        std::string                                             cgi_extension;

        std::string                                             upload_dir;

        std::string                                             index;

        bool                                                    auto_index;

};


#endif