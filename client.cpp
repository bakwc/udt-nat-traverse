#include <ctime>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <random>

#include <arpa/inet.h>
#include <udt/udt.h>
#include <unistd.h>

using namespace std;
using namespace UDT;

int main(int argc, char** argv) {
    const char *const server_ip = argc > 1? argv[1] : "127.0.0.1";

    UDTSOCKET client = UDT::socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(6890);
    inet_pton(AF_INET, server_ip, &serv_addr.sin_addr);   // server address here
    memset(&(serv_addr.sin_zero), '\0', 8);

    // selecting random local port
    std::random_device rd;
    std::default_random_engine re(rd());
    std::uniform_int_distribution<int> uniform_dist(9001, 9200);
    int myPort = uniform_dist(re);
    printf("my port: %d\n", myPort);

    sockaddr_in my_addr;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(myPort);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(my_addr.sin_zero), '\0', 8);


    // Connecting to server

    // binding my port
    if (UDT::ERROR == UDT::bind(client, (sockaddr*)&my_addr, sizeof(my_addr))) {
        cout << "bind error: " << UDT::getlasterror().getErrorMessage();
        return 1;
    }

    // connect to the server
    if (UDT::ERROR == UDT::connect(client, (sockaddr*)&serv_addr, sizeof(serv_addr))) {
        cout << "connect error: " << UDT::getlasterror().getErrorMessage();
        return 2;
    }

    char data[6];
    if (UDT::ERROR == UDT::recv(client, data, 6, 0)) {
        cout << "recv error:" << UDT::getlasterror().getErrorMessage() << endl;
        return 3;
    }

    sockaddr_in peer_addr;
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_addr.s_addr = *(uint32_t*)data;
    peer_addr.sin_port = *(uint16_t*)(data + 4);

    cout << "addr received: " << inet_ntoa(peer_addr.sin_addr) << ":" << ntohs(peer_addr.sin_port) << endl;
    UDT::close(client);

    client = UDT::socket(AF_INET, SOCK_STREAM, 0);
    bool rendezvous = true;
    UDT::setsockopt(client, 0, UDT_RENDEZVOUS, &rendezvous, sizeof(bool));

    if (UDT::ERROR == UDT::bind(client, (sockaddr*)&my_addr, sizeof(my_addr))) {
        cout << "bind error: " << UDT::getlasterror().getErrorMessage();
        return 4;
    }

    if (UDT::ERROR == UDT::connect(client, (sockaddr*)&peer_addr, sizeof(peer_addr))) {
        cout << "connect error: " << UDT::getlasterror().getErrorMessage();
        return 5;
    }
    cout << "SUCCESS!\n";
    UDT::close(client);

    return 0;
}
