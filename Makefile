server: 
	g++ util.cpp client.cpp -o client && \
	g++ util.cpp server.cpp Epoll.cpp InetAddress.cpp Socket.cpp Channel.cpp -o server
test:
	g++ -o test test.cpp
clean:
	rm server && rm client
