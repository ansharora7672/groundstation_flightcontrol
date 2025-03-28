#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")  
#include <winsock2.h>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <sstream>
#include <fstream> 

using namespace std;

//// function to handle clients. this will create a thread. supporting multithreading..
//void handle_client(SOCKET clientSocket) {
//    char buffer[1024] = { 0 };
//
//    // Receive message
//    int bytesReceived;
//
//    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0)) > 0) {
//        buffer[bytesReceived] = '\0';
//        cout << "Received from client: " << buffer << endl;
//
//        if (string(buffer) == "EOF") {
//            cout << "Client finished sending flight data.\n";
//            break;
//        }
//    }
//
//    closesocket(clientSocket);
//}

//void handle_client(SOCKET clientSocket) {
//    char buffer[1024];
//    int bytesReceived;
//
//    vector<float> fuelReadings;
//
//    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0)) > 0) {
//        buffer[bytesReceived] = '\0'; // Make sure string is null-terminated
//        string receivedMsg(buffer);
//
//        if (receivedMsg == "EOF") {
//            cout << "Client finished sending flight data." << endl;
//            break;
//        }
//
//        // Expected format: timestamp|fuel
//        size_t separatorPos = receivedMsg.find('|');
//        if (separatorPos != string::npos) {
//            string timestamp = receivedMsg.substr(0, separatorPos);
//            string fuelStr = receivedMsg.substr(separatorPos + 1);
//
//            try {
//                float fuel = stof(fuelStr); // convert string to float
//                fuelReadings.push_back(fuel);
//
//                cout << "Received: [" << timestamp << "] Fuel = " << fuel << endl;
//            }
//            catch (exception& e) {
//                cerr << "Invalid fuel value received: " << fuelStr << endl;
//            }
//        }
//        else {
//            cerr << "Invalid message format: " << receivedMsg << endl;
//        }
//    }
//
//    // After receiving all data: compute average
//    if (!fuelReadings.empty()) {
//        float totalFuel = 0.0f;
//        for (float fuel : fuelReadings) {
//            totalFuel += fuel;
//        }
//
//        float averageFuel = totalFuel / fuelReadings.size();
//
//        cout << "\n---- Flight Summary ----" << endl;
//        cout << "Total readings: " << fuelReadings.size() << endl;
//        cout << "Average fuel: " << averageFuel << " units" << endl;
//        cout << "------------------------" << endl;
//    }
//    else {
//        cout << "No fuel data received.\n";
//    }
//
//    closesocket(clientSocket);
//}


//void handle_client(SOCKET clientSocket) {
//    char buffer[1024];
//    int bytesReceived;
//    int flightID = -1;
//    vector<float> fuelReadings;
//
//    // === Step 1: Receive the Flight ID ===
//    bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
//    if (bytesReceived <= 0) {
//        cerr << "[?] Failed to receive flight ID.\n";
//        closesocket(clientSocket);
//        return;
//    }
//
//    buffer[bytesReceived] = '\0';
//    string receivedMsg(buffer);
//
//    if (receivedMsg.rfind("FLIGHT_ID|", 0) == 0) {
//        string idStr = receivedMsg.substr(10);
//        try {
//            flightID = stoi(idStr);
//            cout << "[Flight " << flightID << "] Connected\n";
//        }
//        catch (...) {
//            cerr << "[?] Invalid flight ID format: " << idStr << endl;
//            closesocket(clientSocket);
//            return;
//        }
//    }
//    else {
//        cerr << "[?] First message was not FLIGHT_ID|...\n";
//        closesocket(clientSocket);
//        return;
//    }
//
//    // === Step 2: Start Receiving Telemetry Data ===
//    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0)) > 0) {
//        buffer[bytesReceived] = '\0';
//        receivedMsg = string(buffer);
//
//        if (receivedMsg == "EOF") {
//            cout << "[Flight " << flightID << "] Transmission complete.\n";
//            break;
//        }
//
//        // Format: timestamp|fuel
//        size_t separatorPos = receivedMsg.find('|');
//        if (separatorPos != string::npos) {
//            string timestamp = receivedMsg.substr(0, separatorPos);
//            string fuelStr = receivedMsg.substr(separatorPos + 1);
//
//            stringstream ss(fuelStr);
//            float fuel;
//            if (ss >> fuel) {
//                fuelReadings.push_back(fuel);
//                cout << "[Flight " << flightID << "] Received: [" << timestamp << "] Fuel = " << fuel << endl;
//            }
//            else {
//                cerr << "[Flight " << flightID << "] Invalid fuel value: " << fuelStr << endl;
//            }
//        }
//        else {
//            cerr << "[Flight " << flightID << "] Invalid message format: " << receivedMsg << endl;
//        }
//    }
//
//    // === Step 3: Show Summary ===
//    if (!fuelReadings.empty()) {
//        float totalFuel = 0.0f;
//        for (float fuel : fuelReadings) {
//            totalFuel += fuel;
//        }
//        float averageFuel = totalFuel / fuelReadings.size();
//
//        cout << "\n[Flight " << flightID << "] ---- Flight Summary ----" << endl;
//        cout << "[Flight " << flightID << "] Total readings: " << fuelReadings.size() << endl;
//        cout << "[Flight " << flightID << "] Average fuel: " << averageFuel << " units" << endl;
//        cout << "[Flight " << flightID << "] --------------------------\n" << endl;
//    }
//    else {
//        cout << "[Flight " << flightID << "] No valid fuel data received.\n";
//    }
//
//    closesocket(clientSocket);
//}

