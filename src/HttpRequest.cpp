#include "HttpRequest.hpp"


// Constructor & Destructor
HttpRequest::HttpRequest(void)
{
}

HttpRequest::HttpRequest(const HttpRequest &copy)
{
	*this = copy;
}

HttpRequest &HttpRequest::operator=(const HttpRequest &copy)
{
	(void)copy;
	return (*this);
}

HttpRequest::~HttpRequest(void)
{
}


// Other function
int HttpRequest::ParseRequest(std::string buffer) const
{
    // std::cout << "Test : " << std::endl << buffer << std::endl;

    std::stringstream ss(buffer);
	std::string token;

	while (std::getline(ss, token, ' '))
	{
        if (token[0] != "GET" && token[0] != "POST" token[0] != "DELETE")
        {
            std::cout << "Error" << std::endl;
            return (1);
        } else
            this->_method = token[0];
        if (token[1] != "/index.html")
        {
            std::cout << "Error" << std::endl;
            return (1);
        } else
            this->_path = token[1];
        if (token[2] != "HTTP/1.1")
        {
            std::cout << "Error" << std::endl;
            return (1);
        } else
            this->_path = token[2];
        
        std::cout << this->_method << " " << this->_path << " " << this->_version << std::endl;
	}
    return (0);
}
