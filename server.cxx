#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    short connectPort;

    // Prompt user for the server port number
    std::cout << "Enter the server \e[4;34mport number\e[0m: ";
    std::cin >> connectPort;

    // Validate port input
    if (std::cin.fail() || connectPort <= 0 || connectPort > 65535) {
        std::cerr << "Invalid port number. Please enter a number between 1 and 65535." << std::endl;
        return -1;
    }
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(connectPort);

    // Binding the socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port " 
              << "\e[4;34m" << connectPort << "\e[0m" 
              << std::endl;

    while (true) {
        // Accepting a new connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept");
            exit(EXIT_FAILURE);
        }
        else 
        {
			std::cout << "Connection established with " << inet_ntoa(address.sin_addr) << std::endl;
		}
	
        while (true) {
            memset(buffer, 0, BUFFER_SIZE);
            int valread = read(new_socket, buffer, BUFFER_SIZE);
            if (valread <= 0) {
				std::cout << "Connection lost" << std::endl; 
				break;
            }

            std::cout << "Received: " << buffer << std::endl;

            // Respond with "pong"
            const char *pong = "pong";
            send(new_socket, pong, strlen(pong), 0);
            std::cout << "Sent: " << pong << std::endl;
            
            
        }
        
        close(new_socket); // Close connection with the current client
    }

    return 0;
}