void handle_client(SOCKET clientSocket) {
    char buffer[1024];
    int bytesReceived;
    int flightID = -1;
    vector<float> fuelReadings;

    bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        cerr << "[?] Failed to receive flight ID.\n";
        closesocket(clientSocket);
        return;
    }

    buffer[bytesReceived] = '\0';
    string receivedMsg(buffer);

    if (receivedMsg.rfind("FLIGHT_ID|", 0) == 0) {
        try {
            flightID = stoi(receivedMsg.substr(10));
            cout << "[Flight " << flightID << "] Connected\n";
        }
        catch (...) {
            cerr << "[?] Invalid flight ID format.\n";
            closesocket(clientSocket);
            return;
        }
    }
    else {
        cerr << "[?] First message must be FLIGHT_ID.\n";
        closesocket(clientSocket);
        return;
    }

    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesReceived] = '\0';
        receivedMsg = string(buffer);

        if (receivedMsg == "EOF") {
            cout << "[Flight " << flightID << "] Transmission complete.\n";
            break;
        }

        size_t sep = receivedMsg.find('|');
        if (sep != string::npos) {
            string timestamp = receivedMsg.substr(0, sep);
            string fuelStr = receivedMsg.substr(sep + 1);

            stringstream ss(fuelStr);
            float fuel;
            if (ss >> fuel) {
                fuelReadings.push_back(fuel);
                cout << "[Flight " << flightID << "] Received: [" << timestamp << "] Fuel = " << fuel << endl;
            }
            else {
                cerr << "[Flight " << flightID << "] Invalid fuel: " << fuelStr << endl;
            }
        }
        else {
            cerr << "[Flight " << flightID << "] Invalid message format: " << receivedMsg << endl;
        }
    }

    if (!fuelReadings.empty()) {
        float totalFuel = 0.0f;
        for (float fuel : fuelReadings)
            totalFuel += fuel;

        float averageFuel = totalFuel / fuelReadings.size();

        cout << "\n[Flight " << flightID << "] ---- Summary ----" << endl;
        cout << "[Flight " << flightID << "] Readings: " << fuelReadings.size() << endl;
        cout << "[Flight " << flightID << "] Avg fuel: " << averageFuel << " units\n";

        ofstream outFile("flight_logs.txt", ios::app);
        if (outFile.is_open()) {
            outFile << "-----------------------------\n";
            outFile << "Flight ID: " << flightID << "\n";
            outFile << "Telemetry Points: " << fuelReadings.size() << "\n";
            outFile << "Average Fuel: " << averageFuel << "\n";
            outFile << "-----------------------------\n\n";
            outFile.close();

            cout << "[Flight " << flightID << "] Log appended to flight_logs.txt\n";
        }
        else {
            cerr << "[Flight " << flightID << "] Failed to write to flight_logs.txt\n";
        }

        cout << "[Flight " << flightID << "] -------------------\n";
    }
    else {
        cout << "[Flight " << flightID << "] No valid fuel data.\n";
    }

    closesocket(clientSocket);
}


int main() {
    // initialzing winsock
    WSADATA wsaData;

    //server socket is the listening socket used to wait for clients
    // clientsocket will hold the actual connected client
    SOCKET serverSocket, clientSocket;
    // struct to hold the IP and the Port info
    sockaddr_in serverAddr{}, clientAddr{};
    int clientSize = sizeof(clientAddr);

    // initialzing winsock.. throwing an error if that fails..
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed.\n";
        return 1;
    }

    //creating a tcp socket.. if this fails clean up and exit.
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed.\n";
        WSACleanup();
        return 1;
    }

    // binding the socket to IP and port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // listen from all the network interfaces...
    serverAddr.sin_port = htons(8080); // Server will listen on port 8080

    // binding error. cleanup and exit
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Bind failed.\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // server listen
    listen(serverSocket, SOMAXCONN); // SOMAXCONN is the maximum number of clients OS allows in a queue
    cout << "Server is listening on port 8080...\n";

    //// accepting one client now
    //clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);
    //if (clientSocket != INVALID_SOCKET) {
    //    cout << "Client connected!\n";
    //}

    while (true) {
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);
        if (clientSocket != INVALID_SOCKET) {
            cout << "Client connected!\n";

            // Handle this client in a new thread
            thread clientThread(handle_client, clientSocket);
            clientThread.detach(); // Let it run in the background
        }
    }

    // Cleanup
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
