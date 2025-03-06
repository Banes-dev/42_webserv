#include "Server.hpp"


// Constructor & Destructor
Server::Server(void)
{
}

Server::Server(const Server &copy)
{
	*this = copy;
}

Server &Server::operator=(const Server &copy)
{
	(void)copy;
	return (*this);
}

Server::~Server(void)
{
}


// Other function
void Server::ParseConfigurationFile(std::string arg)
{
	std::cout << Green << "Configuration file : " << arg << " parsed" << Reset_Color << std::endl;
}

bool running = true;
void handle_signal(int signal)
{
    if (signal == SIGINT)
        running = false;
}
void Server::InitSocket(void)
{
	signal(SIGINT, handle_signal);

    // 1. Création du socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0)
		throw SocketException();
	int flags = fcntl(server_fd, F_GETFL, 0);
    fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);

    // 2. Configuration de l'adresse
    address.sin_family = AF_INET;
    // address.sin_addr.s_addr = INADDR_ANY;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(PORT);

	addrlen = sizeof(address);

    // 3. Liaison du socket au port
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
		throw BindException();

    // 4. Mise en écoute
    if (listen(server_fd, SOMAXCONN) < 0)
		throw ListenException();

    char host[NI_MAXHOST];
    if (getnameinfo((struct sockaddr*)&address, sizeof(address), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST) != 0)
        std::cerr << "Error getting address" << std::endl;
    else
        std::cout << Green << "Serveur listen : " << server_fd << " - " << host << ":" << PORT << Reset_Color << std::endl;

    // 5. Création de l'instance epoll
    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0)
		throw EpollException();

    // 6. Ajout du socket serveur à epoll
    struct epoll_event ev;
    ev.events = EPOLLIN;  // Surveille les nouvelles connexions
    ev.data.fd = server_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);
}

void Server::ManageConnection(void)
{
	// 7. Boucle principale
    struct epoll_event events[MAX_EVENTS];
    while (running)
	{
        int event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (event_count < 0)
		{
			if (errno == EINTR)
				continue;
			throw EpollWaitException();
		}

        // 8. Parcours des événements détectés
        for (int i = 0; i < event_count; i++)
		{
            int event_fd = events[i].data.fd;

            // 8.1 Nouvelle connexion entrante
            if (event_fd == server_fd)
			{
                int new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
                if (new_socket < 0)
				{
                    std::cerr << "Accept error" << std::endl;
                    continue;
                }
                std::cout << Green << "New connection from : " << new_socket << " - " << server_fd << Reset_Color << std::endl;

                // Met le client en mode non-bloquant
				int flags = fcntl(new_socket, F_GETFL, 0);
    			fcntl(new_socket, F_SETFL, flags | O_NONBLOCK);

                // Ajoute le client à epoll
                struct epoll_event client_ev;
                client_ev.events = EPOLLIN | EPOLLET; // Lecture en mode Edge Triggered
                client_ev.data.fd = new_socket;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &client_ev);
            }
            // 8.2 Requête d'un client existant
            else {
                char buffer[BUFFER_SIZE] = {0};
                int valread = read(event_fd, buffer, sizeof(buffer) - 1);

                // std::cout << "valread : " << valread << std::endl;
                if (valread <= 0)
				{
                    // Déconnexion du client
                    std::cout << Red << "Client disconnect " << event_fd << " - " << server_fd << Reset_Color << std::endl << std::endl;
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, event_fd, NULL);
                    close(event_fd);
                }
				else {
                    HttpRequest request;
                    try {
                        request.ParseRequest(buffer);
                    } catch (std::exception &e) {
                        std::cerr << e.what() << std::endl;
                        close(event_fd);
                        continue;
                    }
                    // if (request.ParseRequest(buffer) == 1)
                    // {
                    //     std::cerr << "Requête HTTP invalide" << std::endl;
                    //     close(event_fd);
                    //     continue;
                    // }

                    // Réponse HTTP basique
                    std::string response =
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/plain\r\n"
                        "Content-Length: 13\r\n"
                        "\r\n"
                        "Hello, Client!";

                    send(event_fd, response.c_str(), response.size(), 0);
                }
            }
        }
    }
    
    // 9. Fermeture du socket client
    close(server_fd);
    close(epoll_fd);
	std::cout << std::endl << std::endl << Deny << Red << " Server close" << Reset_Color << std::endl << std::endl;
}

std::string Server::GetTime(void)
{
    time_t actuel = time(0);
    tm *ltm = localtime(&actuel);
    std::ostringstream timeStr;
    timeStr << "[" 
               << (1900 + ltm->tm_year) << "-"  // Année
               << std::setw(2) << std::setfill('0') << (1 + ltm->tm_mon) << "-"  // Mois avec 0 devant
               << std::setw(2) << std::setfill('0') << ltm->tm_mday << " "       // Jour avec 0 devant
               << ltm->tm_hour << ":"          // Heure
               << ltm->tm_min << ":"           // Minute
               << ltm->tm_sec << "]";          // Seconde

    return (timeStr.str());
}
