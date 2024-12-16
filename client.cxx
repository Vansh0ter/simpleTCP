#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <limits>

#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    short connectPort;
    std::string server_ip;

    // Prompt user for the server IP address
    std::cout << "Enter the server \e[4;31mIP address\e[0m: ";
    std::cin >> server_ip;

    // Prompt user for the server port number
    std::cout << "Enter the server \e[4;31mport number\e[0m: ";
    std::cin >> connectPort;

    // Validate port input
    if (std::cin.fail() || connectPort <= 0 || connectPort > 65535) {
        std::cerr << "Invalid port number. Please enter a number between 1 and 65535." << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(connectPort); // Set the port number from user input

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, server_ip.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    // Connecting to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        return -1;
    }
    else 
    {
		std::cout << "Connection established with " << server_ip << std::endl;
	}

    while (true) {
        const char *ping = "ping";
        send(sock, ping, strlen(ping), 0);
        std::cout << "Sent: ping" << std::endl;

        char buffer[BUFFER_SIZE] = {0};
        int valread = read(sock, buffer, BUFFER_SIZE);
        if(valread <= 0) 
        {
			std::cerr << "Connection lost" << std::endl;
		}
		else std::cout << "Received: " << buffer << std::endl;

        sleep(1); // Wait for a second before sending next ping
    }

    close(sock); // Close the socket when done
    return 0;
}
