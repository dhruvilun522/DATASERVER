# DATASERVER

This project is a client-server based application that allows a client to send data to a server, which then stores the data in a file on the server. and allows client to read or update data given to server. Additionally, the server can send data back to the client whenever requested.

## How to Use
To use this application, you will need to have both the client and server programs installed on your computer. The server program should be running on a computer that is accessible to the client.

## Server Installation and Setup
1. Download the server program file.
2. Open a terminal or command prompt and navigate to the directory where you extracted the server program file.
3. Run the command "g++ server.cpp -lws2_32" to build the server and run comand ".\a.exe" to run server.

## Client Installation and Setup
1. Download the client program file.
2. Edit the client.cpp file to include the IP address or hostname of the server and the port number that the server is running on.
3. Open a terminal or command prompt and navigate to the directory where you extracted the client program file.
4. Run the command "g++ client.cpp -lws2_32" to build the client and run comand ".\a.exe" to run client.







