/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   confParsing.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwart <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:43:59 by gschwart          #+#    #+#             */
/*   Updated: 2025/03/04 18:44:00 by gschwart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <string>
# include <fstream>
# include <sstream>
# include <stdexcept>
# include <algorithm>

# define CONF_PATH "../config/default.conf"

class CParsing
{
    public:
        CParsing(void);
        CParsing(CParsing const & src);
        ~CParsing(void);

        CParsing &      operator=(CParsing const & rhs);

        unsigned int       getPort(void) const;
        std::string        getHost(void) const;

        void               ft_parsConf(void);

        class BadParsing : public std::exception
        {
            public:
                virtual const char * what () const throw (); 
        };

    private:
        unsigned int    _port;
        std::string     _host; 
};