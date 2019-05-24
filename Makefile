compile:
	g++ -std=c++11 src/client.cpp -lpthread -o bin/client
	g++ -std=c++11 src/server.cpp -lpthread -o bin/server

clean:
	rm -f bin/server bin/client
