#include "Utils.hpp"
#include "ConfParsing.hpp"

#include "Server.hpp"

int main(int argc, char**argv)
{
    // check args
    (void)argv;
    if (argc != 2)
    {
        std::cerr << Deny << Red << "Error wrong args\n" << Yellow << "Try : ./webserv [file].conf" << Reset_Color << std::endl;
        return (1);
    }

/*    try {
        // Check si le .conf est bon (.conf a la fin), si on peut l'ouvrir et le lire
        Server server;
        server.ParseConfigurationFile(argv[1]); // Lire le fichier et mettre les infos dans les private du server
        server.InitSocket();                    // Initialiser les sockets pour chaque server et les mettres en ecoute
        server.ManageConnection();              // Gerer les connections (plusieurs clients), differentes requetes http (get, post, etc ...), reponses http et CGI
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return (1);
    }*/
    try
    {
        ConfParsing     cp;
        cp.parsConfUn();
        std::cout << cp;
/*        std::string     aw = cp.getHost();
        std::cout << "host " << aw << std::endl;
        unsigned int    i = cp.getPort();
        std::cout << "port " << i << std::endl;*/
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    try
    {
        ConfParsing     cpp;
        cpp.parsConfUn();
        if (cpp.getConf().empty()) 
        {
            throw std::runtime_error("Erreur: La liste de configurations est vide.");
        }
        const std::multimap< std::string, std::vector<std::string> > configMap = cpp.getConf().front();

        // Vérifier si la clé "location" existe
        std::pair< std::multimap<std::string, std::vector<std::string> >::const_iterator, 
                std::multimap< std::string, std::vector<std::string> >::const_iterator> range = configMap.equal_range("location");

        if (range.first == range.second) 
        {
            throw std::runtime_error("Erreur: Aucune clé 'location' trouvée dans la configuration.");
        }

        // Afficher toutes les valeurs associées à "location"
        std::cout << "Locations trouvées: " << std::endl;
        for (std::multimap< std::string, std::vector<std::string> >::const_iterator it = range.first; it != range.second; ++it)
        {
            std::cout << " - ";
            for (std::vector<std::string>::const_iterator vit = it->second.begin(); vit != it->second.end(); ++vit)
            {
                std::cout << *vit << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
}


	return (0);
}
