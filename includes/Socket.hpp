#pragma once


# include "Helper.hpp"
# include "ServerConfiguration.hpp"

class Socket {
	
	public:
		Socket();
		virtual ~Socket();
		Socket(const Socket& source);
		Socket& operator=(const Socket &original_copy);

		void	createSocket(int ai_family, int ai_socktype, int ai_protocol);
		void	bindSocket(struct sockaddr* ai_addr, socklen_t ai_addrlen);
		void	listenToSocket(void);
		void	bindSocketAndListen(const char* port);
        void    closeSocket(void);

		int		        getSocketFD(void);
        ServerConfiguration*    getServerConfig(void);

        void    assignAcceptedFD(int fd);
        void    setPortFD(str port);
        void    setServerConfig(ServerConfiguration* sb);

		class SocketInitException : public std::exception {
			const char * what () const throw();
		};
		class SocketFailureException : public std::exception {
			const char * what () const throw();
		};
		class SocketSetOptException : public std::exception {
			const char * what () const throw();
		};
		class SocketBindException : public std::exception {
			const char * what () const throw();
		};
		class SocketListenException : public std::exception {
			const char * what () const throw();
		};
		class SocketAcceptException : public std::exception {
			const char * what () const throw();
		};
        class SocketCloseException : public std::exception {
			const char * what () const throw();
		};

	private:
		int						socket_fd;
		socklen_t				_sin_size;
        str             _port;
        ServerConfiguration*            _server_block;
};
