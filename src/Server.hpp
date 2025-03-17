#pragma once

#include <iostream>  	// base
#include <iomanip> 		// std::setw, std::setfill
#include <fstream>		// file gestion
#include <sstream>		// text gestion
#include <string>		// stoi()
#include <vector> 		// vector
#include <list> 		// list
#include <map> 			// map
#include <algorithm> 	// algo
#include <unistd.h>		// close etc
#include <sys/socket.h> // socket
#include <netinet/in.h> // net
#include <sys/epoll.h>  // epoll
#include <fcntl.h>		// fcntl
#include <csignal> 		// signal
#include <netdb.h>
#include <arpa/inet.h>
#include <ctime> 		// time

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Utils.hpp"


// #define PORT 8080
#define MAX_EVENTS 100
#define BUFFER_SIZE 4096

class Server
{
	private:
		// int server_fd;
		std::vector<int> _server_fds;
		std::map<int, std::multimap< std::string, std::vector<std::string> > > _serv_info;
		int epoll_fd;
		struct sockaddr_in address;
		socklen_t addrlen;

		std::map<int, time_t> _client_last_active;
	public:
		Server();
		Server(const Server &copy);
		Server &operator=(const Server &copy);
		~Server();

		// Other function
        void InitSocket(const std::list< std::multimap< std::string, std::vector<std::string> > > &conf);                			// Initialiser les sockets pour chaque server et les mettres en ecoute
        void ManageConnection(void);          			// Gerer les connections (plusieurs clients), differentes requetes http (get, post, etc ...), reponses http et CGI
		static std::string GetTime(void);

		// Exceptions
		class SocketException : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("\033[0;31mSocket error\033[0m");
				}
		};
		class SetsockoptException : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("\033[0;31mSetsockopt error\033[0m");
				}
		};
		class BindException : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("\033[0;31mBind error\033[0m");
				}
		};
		class ListenException : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("\033[0;31mListen error\033[0m");
				}
		};
		class EpollException : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("\033[0;31mEpoll error\033[0m");
				}
		};
		class EpollWaitException : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("\033[0;31mEpoll_Wait error\033[0m");
				}
		};
		class AcceptException : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("\033[0;31mAccept error\033[0m");
				}
		};
};
