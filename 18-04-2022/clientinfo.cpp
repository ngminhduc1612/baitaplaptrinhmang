#include <stdio.h>
#include <WinSock2.h>

#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

int main()
{

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    addrinfo* info;
    int ret = getaddrinfo("localhost", "http", NULL, &info);

    SOCKADDR_IN addr;
    if (ret != 0) {
        printf("Khong the phan gia ten mien");
        return 1;
    }
    

    addr.sin_family = AF_INET;
    memcpy(&addr, info->ai_next->ai_addr, info->ai_next->ai_addrlen);
    addr.sin_port = htons(8000);

    system("pause");

    ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));
    if (ret == SOCKET_ERROR)
    {
        ret = WSAGetLastError();
        printf("Ket noi khong thanh cong - %d\n", ret);
        return 1;
    }

    //Truyen du lieu den server
    char buf[256];
    char name[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(name);

    GetComputerNameA(name, &size);

    char listDrive[40];
    GetLogicalDriveStringsA(sizeof(listDrive), listDrive);

    DWORD lpBytesPerSector = 0;
    DWORD lpSectorsPerCluster = 0;
    DWORD totalNumberOfClusters = 0;
    DWORD amountOfKBytesOfComputer = 0;
    GetDiskFreeSpaceA("C:", &lpSectorsPerCluster, &lpBytesPerSector, NULL, &totalNumberOfClusters);

    amountOfKBytesOfComputer = (totalNumberOfClusters/1024) * lpSectorsPerCluster * lpBytesPerSector;
    
    //doi DWORD sang chuoi char
    char procID[20];
    sprintf(procID, "%d", amountOfKBytesOfComputer);
    //so thu f
    float f;
    printf("Nhap so 1 de in ra ten may tinh, so 2 de inra danh sach o, so 3 de in ra kich thuoc cua o dia\n");
    while (1) 
    {
        printf("Nhap so thu: ");
        scanf("%f", &f);
        if (f == 1) {
            send(client, name, sizeof(name), 0);
        }
        if (f == 2) {
            send(client, listDrive, sizeof(listDrive), 0);
        }
        if (f == 3) {
            send(client, procID, sizeof(procID), 0);
        }
    }
    closesocket(client);
    WSACleanup();
}
