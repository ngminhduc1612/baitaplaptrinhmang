#include <stdio.h>
#include <string>
#include <WinSock2.h>
#include <iostream>
#include <fstream>

using namespace std;

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

SOCKET clients[64];
int numClients = 0;

DWORD WINAPI ClientThread(LPVOID lpParam)
{
    SOCKET client = *(SOCKET*)lpParam;
    int ret;
    char user[256];
    char pwd[50];
    char buf[256];
    bool check = FALSE;
    const char* notice = "Ban hay nhap username va password:\n";
    const char* notice2 = "Sai username va password!\n";
    const char* notice3 = "Dang nhap thanh cong!";
    // Nhan du lieu tu client va in ra man hinh
    while (!check)
    {
        send(client, notice, strlen(notice), 0);
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;
        buf[ret] = 0;
        // Tach username va password 
        sscanf(buf, "%s %s", user, pwd);

        FILE* f = fopen("C:\\Users\\PC\\Desktop\\csdl_acc.txt", "r");

        
        while (fgets(buf, 256, f)) {
            char u[256];
            char p[50];
            sscanf(buf, "%s %s", u, p);
            if (strcmp(u, user) == 0 && strcmp(p, pwd) == 0) {
                check = TRUE;
                break;
            }
        }
        if (!check) {
            send(client, notice2, strlen(notice2), 0);
        }
        fclose(f);
    }
    send(client, notice3, strlen(notice3), 0);
    closesocket(client);
    numClients--;
}

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8000);

    bind(listener, (SOCKADDR*)&addr, sizeof(addr));
    listen(listener, 5);
    while (1)
    {
        SOCKET client = accept(listener, NULL, NULL);
        clients[numClients] = client;
        numClients++;
        printf("Client moi ket noi: %d\n", client);
        CreateThread(0, 0, ClientThread, &client, 0, 0);
    }
}
