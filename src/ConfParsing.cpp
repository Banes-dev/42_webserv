#include "ConfParsing.hpp"


// Constructor & Destructor
ConfParsing::ConfParsing(std::string argv) : _path(argv)
{
    return;
}

ConfParsing::ConfParsing(const ConfParsing &copy)
{
    *this = copy;
    return;
}

ConfParsing &ConfParsing::operator=(const ConfParsing &copy)
{
    if (this != &copy)
    {
        *this = copy; 
    }
    return (*this);
}

ConfParsing::~ConfParsing(void)
{
    return;
}


// Other function

std::string            ConfParsing::parsConfDeux(std::string ligne)
{
    size_t     j = 0;
    for (std::string::iterator at = ligne.begin(); at != ligne.end() ; at++)
    {
        if (!(isspace(*at)))
        {
            std::string  ligne2;
            size_t       s = ligne.find(" ", j);
            if (s != std::string::npos)
                ligne2 = ligne.substr(j, (s - j));
            while ((s < ligne.size()) && isspace(ligne[s]))
                s++;
            if (s == std::string::npos)
                break;
            std::string   ligne3 = ligne.substr(s);
            size_t       f = ligne3.find(";");
            if (f != std::string::npos)
                ligne3 = ligne3.substr(0, f);
            ligne2 = ligne2 + ';' + ligne3;
            return(ligne2);
            break;
        }
        j++;
    }
    return(ligne);
}

void ConfParsing::parsConfUn(void)
{
    std::ifstream   ifs;

    ifs.open(_path.c_str(), std::ifstream::in);
    if (!ifs)
        throw BadParsingException();
    std::string     ligne;
    std::multimap< std::string, std::vector<std::string> >  confDeux;
    while (std::getline(ifs, ligne))
    {
        size_t     j = 0;
        size_t     p = ligne.size();
        if (p == 1 && ligne.find('}') != std::string::npos)
        {
            _def.push_back(confDeux);
            confDeux.clear();
            continue;
        }
        for (std::string::iterator at = ligne.begin(); at != ligne.end() ; at++)
        {
            if (!(isspace(*at)))
            {
                std::string  ligne2;
                size_t       s = ligne.find(" ", j);
                if (s != std::string::npos)
                    ligne2 = ligne.substr(j, (s - j));
                while ((s < ligne.size()) && isspace(ligne[s]))
                    s++;
                if (s == std::string::npos)
                    break;
                std::string   ligne3 = ligne.substr(s);
                size_t       f = ligne3.find(";");
                if (f != std::string::npos)
                    ligne3 = ligne3.substr(0, f);
                size_t          end = ligne3.find('{');
                std::vector<std::string>    v3;
                if ((end != std::string::npos) && (j != 0))
                {
                    ligne3 = ligne3.substr(0, end);
                    v3.push_back(ligne3);
                    while (std::getline(ifs, ligne))
                    {
                        size_t          c = ligne.find('}');
                        if (c != std::string::npos)
                            break;
                        v3.push_back(ConfParsing::parsConfDeux(ligne));
                    }
                }
                else
                    v3.push_back(ligne3);
                confDeux.insert(std::make_pair(ligne2, v3));
//                _conf.insert(std::make_pair(ligne2, v3));
                break;
            }
            j++;
        }
    }
    ifs.close();
}

const std::list< std::multimap< std::string, std::vector<std::string> > > &   ConfParsing::getConf(void) const
{
    return (_def);
}

std::list< std::vector<std::string> >              ConfParsing::getLocation(ConfParsing const & conf) const
{
    std::list< std::vector<std::string> >      result;

    if (conf.getConf().empty()) 
    {
        throw std::runtime_error("Erreur: La liste de configurations est vide.");
    }
    const std::multimap< std::string, std::vector<std::string> > configMap = conf.getConf().front();

    // Vérifier si la clé "location" existe
    std::pair< std::multimap<std::string, std::vector<std::string> >::const_iterator, 
            std::multimap< std::string, std::vector<std::string> >::const_iterator> range = configMap.equal_range("location");
    if (range.first == range.second) 
    {
        throw std::runtime_error("Erreur: Aucune clé 'location' trouvée dans la configuration.");
    }

    // Afficher toutes les valeurs associées à "location"
//    std::cout << "Locations trouvées: " << std::endl;
    for (std::multimap< std::string, std::vector<std::string> >::const_iterator it = range.first; it != range.second; ++it)
    {
        std::vector<std::string>    temp;
//        std::cout << " - ";
        for (std::vector<std::string>::const_iterator vit = it->second.begin(); vit != it->second.end(); ++vit)
        {
            temp.push_back(*vit);
//            std::cout << *vit << " ";
        }
        result.push_back(temp);
        temp.clear();
//        std::cout << std::endl;
    }
//    std::cout << std::endl;
    return (result);
}

std::ostream &      operator<<(std::ostream & o, ConfParsing const & src)
{
    const std::list< std::multimap< std::string, std::vector<std::string> > > &     def = src.getConf(); //stock la reference

    std::list< std::multimap< std::string, std::vector<std::string> > >::const_iterator it = def.begin();
    while(it != def.end())
    {
        std::multimap< std::string, std::vector<std::string> >::const_iterator vi = it->begin();
        while (vi != it->end())
        {
            o << vi->first << "  ->  ";
            for (std::vector<std::string>::const_iterator it = vi->second.begin(); it != vi->second.end(); it++)
                o << *it << "  =>  ";
            o << std::endl;
            vi++;
        }
        o << std::endl << " list " << std::endl << std::endl;
        it++;
    }
    return (o);
}

/*
std::ostream &      operator<<(std::ostream & o, ConfParsing const & src)
{
    //get conf retourne une copie donc disparait juste apres l'appel, il faut donc utiliser une reference constante
    const std::multimap< std::string, std::vector<std::string> > &conf = src.getConf(); //stock la reference

    std::multimap< std::string, std::vector<std::string> >::const_iterator vi = conf.begin();
    while (vi != conf.end())
    {
        o << vi->first << "  ->  ";
        for (std::vector<std::string>::const_iterator it = vi->second.begin(); it != vi->second.end(); it++)
            o << *it << "  =>  ";
        o << std::endl;
        vi++;
    }
    return (o);
}*/

/*
unsigned int ConfParsing::getPort(void) const
{
    return (this->_port);
}

std::string ConfParsing::getHost(void) const
{
    return (this->_host);
}*/

/*
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
}*/
