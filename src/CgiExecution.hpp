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
	private:
        std::string _env;
        std::string _realPath;
        std::string _method;
        std::string _path;
        std::string _body;
        std::string _version;
        std::map<std::string, std::string> const &_header;
    public:
        CgiExecution(const std::string &method, const std::string &path, const std::string &body, const std::string &version, const std::map<std::string, std::string> &header);
        CgiExecution(const CgiExecution &src);
		CgiExecution &operator=(const CgiExecution &copy);
        ~CgiExecution(void);

		// Other function
        void methodeType(void);
        void parsQueryString(std::string &str);
        void parsBody(std::string &str);
        void executeCgi(char **envp);
        void functionDelete(void);
};

void        ft_fre(char **str);
char*       ft_strdup(const char * str);