#include "ConfParsing.hpp"


// Constructor & Destructor
ConfParsing::ConfParsing(void) : _port(0), _host("vide")
{
    return;
}

ConfParsing::ConfParsing(const ConfParsing &copy) : _port(copy._port), _host(copy._host)
{
    *this = copy;
    return;
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
    return;
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

    ifs.open(CONF_PATH, std::ifstream::in);
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
                if (*at >= '0' && *at <= '9')
                {
                    std::string         ligne2 = ligne.substr(j);
                    std::stringstream   string(ligne2);
                    unsigned int        n;
                    string >> n;
                    _port = n;
                    break;
                }
                j++;
            }
        }
        size_t  pos =  ligne.find("proxy_pass http://");
        if (pos != std::string::npos)
        {
            int     i = 0;
            for (std::string::iterator at = ligne.begin(); at != ligne.end() ; at++)
            {
                if (*at >= '0' && *at <= '9')
                {
                    std::string     str = ligne.substr(i);
                    size_t  end = str.find(';');
                    if (end != std::string::npos)
                        str = str.substr(0, end);
                    _host = str;
                    break;
                }
                i++;
            }
        }
    }
}
