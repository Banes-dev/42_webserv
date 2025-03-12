#pragma once

# include <iostream>
# include <string>
# include <fstream>
# include <sstream>
# include <stdexcept>
# include <algorithm>
# include <map>
# include <vector>
# include <ctype.h>
# include <list>

#include "Utils.hpp"

#define CONF_PATH "config/default.conf"

class ConfParsing
{
    private:
        std::list< std::multimap< std::string, std::vector<std::string> > >       _def; 
//        std::multimap< std::string, std::vector<std::string> >                  _conf;
//        unsigned int    _port;
//        std::string     _host;

    public:
        ConfParsing();
        ConfParsing(const ConfParsing &copy);
		ConfParsing &operator=(const ConfParsing &copy);
        ~ConfParsing();

        // Other function
        const std::list< std::multimap< std::string, std::vector<std::string> > > &    getConf(void) const;

//        unsigned int    getPort(void) const;
//        std::string     getHost(void) const;
//        void            parsConf(void);

        void                   parsConfUn(void);
        std::string            parsConfDeux(std::string ligne);

		// Exceptions
		class BadParsingException : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("\033[0;31mConfiguration parsing error\033[0m");
				}
		};
};

std::ostream &      operator<<(std::ostream &, ConfParsing const & src);