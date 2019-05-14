compile:
	g++ src/client.cpp -lpthread -o bin/client
	g++ src/server.cpp -lpthread -o bin/server

clean:
	rm -f bin/server bin/client
