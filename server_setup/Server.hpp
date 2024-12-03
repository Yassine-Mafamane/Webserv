/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:08:35 by ymafaman          #+#    #+#             */
/*   Updated: 2024/12/03 14:33:14 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "../Request/request_parse.hpp"
#include "KqueueEventQueue.hpp"

/// include only needed hraders ! 

#define TMOUT_SEC 1
#define TMOUT_NSEC 2000
#define READ_BUFFER_SIZE 11024

class Server {

	public :

		// Constructor
		Server();

		// Destructor
		~Server();

		void	setup(const HttpContext & http_config);
		void	start();

    private :

	SocketManager		socketManager;
	KqueueEventQueue	kqueueManager;
};


/*                              Kqueue                              */
void    poll_events(int kqueue_fd, std::vector<struct ListenerSocket> & activeListners);

/*                              Cient Management                              */
void    accept_client_connection(ListenerSocket *listener, int kqueue_fd, std::vector<ClientSocket*>& activeClients);
void    delete_client(std::vector<ClientSocket *>& activeClients, int fd);
void    handle_client_request(ClientSocket* client_info);
void    respond_to_client(ClientSocket* client_info);



#endif