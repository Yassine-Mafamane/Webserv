

#ifndef KQUEUE_IDENT_HPP
#define KQUEUE_IDENT_HPP

#include <sys/event.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "../Contexts/HttpContext.hpp"

typedef enum	e_ident_type
{
	CHILD_ID,
	SERVER_SOCK,
	CLIENT_SOCK
}	t_ident_type;

class KqueueIdent
{
	public :

		virtual void			set_ident( const uintptr_t & id ) = 0;
		virtual void			set_type( const t_ident_type & type ) = 0;

		virtual uintptr_t		get_ident( void ) = 0;
		virtual t_ident_type	get_type( void ) = 0;

	protected :

		uintptr_t		_ident;
		t_ident_type	_type;
};



#endif