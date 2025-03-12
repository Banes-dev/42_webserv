#include "CgiExecution.hpp"


CgiExecution::CgiExecution(std::string const & method, std::string const  & path, std::string const & body, std::map<std::string, std::string> const & header) : _env("!b"), _realPath("@@"), _method(method), _path(path), _body(body), _header(header)
{
    return;
}

CgiExecution::CgiExecution(CgiExecution const & src) : _env("!b"), _realPath("@@"), _method(src._method), _path(src._path), _body(src._body), _header(src._header)
{
    *this = src;
    return;
}

CgiExecution::~CgiExecution(void)
{
    return;
}

CgiExecution &      CgiExecution::operator=(CgiExecution const & rhs)
{
    if (this != &rhs)
        *this = rhs;
    return *this;
}

void                CgiExecution::methodeType(void)
{
    if (_method == "GET")
        this->parsQueryString(_path);
    else if (_method == "POST")
        this->parsQueryString(_body);
    else if (_method == "DELETE")
        this->functionDelete();
}

void                CgiExecution::functionDelete()
{
    return;
}

void                CgiExecution::parsQueryString(std::string & str)
{
    (void)str;
    std::string     _path = "sfgfgfs?guigui=1";
    std::cout << "cgiexecution";
    size_t      t = _path.find('?');
    if (t != std::string::npos)
    {
        std::string     path = _path.substr(t);
        std::cout << std::endl << path << std::endl;
        _realPath = path;
    }
    else
        _realPath = "";
}

void                CgiExecution::execute(void)
{

}