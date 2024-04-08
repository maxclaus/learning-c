// Example based on
// https://dev.to/jeffreythecoder/how-i-built-a-simple-http-server-from-scratch-using-c-739
// https://github.com/JeffreytheCoder/Simple-HTTP-Server

// The documentation for most of the external functions and variables
// referenced on this program can be found by using the manual (man command):
//  - man socket
//  - man ip
//  - man pthread_detach
//  -

// TODO: Converto to epoll later
// https://kaleid-liner.github.io/blog/2019/06/02/epoll-web-server.html

#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
// #include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 104857600

const char *get_file_extension(const char *file_name) {
  const char *dot = strrchr(file_name, '.');
  if (!dot || dot == file_name) {
    return "";
  }
  return dot + 1;
}

const char *get_mime_type(const char *file_ext) {
  if (strcasecmp(file_ext, "html") == 0 || strcasecmp(file_ext, "htm") == 0) {
    return "text/html";
  } else if (strcasecmp(file_ext, "txt") == 0) {
    return "text/plain";
  } else if (strcasecmp(file_ext, "jpg") == 0 ||
             strcasecmp(file_ext, "jpeg") == 0) {
    return "image/jpeg";
  } else if (strcasecmp(file_ext, "png") == 0) {
    return "image/png";
  } else {
    return "application/octet-stream";
  }
}

bool case_insensitive_compare(const char *s1, const char *s2) {
  while (*s1 && *s2) {
    if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
      return false;
    }
    s1++;
    s2++;
  }
  return *s1 == *s2;
}

char *get_file_case_insensitive(const char *file_name) {
  DIR *dir = opendir(".");
  if (dir == NULL) {
    perror("opendir");
    return NULL;
  }

  struct dirent *entry;
  char *found_file_name = NULL;
  while ((entry = readdir(dir)) != NULL) {
    if (case_insensitive_compare(entry->d_name, file_name)) {
      found_file_name = entry->d_name;
      break;
    }
  }

  closedir(dir);
  return found_file_name;
}

char *url_decode(const char *src) {
  size_t src_len = strlen(src);
  char *decoded = malloc(src_len + 1);
  size_t decoded_len = 0;

  // decode %2x to hex
  for (size_t i = 0; i < src_len; i++) {
    if (src[i] == '%' && i + 2 < src_len) {
      int hex_val;
      sscanf(src + i + 1, "%2x", &hex_val);
      decoded[decoded_len++] = hex_val;
      i += 2;
    } else {
      decoded[decoded_len++] = src[i];
    }
  }

  // add null terminator
  decoded[decoded_len] = '\0';
  return decoded;
}

void build_http_response(const char *file_name, const char *file_ext,
                         char *response, size_t *response_len) {
  // build HTTP header
  const char *mime_type = get_mime_type(file_ext);
  char *header = (char *)malloc(BUFFER_SIZE * sizeof(char));
  snprintf(header, BUFFER_SIZE,
           "HTTP/1.1 200 OK\r\n"
           "Content-Type: %s\r\n"
           "\r\n",
           mime_type);

  // if file not exist, response is 404 Not Found
  int file_fd = open(file_name, O_RDONLY);
  if (file_fd == -1) {
    snprintf(response, BUFFER_SIZE,
             "HTTP/1.1 404 Not Found\r\n"
             "Content-Type: text/plain\r\n"
             "\r\n"
             "404 Not Found");
    *response_len = strlen(response);
    return;
  }

  // get file size for Content-Length
  struct stat file_stat;
  fstat(file_fd, &file_stat);
  off_t file_size = file_stat.st_size;

  // copy header to response buffer
  *response_len = 0;
  memcpy(response, header, strlen(header));
  *response_len += strlen(header);

  // copy file to response buffer
  ssize_t bytes_read;
  while ((bytes_read = read(file_fd, response + *response_len,
                            BUFFER_SIZE - *response_len)) > 0) {
    *response_len += bytes_read;
  }
  free(header);
  close(file_fd);
}

void *handle_client(void *arg) {
  int client_fd = *((int *)arg);
  char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

  // receive request data from client and store into buffer
  ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
  if (bytes_received > 0) {
    // check if request is GET
    regex_t regex;
    regcomp(&regex, "^GET /([^ ]*) HTTP/1", REG_EXTENDED);
    regmatch_t matches[2];

    if (regexec(&regex, buffer, 2, matches, 0) == 0) {
      // extract filename from request and decode URL
      buffer[matches[1].rm_eo] = '\0';
      const char *url_encoded_file_name = buffer + matches[1].rm_so;
      char *file_name = url_decode(url_encoded_file_name);

      // get file extension
      char file_ext[32];
      strcpy(file_ext, get_file_extension(file_name));

      // build HTTP response
      char *response = (char *)malloc(BUFFER_SIZE * 2 * sizeof(char));
      size_t response_len;
      build_http_response(file_name, file_ext, response, &response_len);

      // send HTTP response to client
      send(client_fd, response, response_len, 0);

      free(response);
      free(file_name);
    }
    regfree(&regex);
  }
  close(client_fd);
  free(arg);
  free(buffer);
  return NULL;
}

// https://github.com/openbsd/src/blob/master/sys/sys/socket.h

int main(int argc, char *argv[]) {
  int server_fd;
  struct sockaddr_in server_addr;

  // create server socket
  // The socket() function shall create an unbound socket in a communications
  // domain, and return a file descriptor that can be used in later function
  // calls that operate on sockets.
  // - domain -> AF_INET: Internet domain sockets for use with IPv4 addresses.
  // - type   -> SOCK_STREAM: Byte-stream socket.
  // - protocol -> 0 (automatically infer the type)
  // https://pubs.opengroup.org/onlinepubs/9699919799/functions/socket.html
  // https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_socket.h.html
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // config socket
  // AF_INET: Internet domain sockets for use with IPv4 addresses.
  server_addr.sin_family = AF_INET;
  // INADDR_ANY: IPv4 wildcard address.
  // https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/netinet_in.h.html
  server_addr.sin_addr.s_addr = INADDR_ANY;
  // htons() function shall return the argument value converted from host to
  // network byte order. https://pubs.opengroup.org/onlinepubs/9699919799/
  server_addr.sin_port = htons(PORT);

  // bind socket to port
  // The bind() function shall assign a local socket address address to a socket
  // identified by descriptor socket that has no local socket address assigned.
  // Sockets created with the socket() function are initially unnamed; they are
  // identified only by their address family.
  // https://pubs.opengroup.org/onlinepubs/9699919799/functions/bind.html#
  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // listen for connections
  // The listen() function shall mark a connection-mode socket, specified by the
  // socket argument, as accepting connections.
  // https://pubs.opengroup.org/onlinepubs/9699919799/functions/listen.html#
  if (listen(server_fd, 10) < 0) {
    perror("listen failed");
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d\n", PORT);
  while (1) {
    // client info
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int *client_fd = malloc(sizeof(int));

    // accept client connection
    // The accept() function shall extract the first connection on the queue of
    // pending connections, create a new socket with the same socket type
    // protocol and address family as the specified socket, and allocate a new
    // file descriptor for that socket. The file descriptor shall be allocated
    // as described in File Descriptor Allocation.
    // https://pubs.opengroup.org/onlinepubs/9699919799/functions/accept.html#
    if ((*client_fd = accept(server_fd, (struct sockaddr *)&client_addr,
                             &client_addr_len)) < 0) {
      perror("accept failed");
      continue;
    }

    // create a new thread to handle client request
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, handle_client, (void *)client_fd);
    pthread_detach(thread_id);
  }

  close(server_fd);
  return 0;
}
