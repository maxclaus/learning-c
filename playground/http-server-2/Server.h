// Server.h - Header file for the server structure and functions

#ifndef SERVER_H    // Include guard to ensure this file is only included once
#define SERVER_H

#include <sys/socket.h> // Necessary for socket creation and binding
#include <netinet/in.h> // Provides constants and structures needed for internet domain addresses

// Server structure with all the necessary fields to set up a socket
struct Server {
    int domain; // Domain of the socket, e.g., AF_INET for IPv4
    int service; // Service type, e.g., SOCK_STREAM for TCP
    int protocol; // Protocol type, usually 0 for default protocol
    unsigned long interface; // Interface address, e.g., INADDR_ANY to bind to all available interfaces
    int port; // Port number to listen on
    int backlog; // Number of allowed pending connections
    struct sockaddr_in address; // Internet address structure
    int socket; // File descriptor for the server socket
    void (*launch)(struct Server*); // Function pointer to the server launch function
};

// Constructor for the server, takes all necessary parameters to initialize a server instance
struct Server server_constructor(int domain, int service, int protocol,
                                 unsigned long interface, int port, int backlog,
                                 void (*launch)(struct Server*));

#endif // SERVER_H
