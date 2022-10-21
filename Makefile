server: 
	g++ src/util.cpp client.cpp -o client && \
	g++ src/util.cpp src/Server.cpp src/Epoll.cpp src/InetAddress.cpp src/Socket.cpp src/Channel.cpp src/EventLoop.cpp src/Acceptor.cpp src/Connection.cpp server.cpp -o server
test:
	g++ -o test test.cpp
clean:
	rm server && rm client
