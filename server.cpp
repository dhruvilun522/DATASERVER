#include <iostream>
#include <string.h>
#include <winsock2.h>
#include <winsock.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <fstream>
using namespace std;

string aqtype(string aqi){
    int n=aqi.length(),r=0;
    for(int i=0;i<n;i++){
        r=r*10 + int(aqi[i]-'0');
    }
    if(r>100){
        return string(" bad");
    }
    if(r>50){
        return string(" moderate");
    }
    return string(" good");
}

void fileupdate(char *city,char *temp,char *humidity,char *aqi){
    fstream fout;
    char c[50];
    strcpy(c,city);
    char n[]=".txt";
    strcat(c,n);
    fout.open(c,ios::out);
    fout<<temp<<endl;
    fout<<humidity<<endl;
    fout<<string(aqi)+aqtype(string(aqi))<<endl;
    fout.close();
    return;

}
string showtemp(char * city){
    fstream fin;
    char c[50],s1[50],s2[50],s3[50];

    strcpy(c,city);
    char n[]=".txt";
    strcat(c,n);
    fin.open(c,ios::in);
    fin>>s1>>s2>>s3;
    cout<<s1<<s2<<s3<<endl;
    fin.close();
    string t="temperature : "+string(s1);
    return t;

}

string showhumidity(char * city){
    fstream fin;
    char c[50],s1[50],s2[50],s3[50];

    strcpy(c,city);
    char n[]=".txt";
    strcat(c,n);
    fin.open(c,ios::in);
    fin>>s1>>s2>>s3;
    fin.close();
    string t="humidity : "+string(s2);
    return t;

}

string showaqi(char * city){
    fstream fin;
    char c[50],s1[50],s2[50],s3[50];

    strcpy(c,city);
    char n[]=".txt";
    strcat(c,n);
    fin.open(c,ios::in);
    fin>>s1>>s2>>s3;
    fin.close();

    string w=s3;


    string t="aqi : "+string(s3)+aqtype(w);
    return t;

}


DWORD WINAPI serverReceive(LPVOID lpParam)
{

    char buffer[1024] = { 0 };

    SOCKET client = *(SOCKET*)lpParam;

    while (true) {

        if (recv(client, buffer, sizeof(buffer), 0)== SOCKET_ERROR) {

            cout << "recv function failed with error "<< WSAGetLastError() << endl;

            return -1;

        }

        if(strcmp(buffer,"1")==0){
                char s1[]="city name : ";
                char s2[]="temperature : ";
                char s3[]="humidity : ";
                char s4[]="aqi : ";
                char city[50],temp[50],humidity[50],aqi[50];
                send(client,s1,sizeof(s1),0);
                memset(buffer, 0,sizeof(buffer));
                recv(client,buffer,sizeof(buffer),0);
                strcpy(city,buffer);

                send(client,s2,sizeof(s2),0);
                memset(buffer, 0,sizeof(buffer));
                recv(client,buffer,sizeof(buffer),0);
                strcpy(temp,buffer);

                send(client,s3,sizeof(s2),0);
                memset(buffer, 0,sizeof(buffer));
                recv(client,buffer,sizeof(buffer),0);
                strcpy(humidity,buffer);

                send(client,s4,sizeof(s2),0);
                memset(buffer, 0,sizeof(buffer));
                recv(client,buffer,sizeof(buffer),0);
                strcpy(aqi,buffer);

                fileupdate(city,temp,humidity,aqi);
            char ss[]="updated!!!!!!!";
            send(client,ss,sizeof(ss),0);
        }
        else if(strcmp(buffer,"2")==0){
                char s1[]="city name : ";
                char city[50];
                send(client,s1,sizeof(s1),0);
                memset(buffer, 0,sizeof(buffer));
                recv(client,buffer,sizeof(buffer),0);
                strcpy(city,buffer);

                string temp=showtemp(city);
                string humidity=showhumidity(city);
                string aqi=showaqi(city);

            char ss[50],ss1[50],ss2[50];
            strcpy(ss,temp.c_str());
            strcpy(ss1,humidity.c_str());
            strcpy(ss2,aqi.c_str());
            send(client,ss,sizeof(ss),0);
            cout<<ss<<endl;
            send(client,ss1,sizeof(ss),0);
            cout<<ss1<<endl;
            send(client,ss2,sizeof(ss),0);
            cout<<ss2<<endl;
            cout<<"data of city "<<city<<" sent."<<endl;
        }

        if (strcmp(buffer, "exit") == 0) {

            cout << "Client Disconnected."<< endl;

            break;

        }

    }

    return 1;
}

DWORD WINAPI serverSend(LPVOID lpParam)
{

    char buffer[1024] = { 0 };
    SOCKET client = *(SOCKET*)lpParam;

    while (true) {

        gets(buffer);

        if (send(client,buffer,sizeof(buffer), 0)== SOCKET_ERROR) {

            cout << "send failed with error "<< WSAGetLastError() << endl;

            return -1;

        }
        if (strcmp(buffer, "exit") == 0) {

            cout << "Thank you for using the application"<< endl;

            break;

        }

    }

    return 1;
}

int main()
{
    WSADATA WSAData;
    SOCKET server, client;

    SOCKADDR_IN serverAddr, clientAddr;



    WSAStartup(MAKEWORD(2, 0), &WSAData);

    server = socket(AF_INET,SOCK_STREAM, 0);

    if (server == INVALID_SOCKET) {

        cout << "Socket creation failed with error:"<< WSAGetLastError() << endl;

        return -1;

    }

    serverAddr.sin_addr.s_addr = INADDR_ANY;

    serverAddr.sin_family = AF_INET;

    serverAddr.sin_port = htons(5555);
    if (bind(server,(SOCKADDR*)&serverAddr,sizeof(serverAddr))== SOCKET_ERROR) {

        cout << "Bind function failed with error: "<< WSAGetLastError() << endl;

        return -1;

    }

    if (listen(server, 0)== SOCKET_ERROR) {

        cout << "Listen function failed with error:"<< WSAGetLastError() << endl;

        return -1;

    }



    cout << "Listening for incoming connections...." << endl;

    char buffer[1024];

    int clientAddrSize = sizeof(clientAddr);

    if ((client = accept(server,(SOCKADDR*)&clientAddr,&clientAddrSize))!= INVALID_SOCKET) {

        cout << "Client connected!" << endl;
        DWORD tid;
        HANDLE t1 = CreateThread(NULL,0,serverReceive,&client,0,&tid);
        if (t1 == NULL) {

            cout << "Thread Creation Error: "<< WSAGetLastError() << endl;

        }

        HANDLE t2 = CreateThread(NULL,0,serverSend,&client,0,&tid);
        if (t2 == NULL) {

            cout << "Thread Creation Error: "<< WSAGetLastError() << endl;

        }
        WaitForSingleObject(t1,INFINITE);

        WaitForSingleObject(t2,INFINITE);

        closesocket(client);

        if (closesocket(server)== SOCKET_ERROR) {

            cout << "Close socket failed with error: "<< WSAGetLastError() << endl;

            return -1;

        }

        WSACleanup();

    }
}
