LDFLAGS=-ludt

all: server client

server: server.o
client: client.o

client server:
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

.PHONY: clean
clean:
	rm -f server server.o client client.o
