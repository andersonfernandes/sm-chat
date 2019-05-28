DEPS=src/messages_queue.cpp src/user.cpp

compile:
	g++ -std=c++11 $(DEPS) src/client.cpp -lpthread -o bin/client
	g++ -std=c++11 $(DEPS) src/server.cpp -lpthread -o bin/server

clean:
	rm -f bin/server bin/client
