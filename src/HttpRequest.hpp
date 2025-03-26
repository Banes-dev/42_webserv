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
        void ParseRequest(std::string buffer);

		std::string decodeChunkedBody(const std::string &body);
		const std::string GetMethod(void);
		const std::string GetPath(void);
		const std::string GetVersion(void);
		const std::map<std::string, std::string> &GetHeaders(void);
		const std::string GetBody(void);

		// Exceptions
		class HttpRequestLineException : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("\033[0;31mHttp request line error\033[0m");
				}
		};
		class MethodException : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("\033[0;31mMethod request error\033[0m");
				}
		};
		class HttpVersionException : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("\033[0;31mHttp version error\033[0m");
				}
		};
		class HeadersException : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("\033[0;31mHeaders error\033[0m");
				}
		};
};
