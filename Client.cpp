#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")  
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>       
#include <chrono>    
#include <string>
using namespace std;
#include <ctime>
#include <cstdlib>
//assinging an id starting from 1000.. this is incremented and assinged each time the client starts

int main() {
    srand(static_cast<unsigned int>(time(0)));
    int flightID = 1000 + rand() % 9000;
    string flightIDStr = to_string(flightID);

    // initialzing winsock
    WSADATA wsaData;
    //client socket used to connect to the client
    SOCKET clientSocket;
    // struct to hold the IP and the Port info
    sockaddr_in serverAddr{};

    // initialzing winsock.. throwing an error if that fails..
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed.\n";
        return 1;
    }

    //creating a tcp socket.. if this fails clean up and exit.
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed.\n";
        WSACleanup();
        return 1;
    }
    // binding the socket to IP and port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Localhost for now

    // connect error. cleanup and exit
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Connection failed.\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    cout << "Connected to server!\n";

    // Sending the ID after connecting
    string idMessage = "FLIGHT_ID|" + flightIDStr;
    send(clientSocket, idMessage.c_str(), idMessage.size(), 0);
    cout << "Sent Flight ID: " << flightIDStr << endl;

    char buffer[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, buffer);
    cout << "Current working directory: " << buffer << endl;

    //reading the data from the text file
    ifstream file("data.txt");


    if (!file.is_open()) {
        cerr << "Could not open file.\n";
        perror("File open error");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    string line;
    // varibale to keep a track of teh first line cause it was all headers and labels

    bool isFirstLine = true;

    while (getline(file, line)) {
        // skipping the first line
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }

        // Removing trailing comma
        if (!line.empty() && line.back() == ',') {
            line.pop_back();
        }

        // Skiping blank lines
        if (line.empty()) continue;

        // Split line by comma
        stringstream ss(line);
        string timestamp, fuel;
        getline(ss, timestamp, ',');
        getline(ss, fuel, ',');

        // Formating message to send 
        string message = timestamp + "|" + fuel;

        // Sending message
        send(clientSocket, message.c_str(), message.size(), 0);
        cout << "Sent: " << message << endl;

        // Waiting a second
        this_thread::sleep_for(chrono::seconds(1));
    }

    // Send END message to mark flight completion
    string endSignal = "EOF";
    send(clientSocket, endSignal.c_str(), endSignal.size(), 0);

    cout << "Flight data sent. Closing client.\n";

    //// Sending a message as of now 
    //const char* message = "Hello from client";
    //send(clientSocket, message, strlen(message), 0);

    // Cleanup
    file.close();
    closesocket(clientSocket);
    WSACleanup();
    system("pause");
    return 0;
}
