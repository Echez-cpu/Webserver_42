#pragma once

# include "Socket.hpp"
# include "ClientSession.hpp"
# include "Helper.hpp"
# include "ConfigParser.hpp"
#include <sys/types.h>
#include "Signal_flag.hpp"

typedef std::string str;

class Server {
	
	public:

		Server();
		Server(ConfigParser* conf);
		virtual ~Server();
		Server& operator=(const Server& original_copy);

		void	launchServer(void);

		class PollException : public std::exception {
			const char * what () const throw();
		};
		class AcceptConnectionFailure : public std::exception {
			const char * what () const throw();
		};

    private:
        std::vector<Socket*>         listenSock_vector;
		Socket				        _listenSocket;
		struct addrinfo		        criteria_template;
		struct addrinfo*	        _servinfo;
		int					        _fd_count;
		int					        _fd_size;
		struct pollfd   	    	*_pfds;
		ConfigParser*						_config;
        std::map<int, ClientSession*>  _connections;

		void				addConnection(int newfd, ClientSession* new_connect);
		void				disconnectClient(int i);
		void			    processNewConnection(Socket* process);
		void			    readClientRequest(int i);
        void                sendResponseToClient(int i);
		void*			    extractIPAddressPtr(struct sockaddr *sa);
        Socket*             findListeningSocketByFD(int fd);
};
