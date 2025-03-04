/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   confParsing.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gschwart <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:44:07 by gschwart          #+#    #+#             */
/*   Updated: 2025/03/04 18:44:08 by gschwart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "confParsing.hpp"

CParsing::CParsing(void) : _port(0), _host("vide")
{
    return;
}

CParsing::CParsing(CParsing const & src) : _port(src._port), _host(src._host)
{
    *this = src;
    return;
}

CParsing::~CParsing(void)
{
    return;
}

CParsing &      CParsing::operator=(CParsing const & rhs)
{
    if (this != &rhs)
    {
        this->_port = rhs._port;
        this->_host = rhs._host;
    }
    return (*this);
}

unsigned int       CParsing::getPort(void) const
{
    return this->_port;
}

std::string        CParsing::getHost(void) const
{
    return this->_host;
}

const char * CParsing::BadParsing::what () const throw ()
{
    return "CONFIGURATION FILE ERROR";
};


void     CParsing::ft_parsConf(void)
{
    std::ifstream   ifs;

    ifs.open(CONF_PATH);
    if (!ifs)
        throw CParsing::BadParsing();
    std::string     ligne;
    while (std::getline(ifs, ligne))
    {
        size_t  l = ligne.find("listen");
        if (l != std::string::npos)
        {
            int     j = 0;
            for (std::string::iterator at = ligne.begin(); at != ligne.end() ; at++)
            {
                j++;
                if (*at >= '0' && *at <= '9')
                {
                    std::string         ligne2 = ligne.substr(j);
                    std::stringstream   string(ligne2);
                    unsigned int        n;
                    string >> n;
                    _port = n;
                    break;
                }
            }
            throw CParsing::BadParsing();
        }
    }
    while (std::getline(ifs, ligne))
    {
        size_t  pos =  ligne.find("http:");
        if (pos != std::string::npos)
        {
            int     i = 0;
            for (std::string::iterator at = ligne.begin(); at != ligne.end() ; at++)
            {
                i++;
                if (*at >= '0' && *at <= '9')
                {
                    std::string     str = ligne.substr(i, (ligne.size() - 1));
                    _host = str;
                    break;
                }
            }
            throw CParsing::BadParsing();
        }
    }
}
