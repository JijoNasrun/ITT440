require 'socket'

server = TCPServer.open(1027)
loop {
	client = server.accept
	client.puts("Hello World!")
	client.close
}

