#include <arpa/inet.h>
#include <udt.h>
#include <iostream>
#include <cstring>

using namespace std;

int main() {
    UDTSOCKET serv = UDT::socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in my_addr;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(6890);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(my_addr.sin_zero), '\0', 8);

    if (UDT::ERROR == UDT::bind(serv, (sockaddr*)&my_addr, sizeof(my_addr))) {
        cout << "bind error: " << UDT::getlasterror().getErrorMessage();
        return 1;
    }

    UDT::listen(serv, 10);


    while(true) {
        int namelen;
        sockaddr_in recver1addr, recver2addr;
        char ip[16];
        char data[6];
        cout << "waiting for connections\n";

        UDTSOCKET recver1 = UDT::accept(serv, (sockaddr*)&recver1addr, &namelen);
        cout << "new connection: " << inet_ntoa(recver1addr.sin_addr) << ":" << ntohs(recver1addr.sin_port) << endl;

        UDTSOCKET recver2 = UDT::accept(serv, (sockaddr*)&recver2addr, &namelen);
        cout << "new connection: " << inet_ntoa(recver2addr.sin_addr) << ":" << ntohs(recver2addr.sin_port) << endl;

        cout << "sending addresses\n";
        *(uint32_t*)data = recver2addr.sin_addr.s_addr;
        *(uint16_t*)(data + 4) = recver2addr.sin_port;
        UDT::send(recver1, data, 6, 0);


        *(uint32_t*)data = recver1addr.sin_addr.s_addr;
        *(uint16_t*)(data + 4) = recver1addr.sin_port;
        UDT::send(recver2, data, 6, 0);

        UDT::close(recver1);
        UDT::close(recver2);
    }

    UDT::close(serv);

    return 0;
}
