#ifndef KQUEUE_EVENT_QUEUE_HPP
#define KQUEUE_EVENT_QUEUE_HPP

#include "SocketManager.hpp"
#include <sys/event.h>
#include <sys/types.h>

class KqueueEventQueue
{
	public :

	KqueueEventQueue();
	~KqueueEventQueue();

    void    register_listeners_in_kqueue(std::vector<struct ListenerSocket> & activeListners);
    void    switch_interest(ClientSocket* client_info, short old_filter, short new_filter);
	void    register_socket_in_kqueue(Socket * sock_data, short filter);
	void	create_kqueue( void );

    private :

        int		kqueue_fd;
		bool	queue_created;
};














#endif