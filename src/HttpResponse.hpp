#pragma once

#include <iostream>  	// base
#include <sstream>		// text gestion
#include <map> 			// map
#include <sstream>		// file etc
#include <algorithm> 	// algo
#include <fstream>
#include <sys/stat.h>

#include "HttpRequest.hpp"
#include "Utils.hpp"


class HttpResponse
{
	private:
		int _code;
		std::string _msg;
		std::map<std::string, std::string> _headers;
		std::string _body;
	public:
		HttpResponse();
		HttpResponse(const HttpResponse &copy);
		HttpResponse &operator=(const HttpResponse &copy);
		~HttpResponse();

		// Other function
		void SetStatus(const int code);
		void SetHeader(const std::string &key, const std::string &value);
		void SetBody(const std::string &body);
		void SetKeepAlive(const bool keepAlive);
		void ServeFile(const std::string &filePath);
		std::string ToString() const;

		// Exceptions
		class MessageStatusNotFoundException : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("\033[0;31mMessage status not found\033[0m");
				}
		};
};
