SERVER_SRCS			=	server_setup/server.cpp \
						server_setup/polling.cpp  \

UTILS_SRCS			=	Utils/helper_functions.cpp

CONFIG_PARSE_SRCS				=	config_file_parsing/config_exception_throw.cpp \
						config_file_parsing/config_storing.cpp \
						config_file_parsing/config_values_extracter.cpp \
						config_file_parsing/config_parser.cpp \
						config_file_parsing/token_name_checker.cpp \

CONTEXTS_SRCS		=	Contexts/HttpContext.cpp \
						Contexts/LocationContext.cpp \
						Contexts/ServerContext.cpp \

MAIN_SRCS			=	main.cpp

#-------------------------------------------------------------------------------------------------------------------------------#

CONFIG_PARSE_OBJECTS = $(CONFIG_PARSE_SRCS:.cpp=.o)

UTILS_OBJECTS = $(UTILS_SRCS:.cpp=.o)

CONTEXTS_OBJECTS = $(CONTEXTS_SRCS:.cpp=.o)

SERVER_OBJECTS = $(SERVER_SRCS:.cpp=.o)

MAIN_OBJECTS = $(MAIN_SRCS:.cpp=.o)

#-------------------------------------------------------------------------------------------------------------------------------#

CPP = c++

FLAGS = -Wall -Wextra -Werror -std=c++98 

NAME = webserv

#-------------------------------------------------------------------------------------------------------------------------------#

all : $(NAME)

$(NAME) : $(CONFIG_PARSE_OBJECTS) $(UTILS_OBJECTS) $(CONTEXTS_OBJECTS) $(SERVER_OBJECTS) $(MAIN_OBJECTS)
		$(CPP) $(FLAGS) $(CONFIG_PARSE_OBJECTS) $(UTILS_OBJECTS) $(CONTEXTS_OBJECTS) $(SERVER_OBJECTS) $(MAIN_OBJECTS) -o $(NAME)

Utils/%.o : Utils/%.cpp
	$(CPP) $(FLAGS) -c $< -o $@

config_file_parsing/%.o : config_file_parsing/%.cpp
	$(CPP) $(FLAGS) -c $< -o $@

Contexts/%.o : Contexts/%.cpp
	$(CPP) $(FLAGS) -c $< -o $@

server_setup/%.o : server_setup/%.cpp
	$(CPP) $(FLAGS) -c $< -o $@

%.o : %.cpp
	$(CPP) $(FLAGS) -c $< -o $@

#-------------------------------------------------------------------------------------------------------------------------------#

clean :
	rm -rf $(CONTEXTS_SRCS)
	rm -rf $(CONFIG_PARSE_OBJECTS)
	rm -rf $(UTILS_OBJECTS)
	rm -rf $(SERVER_OBJECTS)
	rm -rf $(MAIN_OBJECTS)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : clean
