CPP = c++

FLAGS = -Wall -Wextra -Werror -std=c++98

CONFIG_FILE_PARSING_HEADER_FILES =  webserv.hpp

CONFING_PARSE_SRCS = config_file_parsing/config_parser.cpp config_file_parsing/HttpContext.cpp

CONFING_PARSE_OBJECTS = $(CONFING_PARSE_SRCS:.cpp=.o)

MAIN_SRC = main.cpp

MAIN_OBJECTS = $(MAIN_SRC:.cpp=.o)

NAME = webserv

all : $(NAME)

$(NAME) : $(CONFING_PARSE_OBJECTS) $(MAIN_OBJECTS)
		$(CPP) $(FLAGS) $(CONFING_PARSE_OBJECTS) $(MAIN_OBJECTS) -o $(NAME)

config_file_parsing/%.o : config_file_parsing/%.cpp config_file_parsing/HttpContext.hpp config_file_parsing/ServerContext.hpp config_file_parsing/LocationContext.hpp config_file_parsing/config_parse.hpp
						$(CPP) $(FLAGS) -c $< -o $@

%.o : %cpp webserv.hpp
	$(CPP) $(FLAGS) -c $< -o $@

clean :
	rm -rf $(CONFING_PARSE_OBJECTS)
	rm -rf $(MAIN_OBJECTS)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : clean
