#include "HttpRequest.hpp"
#include "Server.hpp"


// Constructor & Destructor
HttpRequest::HttpRequest(void) : _method(""), _path(""), _version(""), _body("")
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
void trim(std::string &str)
{
    // Supprimer les espaces et les retours à la ligne en début et fin de chaîne
    size_t first = str.find_first_not_of(" \r\n\t");
    if (first == std::string::npos) {
        str = "";  // La chaîne ne contient que des espaces blancs
    } else {
        size_t last = str.find_last_not_of(" \r\n\t");
        str = str.substr(first, (last - first + 1));
    }
}
void HttpRequest::ParseRequest(std::string buffer)
{
    // std::cout << "Test : " << std::endl << buffer << std::endl;

    std::replace(buffer.begin(), buffer.end(), '\r', ' ');
    std::stringstream ss(buffer);
    std::string method, path, version;

    // 1. Request Line
    // if (!(ss >> method >> path >> version))
    //     throw HttpRequestLineException();
    // if (method != "GET" && method != "POST" && method != "DELETE")
    //     throw MethodException();
    // if (version != "HTTP/1.1")
    //     throw HttpVersionException();
    std::getline(ss, method, ' ');  // Lire la méthode (avant l'espace)
    std::getline(ss, path, ' ');    // Lire le chemin (avant l'espace)
    std::getline(ss, version); // Lire la version (jusqu'à la fin de la ligne)

    trim(version);

    // Vérification de la ligne de requête
    if (method.empty() || path.empty() || version.empty())
        throw HttpRequestLineException();
    if (method != "GET" && method != "POST" && method != "DELETE")
        throw MethodException();
    if (version != "HTTP/1.1")
        throw HttpVersionException();
    this->_method = method;
    this->_path = path;
    this->_version = version;

    // 2. Parse headers
    std::string header_line;
    while (std::getline(ss, header_line) && !header_line.empty())
    {
        std::cout << "Header line : " << header_line << std::endl;
        size_t slipt_pos = header_line.find(": ");
        // std::cout << header_line << " " << slipt_pos << " " << std::string::npos << std::endl;
        if (slipt_pos != std::string::npos)
        {
            std::string key = header_line.substr(0, slipt_pos);
            std::string value = header_line.substr(slipt_pos + 2);  // Ignore le ": "
            this->_headers[key] = value;

            std::cout << "Header: " << key << ": " << value << std::endl;
        }
        else
            throw HeadersException();
    }

    std::cout << Purple << Server::GetTime() << " " << this->_method << " " << this->_path << " " << this->_version << Reset_Color << std::endl;
}
