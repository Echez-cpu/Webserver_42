# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pokpalae <pokpalae@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/08 13:54:05 by pokpalae          #+#    #+#              #
#    Updated: 2025/07/08 13:54:10 by pokpalae         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = webserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC_DIRS = main.cpp \
	config/ConfigParser.cpp config/LocationBlock.cpp config/ServerConfiguration.cpp \
	server/Server.cpp server/Socket.cpp \
	http/ClientRequest.cpp http/ReplyClient.cpp \
	http/UR_Locator.cpp http/ClientSession.cpp \
	utils/Helper.cpp \
	httpResponse/DeleteActionResponse.cpp httpResponse/FailActionResponse.cpp \
	httpResponse/GetActionResponse.cpp httpResponse/PostActionResponse.cpp

OBJS = $(SRC_DIRS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)
	@echo "✅ Compilation successful: $(NAME)"

clean:
	rm -f $(OBJS)
	@echo "🧹 Object files cleaned."

fclean: clean
	rm -f $(NAME)
	@echo "🧹 Binary removed: $(NAME)"

re: fclean all




