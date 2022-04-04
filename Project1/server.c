/* PLEASE include these headers */
#include <stdio.h> // for printf
#include <stdlib.h> // for exit()
#include <string.h>
#include <errno.h> // for errno and perror
#include <sys/types.h> // for socket(), bind(), and accept()
#include <sys/socket.h> // socket(), bind(), listen(), accept()
#include <sys/wait.h> // waitpid()
#include <netinet/in.h> // struct sockaddr_in, htons()
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <arpa/inet.h> // for inet_ntoa
#define MYPORT 15635 /* Avoid reserved ports */
#define BACKLOG 10 /* pending connections queue size */
#define MESSAGE_SIZE 1024

char* parse_request(char *request) {

    char* start = strchr(request, '/') + 1; // find the first '/', and then skip the '/'

    for(int i = 0; i < strlen(start); i++) {
        if(start[i] == ' ') { // find the space after the file name
            char* fname = malloc(i + 1);
            strncpy(fname, start, i);
            fname[i] = '\0'; // terminate the string
            return fname; // return the file name
        }
    }

    return NULL; // if no file name is found
}

int main(int argc, char *argv[])
{
  int sockfd, new_fd; /* listen on sock_fd, new connection on new_fd */
  struct sockaddr_in server_addr; /* my address */
  struct sockaddr_in client_addr; /* connector addr */
  int sin_size;
  char* filename;

  // Create a socket
  if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) //AF_INET and PF_INET are the same
  {
    perror("socket");
    exit(-1);
  }

  // Set up the address structure
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(MYPORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  // Bind the socket to the address
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)); //reuse the port (prevents "Address already in use" error)
  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) 
  {
    perror("bind");
    exit(-1);
  }

  // Listen for connections
  if (listen(sockfd, BACKLOG) == -1) 
  {
    perror("listen");
    exit(-1);
  }

  for(;;) 
  {
    // Accept a connection
    sin_size = sizeof(struct sockaddr_in);
    if ((new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size)) == -1) {
      perror("accept");
      continue;
    }
    printf("Server: got connection from %s\n", inet_ntoa(client_addr.sin_addr));
    
    // read from the socket
    char buffer[MESSAGE_SIZE];
    int nbytes;
    if ((nbytes = recv(new_fd, buffer, MESSAGE_SIZE, 0)) == -1) { //recv is usually used to read from a socket whereas read is used to read from a file
      perror("recv");
      exit(-1);
    }
    
    printf("Server: received %d bytes\n", nbytes);
    printf("Server: received the following message\n%s", buffer);

    filename = parse_request(buffer);
    printf("Server: parsed filename: %s\n", filename);

    close(new_fd);
    break;
  }
  close(sockfd);
  free(filename);
  return 0;
}
