#pragma once

# include <iostream>
# include <string>
# include <fstream>
# include <sstream>
# include <stdexcept>
# include <algorithm>
# include <map>
# include <vector>
# include <ctype.h>
# include <list>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <string.h>
# include "Utils.hpp"
# include <fstream>


class CgiExecution
{
    public:
        CgiExecution(std::string const & method, std::string const  & path, std::string const & body, std::string const & version, std::map<std::string, std::string> const & header);
        CgiExecution(CgiExecution const & src);
        ~CgiExecution(void);

        CgiExecution &      operator=(CgiExecution const & rhs);
        void                methodeType();
        void                parsQueryString(std::string & str);
        void                parsBody(std::string & str);
        void                executeCgi(char **envp);
        void                functionDelete();

    private:
        std::string     _env;
        std::string     _realPath;
        std::string     _method;
        std::string     _path;
        std::string     _body;
        std::string     _version;
        std::map<std::string, std::string> const &   _header;
};

void        ft_fre(char **str);
char*       ft_strdup(const char * str);