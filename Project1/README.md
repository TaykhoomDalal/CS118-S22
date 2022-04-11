# CS118 Project 1

Name: Taykhoom Dalal & Nevin Liang \
Email: tdalal@ucla.edu & nliang868@ucla.edu \
UID: 305303123 & 705575353

## Basic Testing

Compile and start the server:

`$ gcc server.c -o server` \
`$ ./server`

Start the client and request a file:

1. Open your webbrowser of choice
2. Type in the following: http://localhost:15635/[insert_file_name]
3. Press enter

## High Level Design

At a high level, our server opens a TCP socket and listens for incoming connections. When a connection is established with a client (such as a web browser), the server reads the request from the client, parsing the file name from the request, and checking to make sure the name adheres to the specifications outligned in the project description. If the file name is valid, the server then builds an HTTP response, opens the file, and sends the HTTP response along with the contents of the file to the client for display (if it can be displayed). If the file name is invalid, the file doesn't exist, or any other errors occur while processing the client's request, the server stops serving the request and continues waiting for more connections. If no other issues occur, the server then closes the connection with the client and continues listening for more connections.

## Problems and Solutions

One of the main problems we ran into while finishing this project was figuring out an efficient way of parsing strings in C. In order to satisfy the requirements for the project, we needed to parse the HTTP request for the filename by figuring out the start and end positions and replacing any "%20" characters with spaces. C does not make it easy to work with strings so we initially just used multiple for loops and a lot of malloc and strcpy() to do the parsing. While this worked, this felt very clunky so we instead used the following resources to figure out how to parse strings more cleanly in C - [strchr](http://www.cplusplus.com/reference/cstring/strchr/) and [strstr](http://www.cplusplus.com/reference/cstring/strstr/). This reduced the amount of code made it a little easier to parse the request.

Another problem we ran into, albiet much less important, was that when we were in the initial stages of testing our server, we found that if we closed the server, and tried to reopen it within a small period of time, we would be hit with an error: "bind failed: address already in use" and we would have to wait a certain period of time before trying to run the server again. As this was quite annoying, we did some research and found the following [link](https://stackoverflow.com/questions/15198834/bind-failed-address-already-in-use) that recommend we use the setsockopt() function in the C library to allow us to set the SO_REUSEADDR option on the socket. This allows us to reuse the socket if we close and reopen the server. It was a small problem and a simple solution but it made our testing hassle free. 


## Acknowledgement of Sources

Besides the resources listed in the problems and solutions section as well as the TA slides, we didn't use any other sources to help us with our project.

Additional Libraries: 
1. #include <unistd.h> // for close
2. #include <arpa/inet.h> // for inet_ntoa (used in debugging)