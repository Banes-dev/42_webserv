#pragma once

#include <iostream>  	// base
#include <sstream>		// text gestion
#include <map> 			// map
#include <algorithm> 	// algo

#include "Utils.hpp"


class HttpRequest
{
	private:
		std::string _method;
		std::string _path;
		std::string _version;
		std::map<std::string, std::string> _headers;
		std::string _body;
	public:
		HttpRequest();
		HttpRequest(const HttpRequest &copy);
		HttpRequest &operator=(const HttpRequest &copy);
		~HttpRequest();

		// Other function
        int ParseRequest(std::string buffer) const;

		// Exceptions
		class SocketException : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("\033[0;31mSocket error\033[0m");
				}
		};
};
