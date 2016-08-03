//#ifdef WIN32
//#include <WinSock2.h>
//#include <sys\types.h>
//#else
//#include <sys/socket.h>
//#include <sys/types.h>
//#include <netinet/in.h>
//#endif
//
//#include <event2/event.h>
//#include <event2/bufferevent.h>
//#include <event2/buffer.h>
//#include <event2/event_struct.h>
//#include <event2/event_compat.h>
//
//#define BACKLOG     5
//#define MEM_SIZE    1024
//
//#ifdef WIN32
//#define CLOSE closesocket
//#else
//#define CLOSE close
//#endif
//
//class Server;
//
//struct sock_ev {
//	struct event* read_ev;
//	struct event* write_ev;
//	char* buffer;
//	Server* server;
//	int bufferSize;
//	int socket;
//};
//
//void release_sock_event(struct sock_ev* ev)
//{
//	event_del(ev->read_ev);
//	free(ev->read_ev);
//	free(ev->write_ev);
//	free(ev->buffer);
//	free(ev);
//}
//
//
//class Server
//{
//public:
//	Server();
//	~Server();
//	int Bind(struct sockaddr_in sin);
//	void Start();
//	//static void on_read(int sock, short eventer, void* arg);
//	int on_accept(int socket, struct sock_ev* ev);
//	int on_send(int socket, const char*msg, int length);
//	int on_close();
//	virtual int on_message(struct sock_ev* ev);
//
//protected:
//
//private:
//	int m_socket;
//	struct event_base* base;
//};
//
//Server::Server()
//{
//}
//
//Server::~Server()
//{
//}
//
//int Server::Bind(struct sockaddr_in sin)
//{
//#ifdef WIN32
//	WSADATA WSAData;
//	WSAStartup(MAKEWORD(2, 2), &WSAData);
//#endif
//
//	m_socket = ::socket(AF_INET, SOCK_STREAM, 0);
//	char yes = 1;
//	setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
//	::bind(m_socket, (struct sockaddr*)&sin, sizeof(struct sockaddr));
//	::listen(m_socket, BACKLOG);
//
//	base = event_base_new();
//
//	struct sock_ev* event_accept = (struct sock_ev*)malloc(sizeof(struct sock_ev));
//	event_accept->read_ev = (struct event*)malloc(sizeof(struct event));
//	event_accept->write_ev = (struct event*)malloc(sizeof(struct event));
//	event_accept->server = this;
//
//	auto func_accept = [](int sock, short eventer, void* arg){
//		struct sockaddr_in cli_addr;
//		struct sock_ev* event_accept_l = (struct sock_ev*)arg;
//		int newfd, sin_size;
//		sin_size = sizeof(struct sockaddr_in);
//		newfd = accept(sock, (struct sockaddr*)&cli_addr, &sin_size);
//		printf("accept a new socket:%d\n", newfd);
//		event_accept_l->server->on_accept(newfd, event_accept_l);
//	};
//	event_set(event_accept->read_ev, m_socket, EV_READ | EV_PERSIST, func_accept, event_accept);
//	event_base_set(base, event_accept->read_ev);
//	event_add(event_accept->read_ev, NULL);
//
//	return 0;
//}
//
//int Server::on_accept(int socket, struct sock_ev* ev)
//{
//	printf("on_accept.\n");
//	struct sock_ev* event_read = (struct sock_ev*)malloc(sizeof(struct sock_ev));
//	memset(event_read, 0, sizeof(struct sock_ev));
//	event_read->read_ev = (struct event*)malloc(sizeof(struct event));
//	event_read->write_ev = (struct event*)malloc(sizeof(struct event));
//	event_read->server = ev->server;
//	event_read->socket = socket;
//	//event_set(ev->read_ev, m_socket, EV_READ | EV_PERSIST, on_read, ev);
//	auto func_read = [](int sock, short eventer, void* arg){
//		int size;
//		struct sock_ev* event_read_l = (struct sock_ev*)arg;
//		event_read_l->buffer = (char*)malloc(MEM_SIZE);
//		//bzero(ev->buffer, MEM_SIZE);
//		memset(event_read_l->buffer, 0, MEM_SIZE);
//		size = recv(sock, event_read_l->buffer, MEM_SIZE, 0);
//		printf("1111receive data:%s, size:%d\n", event_read_l->buffer, size);
//		if (size <= 0) {
//			release_sock_event(event_read_l);
//			CLOSE(sock);
//			return;
//		}
//		event_read_l->bufferSize = size;
//		event_read_l->server->on_message(event_read_l);
//
//	};
//	event_set(event_read->read_ev, socket, EV_READ | EV_PERSIST, func_read, event_read);
//	event_base_set(base, event_read->read_ev);
//	event_add(event_read->read_ev, NULL);
//	return 0;
//}
//
//int Server::on_message(struct sock_ev* ev)
//{
//	printf("receive data:%s, size:%d\n", ev->buffer, ev->bufferSize);
//	on_send(ev->socket, ev->buffer, ev->bufferSize);
//	return 0;
//}
//
//int Server::on_send(int socket, const char*msg, int length)
//{
//	printf("send message:%s, size:%d\n", msg, length);
//	return ::send(socket, msg, length, 0);
//	return 0;
//}
//
//void Server::Start()
//{
//	event_base_dispatch(base);
//}

#include "Server.h"

class testServer :public lts::Server
{
public:
	int on_message(struct lts::sock_ev* ev)
	{
		printf("testServer recvive data:%s\n", ev->buffer);
		on_send(ev->socket, ev->buffer, ev->bufferSize);
		return 0;
	}
};

int main(int argc, char** argv)
{
	struct sockaddr_in my_addr;
	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(5000);
	my_addr.sin_addr.s_addr = INADDR_ANY;

	//lts::Server server;
	//server.Bind(my_addr);
	//server.Start();

	testServer tServer;
	tServer.Bind(my_addr);
	tServer.Start();

	return 0;
}