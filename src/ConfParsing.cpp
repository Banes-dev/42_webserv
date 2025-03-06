#include "ConfParsing.hpp"


// Constructor & Destructor
ConfParsing::ConfParsing(void) : _port(0), _host("vide")
{
}

ConfParsing::ConfParsing(const ConfParsing &copy) : _port(copy._port), _host(copy._host)
{
    *this = copy;
}

ConfParsing &ConfParsing::operator=(const ConfParsing &copy)
{
    if (this != &copy)
    {
        this->_port = copy._port;
        this->_host = copy._host;
    }
    return (*this);
}

ConfParsing::~ConfParsing(void)
{
}


// Other function
unsigned int ConfParsing::getPort(void) const
{
    return (this->_port);
}

std::string ConfParsing::getHost(void) const
{
    return (this->_host);
}

void ConfParsing::parsConf(void)
{
    std::ifstream   ifs;

    ifs.open(CONF_PATH);
    if (!ifs)
        throw BadParsingException();
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
            throw BadParsingException();
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
            throw BadParsingException();
        }
    }
}
