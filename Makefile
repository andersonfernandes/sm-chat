DEPS=src/shm_queue.cpp src/user.cpp

compile:
	g++ -std=c++11 $(DEPS) src/client.cpp -lpthread -o bin/client
	g++ -std=c++11 $(DEPS) src/server.cpp -o bin/server

clean:
	rm -f bin/server bin/client

test:
	g++ -std=c++11 tests/shm_queue_test.cpp src/shm_queue.cpp -o tests/bin/shm_queue_test
	./tests/bin/shm_queue_test --success
