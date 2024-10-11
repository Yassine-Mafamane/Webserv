/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpContext.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 11:31:07 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/11 20:25:50 by ymafaman         ###   ########.fr       */
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

        /* Setters */
        void    set_error_page      (const std::pair <unsigned short, std::string>& error_info);
        void    set_server          (ServerContext& new_server);
        void    set_cgi_extension   (const std::string& extension);
        void    set_auto_index      (bool auto_index_on);

    private :

        std::vector<std::pair <unsigned short, std::string> >   error_pages;

        std::vector<ServerContext>                              servers;

        std::string                                             cgi_extension;

        bool                                                    auto_index;

};


#endif