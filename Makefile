server:
	g++ -o server \
		server.cpp \
		src/EventLoop.cpp src/Server.cpp src/Connection.cpp src/Channel.cpp\
		src/Epoll.cpp src/Socket.cpp src/InetAddress.cpp src/Util.cpp src/Acceptor.cpp \
		src/Buffer.cpp
client:
	g++ -o client \
		client.cpp \
		src/Socket.cpp src/InetAddress.cpp src/Util.cpp src/Buffer.cpp
clean:
	rm server && rm client
