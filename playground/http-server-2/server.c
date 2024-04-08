// Example based on
// https://towardsdev.com/crafting-a-simple-web-server-in-c-ebf4cbab1973

// Server.c - Implementation of the server structure and functions

#include "Server.h" // Incorporates the Server structure and its related declarations from Server.h.
#include <stdio.h> // Grants us the use of standard input/output functions for logging and error messages.
#include <stdlib.h> // Provides essential functions such as exit() for terminating the program.
#include <string.h> // Allows us to work with strings, which will be useful for handling text data.
#include <unistd.h> // Gives access to the POSIX operating system API for various system calls.

// The server_constructor function is like the foreman of our server
// construction site.
struct Server server_constructor(int domain, int service, int protocol,
                                 unsigned long interface, int port, int backlog,
                                 void (*launch)(struct Server *)) {
  struct Server server; // We start by creating an instance of the Server
                        // struct, akin to laying the foundation.

  // Now we fill in the details, setting each property to align with the
  // server's intended behavior:

  // The family of protocols that the server will use (IPv4/IPv6).
  server.domain = domain;
  // The type of service, like SOCK_STREAM for TCP.
  server.service = service;
  // The specific protocol (usually the default
  // protocol for the service type).
  server.protocol = protocol;
  // The network interface on which the server will listen.
  server.interface = interface;
  // The port number, akin to an address where the server
  // will receive data.

  server.port = port;
  // The number of connections that can wait in line
  server.backlog = backlog;
  // if the server is busy.

  // The address is set up just like an envelope's address, specifying
  // where to receive the mail, or in this case, data packets: Sets the
  // address family, again specifying IPv4/IPv6.
  server.address.sin_family = domain;
  // The port number is translated into the format the network
  // stack expects.
  server.address.sin_port = htons(port);
  // We specify the interface address, often set to
  // INADDR_ANY to accept any incoming interface.
  server.address.sin_addr.s_addr = htonl(interface);

  // With the address set, we construct the mailbox, or socket, that will
  // accept the data: This system call crafts our socket.
  server.socket = socket(domain, service, protocol);

  // If the mailbox can't be built, we log the error
  // and halt construction.
  if (server.socket < 0) {
    perror("Failed to create the socket.");
    exit(EXIT_FAILURE);
  }

  // Once the socket is created, we need to attach the address to it, much
  // like assigning a mailbox to a house:
  if (bind(server.socket, (struct sockaddr *)&server.address,
           sizeof(server.address)) < 0) {
    // If the address can't be attached,
    // we log and abort.
    perror("Failed to bind the socket.");
    exit(EXIT_FAILURE);
  }

  // Now we tell the server to start listening for any mail, or data, sent
  // to this address:
  if (listen(server.socket, server.backlog) < 0) {
    // If the server can't start
    // listening, we log the error
    // and cease operations.
    perror("Failed to listen on the socket.");
    exit(EXIT_FAILURE);
  }

  // We're almost ready to open for business. We just need to set up the
  // launch function: This function will be defined later, detailing how
  // to handle incoming mail.
  server.launch = launch;

  // At last, our server is constructed, fully set up, and ready to go:
  // The fully built server is returned, ready for use.
  return server;
}
