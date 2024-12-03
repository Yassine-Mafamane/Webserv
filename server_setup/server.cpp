/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:07:44 by ymafaman          #+#    #+#             */
/*   Updated: 2024/12/03 14:34:56 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
    
}

Server::~Server()
{
    
}

void    Server::setup(const HttpContext & http_config)
{
    socketManager.create_listeners(http_config);
    kqueueManager.create_kqueue();
    kqueueManager.register_listeners_in_kqueue(socketManager.get_listeners());
}

void    Server::start()
{
    // Here we will start ordering the kqueueManager to poll events and return em then we will handle them here
    /*
    1. Server::setup
   ├── SocketHandler::createSockets
   └── KqueueHandler::addSockets

    2. Server::start
    └── loop:
        ├── KqueueHandler::poll
        ├── Server::handleEvent
        └── Event-specific actions
    */
}