#include <iostream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize winsock" << std::endl;
        return 1;
    }

    // Create a socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket" << std::endl;
        WSACleanup();
        return 1;
    }

    // Set up the server address and port
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(25);  // SMTP port

    // Resolve the server hostname to an IP address
    const char* serverHostname = "mail.example.com";  // Replace with your SMTP server address
    if (inet_pton(AF_INET, serverHostname, &(serverAddress.sin_addr)) <= 0) {
        std::cerr << "Failed to resolve server hostname" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Connect to the server
    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) != 0) {
        std::cerr << "Failed to connect to server" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Receive the server's welcome message
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    std::cout << buffer << std::endl;

    // Send the email
    const std::string senderEmail = "usmanmalikse1@gmail.com";  // Replace with your email address
    const std::string recipientEmail = "usmanmalikse2@gmail.com";  // Replace with the client's email address
    const std::string subject = "Meeting Invitation";
    const std::string body = "Please join us for a meeting on June 1st at 10:00 AM.";
    const std::string emailContent =
        "From: " + senderEmail + "\r\n"
        "To: " + recipientEmail + "\r\n"
        "Subject: " + subject + "\r\n\r\n"
        + body + "\r\n";

    std::string command = "HELO\r\n";
    send(clientSocket, command.c_str(), command.size(), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    std::cout << buffer << std::endl;

    command = "MAIL FROM: <" + senderEmail + ">\r\n";
    send(clientSocket, command.c_str(), command.size(), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    std::cout << buffer << std::endl;

    command = "RCPT TO: <" + recipientEmail + ">\r\n";
    send(clientSocket);
}