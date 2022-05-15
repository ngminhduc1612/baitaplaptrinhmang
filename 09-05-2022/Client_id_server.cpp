#include <winsock2.h>
#include <stdio.h>
#include <string>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

// Khai bao mang client
SOCKET clients[64];
int numClients = 0;

DWORD WINAPI ClientThread(LPVOID lpParam)
{
    SOCKET client = *(SOCKET*)lpParam;
    int ret;
    char id[16];
    char check[16];
    char buf[256];
    const char* notice = "Nhap sai cu phap! Vui long nhap lai dung cu phap id (client_id: idcuaban)\n";
    // Nhan du lieu tu client va in ra man hinh
    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;
        buf[ret] = 0;
        // tach id ra khoi chuoi
        sscanf(buf, "%s %s", check , id);

        //printf("Du lieu nhan duoc tu client %d: %s\n", client, buf);
        if (strncmp(check, "client_id:", 9) == 0) {
            break;
        }
        send(client, notice, strlen(notice), 0);
    }

    while (1)
    {
        char mess[256];
        ret = recv(client, mess, sizeof(mess), 0);

        // chuyen thanh chuoi 
        sprintf(buf, "%s: %s\n", id, mess);
        //int num = strlen(id) + strlen(mess) + 2;
        int lengthMess = strcspn(buf, "\n");

       
        for (int i = 0; i < numClients; i++) {
            if (client != clients[i])
                send(clients[i], buf, lengthMess + 1, 0);
        }
    }
    closesocket(client);
    numClients--;
}
int main()
{
    // Khoi tao thu vien
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    // Tao socket
    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Khai bao dia chi server
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8000);
    // Gan cau truc dia chi voi socket
    bind(listener, (SOCKADDR*)&addr, sizeof(addr));
    // Chuyen sang trang thai cho ket noi
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
