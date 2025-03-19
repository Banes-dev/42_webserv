#include "CgiExecution.hpp"


// Constructor & Destructor
CgiExecution::CgiExecution(const std::string &method, const std::string &path, const std::string &body, const std::string &version, const std::map<std::string, std::string> &header) : _env("!b"), _realPath(""), _method(method), _path(path), _body(body), _version(version), _header(header)
{
}

CgiExecution::CgiExecution(const CgiExecution &copy) : _env("!b"), _realPath(""), _method(copy._method), _path(copy._path), _body(copy._body), _version(copy._version), _header(copy._header)
{
    *this = copy;
}

CgiExecution &CgiExecution::operator=(const CgiExecution &copy)
{
    if (this != &copy)
        *this = copy;
    return (*this);
}

CgiExecution::~CgiExecution(void)
{
}


// Other function
void CgiExecution::methodeType(void)
{
    char **envp;

    envp = (char **)malloc(12 * sizeof(char *));
    if (envp == NULL)
        return;
    envp[11] = NULL;
    std::cout << std::endl;
    std::map<std::string, std::string>::const_iterator it3 = _header.find("Content-Length");
    if (it3 == _header.end())
        envp[0] = strdup("CONTENT_LENGTH=0");
    else
    {
        std::string temp = "CONTENT_LENGTH=" + it3->second;
        envp[0] = strdup(temp.c_str());
    }
    if (_method == "GET")
        this->parsQueryString(_path);
//    else if (_method == "POST")
//        this->parsBody(_body);
    else if (_method == "DELETE")
        this->functionDelete();
    std::cout << std::endl << "env" << std::endl;
    envp[1] = strdup(("QUERY_STRING=" + _realPath).c_str());
    envp[2] = strdup(("REQUEST_METHOD=" + _method).c_str());
    envp[3] = strdup(("SERVER_SOFTWARE=" + _version).c_str());
    std::map<std::string, std::string>::const_iterator it2 = _header.find("User-Agent");
    if (it2 != _header.end())
        envp[4] = strdup(("REMOTE_USER=" + it2->second).c_str());
    else
        envp[4] = strdup("REMOTE_USER=");
    std::map<std::string, std::string>::const_iterator it = _header.find("Host");
    if (it != _header.end())
        envp[5] = strdup(("SERVER_NAME=" + it->second).c_str());
    else
        envp[5] = strdup("SERVER_NAME=");
    envp[6] = strdup("CONTENT_TYPE=");
    envp[7] = strdup("SERVER_PORT=");
    envp[8] = strdup("PATH_INFO=");
    envp[9] = strdup("SCRIPT_NAME=");
    envp[10] = strdup("REMOTE_ADDR=");
    std::cout << std::endl << "env2" << std::endl;
    for(int i = 0; envp[i] != NULL; i++)
        printf("%s\n", envp[i]); 
    std::cout << std::endl;
    executeCgi(envp);
    ft_fre(envp);
}

void CgiExecution::functionDelete()
{
    return;
}

void ft_fre(char **str)
{
    if (str == NULL)
        return;
    for(int i = 0; str[i] != NULL; i++)
        free(str[i]);
    free(str);
}

char *ft_strdup(const char *str)
{
    int i = 0;
    char *result;
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

void CgiExecution::parsQueryString(std::string &str)
{
    (void)str;
    std::string _path = "sfgfgfs?name=guigui&comment=pars";
    std::cout << "cgiexecution" << std::endl;
    size_t t = _path.find('?');
    if (t != std::string::npos && ++t != std::string::npos)
    {
        std::string path = _path.substr(t);
        std::cout << Blue << path << Reset_Color << std::endl;
        _realPath = path;
    }
}

void CgiExecution::parsBody(std::string &str)
{
    std::cout << "cgiexecutionPost" << std::endl;
    std::cout << Green << str << Reset_Color << std::endl;
    if (str.empty())
    {
        write(1, "", 1);
        return;
    }
    write(0, str.c_str(), str.size());
}

void CgiExecution::executeCgi(char **envp)
{
    int pipe_fd[2];
    pid_t pid;
    int filefd;

    if (pipe(pipe_fd) == -1)
    {
        printf("pipe :%s", strerror(errno));
        ft_fre(envp);
        return;
    }
    pid = fork();
    if (pid == -1)
    {
        printf("fork :%s", strerror(errno));
        ft_fre(envp);
        return;
    }
    if (pid == 0)
    {
        close(pipe_fd[0]);
        if (_method == "POST")
        {
            filefd = open("hello.txt", O_RDONLY);
            if (filefd == -1)
            {
                std::cout << "error fd 2" << std::endl;
            }
            dup2(filefd, STDIN_FILENO);
            close(filefd);
        }
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
        char **tab;

        tab = (char **)malloc(2 * sizeof(char*));
        tab[0] = ft_strdup("net/cgi/hello.py");
        tab[1] = NULL;
        if (access(tab[0], X_OK) == -1)
        {
            std::cerr << "Erreur d'accès à 'cgi/hello.py' : " << strerror(errno) << std::endl;
            ft_fre(envp);
            return;
        }
        execve("net/cgi/hello.py", tab, envp);
        ft_fre(envp);
        ft_fre(tab);
        std::cout << "echec execute" << std::endl;
    }
    else
    {
        write(pipe_fd[1], _body.c_str(), _body.size());
        close(pipe_fd[1]);
        std::string     output_cgi = "";
        char            buf[10];
        ssize_t         bytes_read;
        while((bytes_read = read(pipe_fd[0], buf, sizeof(buf) - 1)) > 0)
        {
            buf[bytes_read] = '\0';
            std::cout << "output_cgi " << output_cgi << std::endl;
            output_cgi += buf;
        }
        if (!output_cgi.empty())
            std::cout << "output_cgi " << std::endl << Red << output_cgi << Reset_Color << std::endl;
        close(pipe_fd[0]);        
    }
    wait(NULL);
}
