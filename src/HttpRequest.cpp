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
    std::replace(buffer.begin(), buffer.end(), '\r', ' ');
    std::stringstream ss(buffer);
    std::string method, path, version;

    // 1. Request Line
    std::getline(ss, method, ' ');
    std::getline(ss, path, ' ');
    std::getline(ss, version);
    trim(version);
    // std::cout << method << " " << path << " " << version << std::endl;
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
    while (std::getline(ss, header_line))
    {
        trim(header_line);
        if (header_line.empty())
            break;
        size_t slipt_pos = header_line.find(": ");
        if (slipt_pos != std::string::npos)
        {
            std::string key = header_line.substr(0, slipt_pos);
            std::string value = header_line.substr(slipt_pos + 2);
            this->_headers[key] = value;
            // std::cout << "Header: " << key << ": " << value << std::endl;
        }
        else
            throw HeadersException();
    }

    // 3. Get body
    std::string body_content;
    char c;
    while (ss.get(c))
        body_content.push_back(c);
    if (!body_content.empty())
    {
        this->_body = body_content;
        // std::cout << "Body: " << this->_body << std::endl;
    }

    std::cout << Purple << Server::GetTime() << " " << this->_method << " " << this->_path << " " << this->_version << Reset_Color << std::endl;
}

const std::string HttpRequest::GetMethod(void)
{
    return (this->_method);
}
const std::string HttpRequest::GetPath(void)
{
    return (this->_path);
}
const std::string HttpRequest::GetVersion(void)
{
    return (this->_version);
}
const std::map<std::string, std::string> &HttpRequest::GetHeaders(void)
{
    return (this->_headers);
}
const std::string HttpRequest::GetBody(void)
{
    return (this->_body);
}
