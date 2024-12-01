


#include "Socket.hpp"

/* ListenerSocket methods definition */

void    ListenerSocket::set_ident( const uintptr_t & id )
{
    this->_ident = id;
}

void    ListenerSocket::set_type( const t_ident_type & type )
{
    this->_type = type;
}

void    ListenerSocket::set_host( const struct in_addr & host )
{
    this->_host = host;
}

void    ListenerSocket::set_port( const unsigned short & port )
{
    this->_port = port;
}

void    ListenerSocket::set_servers( const std::vector<const ServerContext*> & servers )
{
    this->_related_servers = servers;
}

void    ListenerSocket::add_server( const ServerContext* server )
{
    this->_related_servers.push_back(server);
}

uintptr_t   ListenerSocket::get_ident( void )
{
    return this->_ident;
}

t_ident_type    ListenerSocket::get_type( void )
{
    return this->_type;
}

struct in_addr  ListenerSocket::get_host( void )
{
    return this->_host;
}

unsigned short  ListenerSocket::get_port( void )
{
    return this->_port;
}

const std::vector<const ServerContext*>&	ListenerSocket::get_servers( void )
{
    return this->_related_servers;
}

ListenerSocket& ListenerSocket::get_instance( void )
{
    return *this;
}

/* ClientSocket methods definition */

ClientSocket::ClientSocket() : _request(NULL)
{

}

ClientSocket::~ClientSocket()
{
    if (this->_request)
        delete this->_request;
}

void    ClientSocket::set_ident( const uintptr_t & id )
{
    this->_ident = id;
}

void    ClientSocket::set_type( const t_ident_type & type )
{
    this->_type = type;
}

void    ClientSocket::set_host( const struct in_addr & host )
{
    this->_host = host;
}

void    ClientSocket::set_port( const unsigned short & port )
{
    this->_port = port;
}

void    ClientSocket::set_servers( const std::vector<const ServerContext*> & servers )
{
    this->_related_servers = servers;
}

void    ClientSocket::add_server( const ServerContext* server )
{
    this->_related_servers.push_back(server);
}

uintptr_t   ClientSocket::get_ident( void )
{
    return this->_ident;
}

t_ident_type    ClientSocket::get_type( void )
{
    return this->_type;
}

struct in_addr  ClientSocket::get_host( void )
{
    return this->_host;
}

unsigned short  ClientSocket::get_port( void )
{
    return this->_port;
}

const std::vector<const ServerContext*>&	ClientSocket::get_servers( void )
{
    return this->_related_servers;
}

ClientSocket& ClientSocket::get_instance( void )
{
    return *this;
}

void    ClientSocket::set_request( Request *new_request )
{
    this->_request = new_request;
}

Request*	ClientSocket::get_request( void )
{
    return this->_request;
}

void    ClientSocket::delete_request( void )
{
    if (this->_request)
        delete this->_request;
}
