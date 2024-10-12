/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpContext.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 11:31:07 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/12 17:19:40 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_CONTEXT_HTTP
# define HTTP_CONTEXT_HTTP

#include <vector>
#include <string>
#include <iostream>
#include "ServerContext.hpp"

class HttpContext {

    public :

        /* HttpContext Constructor */
        HttpContext ( void );

        /* HttpContext Destructor */
        ~HttpContext ();

        /* Setters */
        void    set_error_page      ( const std::pair <unsigned short, std::string>& error_info );
        void    set_new_server      ( void ); //  This method creates a new Server Context when a new server directive is found in the configuration file.
        void    set_cgi_extension   ( const std::string& extension );
        void    set_auto_index      ( bool on_off );

        /* Getters */


        /* The following method returns the latest inserted serverContext in the servers vector in order to store it's info. */
        ServerContext&  get_latest_server( void );

    private :

        std::vector<std::pair <unsigned short, std::string> >   error_pages;

        std::vector<ServerContext>                              servers;

        std::string                                             cgi_extension;

        bool                                                    auto_index;

};


#endif