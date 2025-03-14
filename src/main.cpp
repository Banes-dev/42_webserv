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

    try {
        // Check si le .conf est bon (.conf a la fin), si on peut l'ouvrir et le lire
        Server server;
        server.ParseConfigurationFile(argv[1]); // Lire le fichier et mettre les infos dans les private du server
        server.InitSocket();                    // Initialiser les sockets pour chaque server et les mettres en ecoute
        server.ManageConnection();              // Gerer les connections (plusieurs clients), differentes requetes http (get, post, etc ...), reponses http et CGI
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return (1);
    }

/*    try
    {
        ConfParsing     cp(argv[1]);
        cp.parsConfUn();
        std::cout << cp;
//        std::string     aw = cp.getHost();
//        std::cout << "host " << aw << std::endl;
//        unsigned int    i = cp.getPort();
//        std::cout << "port " << i << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }*/

    try
    {
        ConfParsing     cpp(argv[1]);
        cpp.parsConfUn();
        std::list< std::vector<std::string> >      result;
        result = cpp.getLocation(cpp);
        for (std::list< std::vector<std::string> >::iterator it = result.begin(); it != result.end(); it++)
        {
            for (std::vector<std::string>::const_iterator vit = it->begin(); vit != it->end(); ++vit)
            {
                std::cout << *vit << " ";
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
}


	return (0);
}
