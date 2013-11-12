gcc -o server server.cpp -I /usr/include/udt/ -L /usr/lib/ -ludt -lstdc++ -lpthread
gcc -o client client.cpp -I /usr/include/udt/ -L /usr/lib/ -ludt -lstdc++ -lpthread