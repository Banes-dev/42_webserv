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


class CgiExecution
{
    public:
        CgiExecution(std::string const & method, std::string const  & path, std::string const & body, std::map<std::string, std::string> const & header);
        CgiExecution(CgiExecution const & src);
        ~CgiExecution(void);

        CgiExecution &      operator=(CgiExecution const & rhs);
        void                methodeType(void);
        void                parsQueryString(std::string & str);
        void                execute(void);
        void                functionDelete();

    private:
        std::string     _env;
        std::string     _realPath;
        std::string     _method;
        std::string     _path;
        std::string     _body;
        std::map<std::string, std::string> const &   _header;
};