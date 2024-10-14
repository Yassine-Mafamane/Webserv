/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:05:35 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/14 15:31:56 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "webserv.hpp"

int main(int argc, char *argv[])
{
    std::string file_name;

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
        config_tokenizer(file_name);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}