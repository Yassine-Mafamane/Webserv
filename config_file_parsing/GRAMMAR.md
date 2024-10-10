<Config>    ::= <Context>*

<Context>   ::= <HTTP> | <Server> | <Location>

<HTTP>      ::= 'http {' <Directive>* <Server>* '}'

<Server>    ::= 'server {' <Directive>* <Location>* '}'

<Location>  ::= 'location' '='? <PATH> '{' <Directive>* '}'

<PATH>      ::= '/' STR;

<Directive> ::= 'root'
                    | 'autoindex'
                    | 'cgi_extention'
                    | 'error_pages'
                    | 'upload_dir'
                    | 'index'
                    | 'port'
                    | 'server_names'
                    | 'allowed_methods'