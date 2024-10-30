/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:05:35 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/29 21:09:59 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main(int argc, char *argv[])
{
    std::string file_name;

    /*  Better create a function called usageERR to show this message! */
    if (argc != 2)
    {
        std::cerr << "Usage : ./webserv file.config" << std::endl;
        return (1);
    }

    file_name = argv[1];
    
    if((file_name.length() <= 7) || (file_name.find(".config" ,file_name.length() - 7) == std::string::npos))
    {
        std::cerr << "Wrong file extension!" << std::endl;
        std::cerr << "Usage : ./webserv file.config" << std::endl;
        return (1);
    }

    try
    {
        HttpContext http_config = parse_config_file(file_name);
        setup_servers(http_config);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return (1);
    }
    catch(const char* err_msg)
    {
        std::cerr << err_msg << '\n';
        return (1);
    }
}