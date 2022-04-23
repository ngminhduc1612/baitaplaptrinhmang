#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)


int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8000);

    //Gan cau truc dia chi voi socket
    bind(listener, (SOCKADDR*)&addr, sizeof(addr));

    // Chuyen sang trang thai ket noi
    listen(listener, 5);

    //cho ket noi moi
    SOCKET client = accept(listener, NULL, NULL);
    int ret;
    char buf[256];
    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;
        buf[ret] = 0;
        printf("Du lieu tu client: %s\n", buf);
    }

    closesocket(client);
    closesocket(listener);
    WSACleanup();
}
