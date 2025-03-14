#include "CgiExecution.hpp"


CgiExecution::CgiExecution(std::string const & method, std::string const  & path, std::string const & body, std::string const & version, std::map<std::string, std::string> const & header) : _env("!b"), _realPath(""), _method(method), _path(path), _body(body), _version(version), _header(header)
{
    return;
}

CgiExecution::CgiExecution(CgiExecution const & src) : _env("!b"), _realPath(""), _method(src._method), _path(src._path), _body(src._body), _version(src._version), _header(src._header)
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

void                CgiExecution::methodeType(char **env)
{
    addEnv();
    if (_method == "GET")
        this->parsQueryString(_path);
    else if (_method == "POST")
        this->parsBody(_body);
    else if (_method == "DELETE")
        this->functionDelete();
    executeCgi(env);
}

void                CgiExecution::functionDelete()
{
    return;
}

void                CgiExecution::parsQueryString(std::string & str)
{
    (void)str;
    std::string     _path = "sfgfgfs?guigui=1&pars=8";
    std::cout << "cgiexecution" << std::endl;
    size_t      t = _path.find('?');
    if (t != std::string::npos)
    {
        std::string     path = _path.substr(t);
        std::cout << Blue << path << Reset_Color << std::endl;
        _realPath = path;
    }
}

void                CgiExecution::parsBody(std::string & str)
{
    std::cout << "cgiexecutionPost" << std::endl;
    std::cout << Green << str << Reset_Color << std::endl;
    char    *data = getenv("QUERY_STRING");
    int     nb = atoi(data);
    if (nb == 0)
        write(1, "", 1);
    if (str.empty())
        return;
    write(1, str.c_str(), str.size());
}

void                CgiExecution::addEnv(void)
{
    std::cout <<std::endl;
    if (setenv("QUERY_STRING", _realPath.c_str(), 0) == -1)
        throw "variable erreur2";
    char    *data = getenv("QUERY_STRING");
    printf("data %s\n", data);

    std::map<std::string, std::string>::const_iterator  it3 = _header.find("Content-Length");
    if (it3 == _header.end())
    {
    std::string     c = "0";
    if (setenv("CONTENT_LENGTH", c.c_str(), 0) == -1)
        throw "variable erreur1";
    }
    else
    {
        if (setenv("CONTENT_LENGTH", it3->second.c_str(), 0) == -1)
            throw "variable erreur1";
    }
    char    *d3 = getenv("CONTENT_LENGTH");
    printf(" d2 %s\n", d3);

    if (setenv("REQUEST_METHOD", _method.c_str(),0) == -1)
        throw "variable error6";
    char    *m = getenv("REQUEST_METHOD");
    printf(" m %s\n", m);

    if (setenv("SERVER_SOFTWARE", _version.c_str(),0) == -1)
        throw "variable error8";
    char    *s1 = getenv("SERVER_SOFTWARE"); 
    printf(" s1 %s\n", s1); 

    std::map<std::string, std::string>::const_iterator  it2 = _header.find("User-Agent");
    if (it2 != _header.end())
    {
        if (setenv("REMOTE_USER", it2->second.c_str(),0) == -1)
            throw "variable error9";
    }
    char    *r2 = getenv("REMOTE_USER");
    printf(" r2 %s\n", r2);

    std::map<std::string, std::string>::const_iterator  it = _header.find("Host");
    if (it != _header.end())
    {
        if (setenv("SERVER_NAME", it->second.c_str(),0) == -1)
            throw "variable error10";
    }
    char    *s = getenv("SERVER_NAME");
    printf(" s %s\n", s);

/*    if (setenv("CONTENT_TYPE", , 0) == -1)
        throw "variable erreur3";
    std::cout << str << std::endl;
    if (setenv("SERVER_PORT", , 0) == -1)
        throw "variable erreur4";
    if (setenv("PATH_INFO", ,0) == -1)
        throw "variable error5";
    if (setenv("SCRIPT_NAME", ,0) == -1)
        throw "variable error6";
    if (setenv("REMOTE_ADDR", ,0) == -1)
        throw "variable error7";
    std::cout << str << std::endl;*/
}

char*       ft_strdup(const char * str)
{
    int     i = 0;
    char    *result;
    if (str == NULL)
        return (NULL);
    while (str[i] != '\0')
        i++;
    result = (char *)malloc((i + 1) * sizeof(char));
    if (!result)
        return (NULL);
    i = 0;
    while (str[i] != '\0')
    {
        result[i] = str[i];
        i++;
    }
    result[i] = '\0';
    return (result);
}

void    CgiExecution::executeCgi(char **env)
{
    int     pipe_fd[2];

    pid_t   pid;
    (void)env;
    if (pipe(pipe_fd) == -1)
    {
        printf("pipe :%s", strerror(errno));
        return;
    }
    pid = fork();
    if (pid == -1)
    {
        printf("fork :%s", strerror(errno));
        return;
    }
    (void)env;
    if (pid == 0)
    {
        if (_method == "POST")
        {
            dup2(pipe_fd[0], STDIN_FILENO);
        }
        close(pipe_fd[0]);
//        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
        char    **tab;

        tab = (char **)malloc(2 * sizeof(char*));
        tab[0] = (char *)malloc(18 * sizeof(char));
        tab[0] = ft_strdup("src/cgi-bin/hello.py");
        tab[1] = NULL;
        if (access("src/cgi-bin/hello.py", X_OK) == -1)
        {
            std::cerr << "Erreur d'accès à 'cgi-bin/hello.py' : " << strerror(errno) << std::endl;
            return;
        }
        execve("src/cgi-bin/hello.py", tab, env);
        std::cout << "echec execute" << std::endl;
    }
    else
    {
        close(pipe_fd[1]);
    }
    wait(NULL);
}