#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>

#define buff_size 1024

#define PORT 3000

int main(int argc, char *argv[]) {

    int new_socket;

    // Socket Descriptor
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Socket Address structure
    sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[buff_size] = {0};

    char *message = "Hello from server....\n";

    int opt = 1;

    // Check creation of socket
    if(server_fd == 0) {
        perror("server: socket() failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attach socket to <PORT>
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("server: setsocketopt");
        exit(EXIT_FAILURE);
    }

    // Set the structure variable with values
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attach socket to <PORT>
    if(bind(server_fd, (sockaddr*) &address, addrlen) < 0) {
        perror("server: bind() failed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 3) < 0) {
        perror("server: listen");
        exit(EXIT_FAILURE);
    }

    if((new_socket = accept(server_fd, (sockaddr*) &address, (socklen_t*) &addrlen)) < 0) {
        perror("server: accept");
        exit(EXIT_FAILURE);
    }

    int valread = read(new_socket, buffer, buff_size);
    std::cout<<"server:\n\tbuffer : "<<buffer;
    send(new_socket, message, strlen(message), 0);
    std::cout<<"\nserver: \n\tmessage sent : "<<message;
    return 0;

}