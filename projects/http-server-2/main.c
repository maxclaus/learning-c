// Inclusion of the Server header, which outlines the server structure and
// functionalities.
#include "Server.h"
// Standard libraries to provide essential utilities for input/output and system
// calls.
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Prototype of the launch function, which we will define later to dictate the
// server's behavior upon activation.
void launch(struct Server *server);

// The main function: the entry point of our server application.
int main() {
  // Initialization of server settings with specific values for internet
  // protocols and communication characteristics. Specifies IPv4 as the protocol
  // for network communication.
  int domain = AF_INET;
  // Indicates the use of TCP, ensuring reliable data
  // transmission.
  int service = SOCK_STREAM;
  // Automatically selects the default protocol, TCP, for our
  // service type.
  int protocol = 0;
  // Signifies that the server should bind
  // to all available network interfaces.
  unsigned long interface = INADDR_ANY;
  // Designates the port number through which the server will
  // listen for incoming connections.
  int port = 8080;
  // Sets a limit on the number of unaccepted connections that
  // can queue up simultaneously.
  int backlog = 10;

  // The construction of the server structure, which is now ready to be deployed
  // with the parameters we've just specified.
  struct Server server = server_constructor(domain, service, protocol,
                                            interface, port, backlog, launch);

  // The pivotal call to action for the server, invoking the launch function to
  // start the server's operation.
  server.launch(&server);

  // The return of an integer value upon successful completion of the main
  // function.
  return 0;
}

// The fleshed-out launch function that embodies the server's functionality once
// it's up and running.
void launch(struct Server *server) {
  printf("Server running...waiting for connections.\n");

  char buffer[1024]; // A buffer to store incoming data from client requests.
  // HTML content to be sent in response to client requests. It includes a link
  // to an educational video.
  char *helloWorldHTML =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/html\r\n"
      "Connection: close\r\n"
      "\r\n"
      "<!DOCTYPE html>"
      "<html lang=\"en\">"
      "<head><title>Hello World</title></head>"
      "<body>"
      "<h1>Hello World!</h1>"
      "<p>Learn more advanced concepts <a target=\"_blank\" "
      "href=\"https://www.youtube.com/watch?v=uHgt8giw1LY\">here</a>.</p>"
      "</body>"
      "</html>";

  // An infinite loop that keeps the server perpetually ready to accept new
  // connections.
  while (1) {
    // Client address structure to hold the
    // details of the connecting client.
    struct sockaddr_in client_address;
    // Length of the client address structure.
    socklen_t client_len = sizeof(client_address);

    // The server accepts an incoming connection request and creates a new
    // socket for client communication.
    int new_socket =
        accept(server->socket, (struct sockaddr *)&client_address, &client_len);
    if (new_socket < 0) {
      // If the server fails to accept the connection,
      // it logs the error.
      perror("Accept failed");
      // Continues to the next iteration of the loop to await new
      // connections.
      continue;
    }

    // Confirmation that a new connection has
    // been successfully established.
    printf("Connection accepted\n");

    // Clears the buffer and reads the incoming request from the client into the
    // buffer.
    memset(buffer, 0, sizeof(buffer));
    read(new_socket, buffer, sizeof(buffer) - 1);

    // Logs the incoming request for administrative purposes.
    printf("%s\n", buffer);

    // Sends the prepared HTML content as a response to the client's request.
    write(new_socket, helloWorldHTML, strlen(helloWorldHTML));

    // Closes the socket associated with the client, ending the communication
    // gracefully.
    close(new_socket);
  }
}
