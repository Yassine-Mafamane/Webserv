

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "KqueueIdent.hpp"
#include "../Request/request_parse.hpp"


class Socket : public KqueueIdent {

    public :

        virtual void    									set_host( const struct in_addr & host ) = 0;
        virtual void    									set_port( const unsigned short & port ) = 0;
        virtual void    									set_servers( const std::vector<const ServerContext*> & servers ) = 0;
        virtual void    									add_server( const ServerContext* server ) = 0;

        virtual struct in_addr    							get_host( void ) = 0;
        virtual unsigned short    							get_port( void ) = 0;
		virtual const std::vector<const ServerContext*>&	get_servers( void ) = 0;


    protected :

        struct in_addr                      _host;       // In the host representation
        unsigned short                      _port;       // In the host representation

        std::vector<const ServerContext*>   _related_servers;    // List of server contexts that can serve requests received on this socket.

};

class ListenerSocket : public Socket {

    public :

		/* Setter methods inherited from the KqueueIdent class */
		void										set_ident( const uintptr_t & id );
		void										set_type( const t_ident_type & type );


		/* Setter methods inherited from the Socket class */
        void    									set_host( const struct in_addr & host );
        void    									set_port( const unsigned short & port );
        void    									set_servers( const std::vector<const ServerContext*> & servers );
        void    									add_server( const ServerContext* server );


		// Getter methods inherited from the KqueueIdent class
		uintptr_t									get_ident( void );
		t_ident_type								get_type( void );

		/* Getter methods inherited from the Socket class */
        struct in_addr								get_host( void );
        unsigned short								get_port( void );
		const std::vector<const ServerContext*>&	get_servers( void );

		/* ------------------------------------------------------------------- */

        ListenerSocket& 							get_instance( void );

	private :

};

class ClientSocket : public Socket {

    public :

		/* Constructors */
		ClientSocket();

		/* Destructor */
		~ClientSocket();

		/* Setter methods inherited from the KqueueIdent class */
		void										set_ident( const uintptr_t & id );
		void										set_type( const t_ident_type & type );


		/* Setter methods inherited from the Socket class */
        void    									set_host( const struct in_addr & host );
        void    									set_port( const unsigned short & port );
        void    									set_servers( const std::vector<const ServerContext*> & servers );
        void    									add_server( const ServerContext* server );


		// Getter methods inherited from the KqueueIdent class
		uintptr_t									get_ident( void );
		t_ident_type								get_type( void );


		/* Getter methods inherited from the Socket class */
        struct in_addr								get_host( void );
        unsigned short								get_port( void );
		const std::vector<const ServerContext*>&	get_servers( void );


		/* ------------------------------------------------------------------- */

        ClientSocket&   							get_instance( void );

		void										set_request( Request *new_request );

		Request*									get_request( void );

		void										delete_request( void );

    private :

        Request*	_request;

};

#endif