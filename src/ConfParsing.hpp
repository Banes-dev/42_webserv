#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

#include "Utils.hpp"

#define CONF_PATH "config/default.conf"

class ConfParsing
{
    private:
        unsigned int    _port;
        std::string     _host; 
    public:
        ConfParsing();
        ConfParsing(const ConfParsing &copy);
		ConfParsing &operator=(const ConfParsing &copy);
        ~ConfParsing();

        // Other function
        unsigned int    getPort(void) const;
        std::string     getHost(void) const;
        void parsConf(void);

		// Exceptions
		class BadParsingException : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("\033[0;31mConfiguration parsing error\033[0m");
				}
		};
};