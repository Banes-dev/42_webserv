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

    // 5. Création de l'instance epoll
    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0)
		throw EpollException();

    // 6. Ajout du socket serveur à epoll
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);

    char host[NI_MAXHOST];
    if (getnameinfo((struct sockaddr*)&address, sizeof(address), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST) != 0)
        std::cerr << "Error getting address" << std::endl;
    else
        std::cout << Green << "Serveur listen : " << server_fd << " - " << host << ":" << PORT << Reset_Color << std::endl << std::endl;
}

void Server::ManageConnection()
{
	// 7. Boucle principale
    struct epoll_event events[MAX_EVENTS];
    while (running)
	{
        int event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, 60000);
        if (event_count < 0)
		{
			if (errno == EINTR)
				continue;
			throw EpollWaitException();
		}
        time_t now = time(NULL);
        for (std::map<int, time_t>::iterator it = _client_last_active.begin(); it != _client_last_active.end();)
        {
            if (now - it->second > 59)
            {
                std::cout << Red << Server::GetTime() << " " << "Timeout, client disconnect " << it->first << Reset_Color << std::endl;
                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, it->first, NULL);
                close(it->first);
                _client_last_active.erase(it++);
            }
            else
                ++it;
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
                _client_last_active[new_socket] = time(NULL);
                std::cout << Green << Server::GetTime() << " " << "New connection from : " << new_socket << Reset_Color << std::endl;

                // Met le client en mode non-bloquant
				int flags = fcntl(new_socket, F_GETFL, 0);
    			fcntl(new_socket, F_SETFL, flags | O_NONBLOCK);

                // Ajoute le client à epoll
                struct epoll_event client_ev;
                // client_ev.events = EPOLLIN | EPOLLET;
                client_ev.events = EPOLLIN | EPOLLONESHOT;
                client_ev.data.fd = new_socket;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &client_ev);
            }
            // 8.2 Requête d'un client existant
            else {
                char buffer[BUFFER_SIZE] = {0};
                int valread = read(event_fd, buffer, sizeof(buffer) - 1);

                if (valread <= 0)
				{
                    std::cout << Red << Server::GetTime() << " " << "Client disconnect " << event_fd << Reset_Color << std::endl;
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, event_fd, NULL);
                    close(event_fd);
                }
				else {
                    // _client_last_active[event_fd] = time(NULL);

                    HttpRequest request;
                    try {
                        request.ParseRequest(buffer);
                    } catch (std::exception &e) {
                        std::cerr << e.what() << std::endl;
                        close(event_fd);
                        continue;
                    }

                    // Check si keep-alive est dans les headers
                    const std::map<std::string, std::string> &headers = request.GetHeaders();
                    bool keep_alive = false;
                    std::map<std::string, std::string>::const_iterator it = headers.find("Connection");  
                    if (it != headers.end() && it->second == "keep-alive")
                        keep_alive = true;


                    // Réponse HTTP basique
                    // std::string response =
                    //     "HTTP/1.1 200 OK\r\n"
                    //     "Content-Type: text/plain\r\n"
                    //     "Content-Length: 13\r\n"
                    //     "Connection: " + std::string(keep_alive ? "keep-alive" : "close") + "\r\n"
                    //     "\r\n"
                    //     "Hello, Client!";

                    HttpResponse response;
                    try {
                        std::string path = request.GetPath();
                        if (path == "/")
                            path = "/index.html";
                        response.ServeFile(path);
                        // response.SetStatus(200);
                        // response.SetHeader("Content-Type", "text/html");
                        // response.SetBody("<html><body><h1>Hello, WebServ!</h1></body></html>");
                        // response.SetKeepAlive(true);
                    } catch (std::exception &e) {
                        std::cerr << e.what() << std::endl;
                        close(event_fd);
                        continue;
                    }
                    std::string responseStr = response.ToString();

                    // send(event_fd, response.c_str(), response.size(), 0);
                    send(event_fd, responseStr.c_str(), responseStr.size(), 0);
                    if (keep_alive == false)
                    {
                        std::cout << Red << Server::GetTime() << " " << "Client disconnect " << event_fd << Reset_Color << std::endl;
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, event_fd, NULL);
                        close(event_fd);
                    }

                    std::cout << request.GetMethod() << std::endl << request.GetPath() << std::endl << request.GetBody() << request.GetVersion() << std::endl;
//                    char    *data = getenv("QUERY_STRING");
//                    printf(" data %s\n", data);
                    for (std::map<std::string, std::string>::const_iterator itt = request.GetHeaders().begin(); itt != request.GetHeaders().end(); itt++)
                    {
                        std::cout << itt->first << " - " << itt->second << std::endl;
                    }
                    CgiExecution    abc(request.GetMethod(), request.GetPath(), request.GetBody(), request.GetVersion(), request.GetHeaders());
                    abc.methodeType();
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
