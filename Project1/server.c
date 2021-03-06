/* PLEASE include these headers */
#include <stdio.h> // for printf
#include <stdlib.h> // for exit()
#include <string.h> // for strlen() and strcpy()
#include <errno.h> // for errno and perror
#include <sys/types.h> // for socket(), bind(), and accept()
#include <sys/socket.h> // socket(), bind(), listen(), accept()
#include <sys/wait.h> // waitpid()
#include <netinet/in.h> // struct sockaddr_in, htons()
#include <unistd.h> // for close
#include <arpa/inet.h> // for inet_ntoa
#define MYPORT 15635 /* Avoid reserved ports */
#define BACKLOG 10 /* pending connections queue size */
#define MESSAGE_SIZE 1024

char* replace_http_space(char* str) {
  char* res = malloc(strlen(str) + 1); // allocate memory for the new string
  memset(res, '\0', strlen(str) + 1); // set the new string to null

  char* temp;
  int index = 0;
  for(;;) // loop until the end of the string
  {
      temp = strstr(str, "%20"); // find the first occurrence of "%20"
      if(temp == NULL) // if no more "%20" is found
      {
          strcpy(res + index, str); // copy the rest of the string
          return res; // return the new string
      }
      else // if "%20" is found
      {
          strncpy(res + index, str, temp - str); // copy the string before the "%20"
          index += temp - str; // update the index
          str = temp + 3; // update the string
          res[index] = ' '; // replace the "%20" with a space
          index += 1; // update the index
      }
  }

  return res;
}

char* get_filename(char *request) {
  char* start = strchr(request, '/') + 1; // find the first '/', and then skip the '/'

  for(int i = 0; i < strlen(start); i++) {
      if(start[i] == ' ') { // find the space after the file name
          char* fname = malloc(i + 1);
          strncpy(fname, start, i);
          fname[i] = '\0'; // terminate the string
          char* fname_replaced = replace_http_space(fname); // replace any "%20" with a space
          free(fname);
          return fname_replaced; // return the file name
      }
  }

  return NULL; // if no file name is found
}

void send_response(int client_fd, char* filename) {

  char* headers = malloc(MESSAGE_SIZE);
  memset(headers, '\0', MESSAGE_SIZE);
  strcpy(headers, "HTTP/1.1 200 OK\r\n");
  strcat(headers, "Connection: close\r\n");

  char* content_type;

  if (strstr(filename, ".") == NULL){
    content_type = "Content-Type: application/octet-stream\r\n";
  }
  else if(strstr(filename, ".html") != NULL) {
    content_type = "Content-Type: text/html\r\n";
  }
  else if(strstr(filename, ".txt") != NULL) {
    content_type = "Content-Type: text/plain\r\n";
  }
  else if(strstr(filename, ".jpg") != NULL) {
    content_type = "Content-Type: image/jpeg\r\n";
  }
  else if(strstr(filename, ".png") != NULL) {
    content_type = "Content-Type: image/png\r\n";
  }
  else if(strstr(filename, ".gif") != NULL) {
    content_type = "Content-Type: image/gif\r\n";
  }

  strcat(headers, content_type);
  strcat(headers, "\r\n");

  FILE* fd = fopen(filename, "r");
  if(fd == NULL) {
    perror("fopen");
    return;
  }

  // find the size of the file
  fseek(fd, 0, SEEK_END);
  int file_size = ftell(fd);
  fseek(fd, 0, SEEK_SET);

  // allocate memory for the file
  char* buffer = malloc(file_size);
  memset(buffer, '\0', file_size);

  // send the headers
  int nbytes;
  if((nbytes = send(client_fd, headers, strlen(headers), 0)) == -1) {
    perror("send");
    return;
  }
  
  // read the file into the buffer and send it
  int bytes_read = fread(buffer, 1, file_size, fd);
  if((nbytes = send(client_fd, buffer, bytes_read, 0)) == -1) {
    perror("send");
    return;
  }

  fclose(fd);
  free(buffer);
  free(headers);
  return;
}

int main(int argc, char *argv[])
{
  int sockfd, new_fd; /* listen on sock_fd, new connection on new_fd */
  struct sockaddr_in server_addr; /* my address */
  struct sockaddr_in client_addr; /* connector addr */
  int sin_size;
  char* filename;

  // Create a socket
  while((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) //AF_INET and PF_INET are the same
  {
    perror("socket");
  }

  // Set up the address structure
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(MYPORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  // Bind the socket to the address
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)); //reuse the port (prevents "Address already in use" error)
  while(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) 
  {
    perror("bind");
  }

  // Listen for connections
  while(listen(sockfd, BACKLOG) == -1) 
  {
    perror("listen");
  }

  for(;;) 
  {
    // Accept a connection
    sin_size = sizeof(struct sockaddr_in);
    if ((new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size)) == -1) {
      perror("accept");
      continue;
    }
    // printf("Server: got connection from %s\n", inet_ntoa(client_addr.sin_addr));

    // read from the socket
    char buffer[MESSAGE_SIZE];
    int nbytes;
    if ((nbytes = recv(new_fd, buffer, MESSAGE_SIZE, 0)) == -1) { //recv is usually used to read from a socket whereas read is used to read from a file
      perror("recv");
      continue;
    }
    
    // printf("Server: received %d bytes\n", nbytes); //debugging
    // printf("%s", buffer); //debugging

    filename = get_filename(buffer);

    if (filename == NULL)
    {
      close(new_fd);
      free(filename);
      continue;
    }

    // printf("Server: parsed filename: %s\n", filename); //debugging

    send_response(new_fd, filename);

    close(new_fd);
    free(filename);
  }
  close(sockfd);
  return 0;
}
