SERVER_SRCS			=	server_setup/Server.cpp \
						server_setup/SocketManager.cpp  \
						server_setup/ClientHandler.cpp \
						server_setup/KqueueEventQueue.cpp \
						server_setup/Socket.cpp

UTILS_SRCS			=	Utils/helper_functions.cpp

CONFIG_PARSE_SRCS	=	config_file_parsing/token_name_checker.cpp \
						config_file_parsing/ConfigTokenizer.cpp \
						config_file_parsing/ConfigException.cpp \
						config_file_parsing/ConfigParser.cpp \
						config_file_parsing/ConfigValueExtractor.cpp

CONTEXTS_SRCS		=	Contexts/HttpContext.cpp \
						Contexts/LocationContext.cpp \
						Contexts/ServerContext.cpp

MAIN_SRCS			=	main.cpp

REQUEST_SRCS		=	Request/Request.cpp \
						Request/start_line_parser.cpp \
						Request/headers_parser.cpp \
						Request/body_parser.cpp

#-------------------------------------------------------------------------------------------------------------------------------#

CONFIG_PARSE_OBJECTS = $(CONFIG_PARSE_SRCS:.cpp=.o)

UTILS_OBJECTS = $(UTILS_SRCS:.cpp=.o)

CONTEXTS_OBJECTS = $(CONTEXTS_SRCS:.cpp=.o)

SERVER_OBJECTS = $(SERVER_SRCS:.cpp=.o)

MAIN_OBJECTS = $(MAIN_SRCS:.cpp=.o)

REQUEST_OBJECTS = $(REQUEST_SRCS:.cpp=.o)

#-------------------------------------------------------------------------------------------------------------------------------#

CPP = c++

FLAGS =   -Wall -Wextra -Werror -std=c++98 -fsanitize=address

NAME = webserv

#-------------------------------------------------------------------------------------------------------------------------------#

all : $(NAME)

$(NAME) : $(CONFIG_PARSE_OBJECTS) $(UTILS_OBJECTS) $(CONTEXTS_OBJECTS) $(SERVER_OBJECTS) $(MAIN_OBJECTS) $(REQUEST_OBJECTS)
		$(CPP) $(FLAGS) $(CONFIG_PARSE_OBJECTS) $(UTILS_OBJECTS) $(CONTEXTS_OBJECTS) $(SERVER_OBJECTS) $(MAIN_OBJECTS) $(REQUEST_OBJECTS) -o $(NAME)

Utils/%.o : Utils/%.cpp
	$(CPP) $(FLAGS) -c $< -o $@

config_file_parsing/%.o : config_file_parsing/%.cpp
	$(CPP) $(FLAGS) -c $< -o $@

Contexts/%.o : Contexts/%.cpp
	$(CPP) $(FLAGS) -c $< -o $@

server_setup/%.o : server_setup/%.cpp
	$(CPP) $(FLAGS) -c $< -o $@

Request/%.o : Request/%.cpp Request/Request.hpp
	$(CPP) $(FLAGS) -c $< -o $@

%.o : %.cpp
	$(CPP) $(FLAGS) -c $< -o $@

#-------------------------------------------------------------------------------------------------------------------------------#

clean :
	rm -rf $(CONTEXTS_OBJECTS)
	rm -rf $(CONFIG_PARSE_OBJECTS)
	rm -rf $(UTILS_OBJECTS)
	rm -rf $(SERVER_OBJECTS)
	rm -rf $(MAIN_OBJECTS)
	rm -rf $(REQUEST_OBJECTS)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : clean
