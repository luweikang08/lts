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
//#include <memory>
//
//#define MEM_SIZE    1024
//
//#ifdef WIN32
//#define CLOSE closesocket
//#else
//#define CLOSE close
//#endif
//
//class Client;
//
//struct sock_ev {
//	struct event* read_ev;
//	struct event* write_ev;
//	char* buffer;
//	Client* client;
//	int bufferSize;
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
////template<typename T>
////class Singleton
////{
////protected:
////	Singleton()   //构造函数是私有的  
////	{
////		m_pInstance = new T();
////	}
////	static T* m_pInstance;
////public:
////	static T* GetInstance()
////	{
////		if (m_pInstance == NULL)
////			m_pInstance = new T();
////		return m_pInstance;
////	}
////};
////
////class EnevtBase :public Singleton<EnevtBase>
////{
////public:
////	EnevtBase();
////	~EnevtBase();
////
////private:
////
////};
////
////EnevtBase::EnevtBase()
////{
////}
////
////EnevtBase::~EnevtBase()
////{
////}
//
//class Client
//{
//public:
//	Client();
//	~Client();
//
//	int Connect(struct sockaddr_in sin);
//	void Start();
//	void AddTimer();
//	//static void on_read(int sock, short eventer, void* arg);
//	int on_send(const char*msg, int length);
//	int on_close();
//	virtual int on_message(struct sock_ev* ev);
//private:
//	int m_socket;
//	struct event_base* base;
//};
//
//Client::Client()
//{
//#ifdef WIN32
//	WSADATA WSAData;
//	WSAStartup(MAKEWORD(2, 2), &WSAData);
//#endif
//
//	base = event_base_new();
//}
//
//Client::~Client()
//{
//}
//
//int Client::Connect(struct sockaddr_in sin)
//{
//	m_socket = ::socket(AF_INET, SOCK_STREAM, 0);
//	::connect(m_socket, (SOCKADDR*)&sin, sizeof(SOCKADDR));
//
//	struct sock_ev* ev = (struct sock_ev*)malloc(sizeof(struct sock_ev));
//	memset(ev, 0, sizeof(struct sock_ev));
//	ev->read_ev = (struct event*)malloc(sizeof(struct event));
//	ev->client = this;
//	//event_set(ev->read_ev, m_socket, EV_READ | EV_PERSIST, on_read, ev);
//	auto func = [](int sock, short eventer, void* arg){
//		int size;
//		struct sock_ev* ev = (struct sock_ev*)arg;
//		ev->buffer = (char*)malloc(MEM_SIZE);
//		//bzero(ev->buffer, MEM_SIZE);
//		memset(ev->buffer, 0, MEM_SIZE);
//		size = recv(sock, ev->buffer, MEM_SIZE, 0);
//		
//		if (size <= 0) {
//			release_sock_event(ev);
//			CLOSE(sock);
//			return;
//		}
//		ev->bufferSize = size;
//		ev->client->on_message(ev);
//
//	};
//	event_set(ev->read_ev, m_socket, EV_READ | EV_PERSIST, func, ev);
//	event_base_set(base, ev->read_ev);
//	event_add(ev->read_ev, NULL);
//
//	on_send("hello world!", (int)strlen("hello world!"));
//
//	return 0;
//}
//
////void Client::on_read(int sock, short eventer, void* arg)
////{
////	//struct event* write_ev;
////	int size;
////	struct sock_ev* ev = (struct sock_ev*)arg;
////	ev->buffer = (char*)malloc(MEM_SIZE);
////	//bzero(ev->buffer, MEM_SIZE);
////	memset(ev->buffer, 0, MEM_SIZE);
////	size = recv(sock, ev->buffer, MEM_SIZE, 0);
////	printf("receive data:%s, size:%d\n", ev->buffer, size);
////	if (size <= 0) {
////		release_sock_event(ev);
////		CLOSE(sock);
////		return;
////	}
////	ev->bufferSize = size;
////	ev->client->on_message(ev);
////}
//
//int Client::on_send(const char*msg, int length)
//{
//	printf("send message:%s, size:%d\n", msg, length);
//	return ::send(m_socket, msg, length, 0);
//}
//
//void Client::Start()
//{
//	event_base_dispatch(base);
//}
//
//int Client::on_message(struct sock_ev* ev)
//{
//	printf("receive message:%s, size:%d\n", ev->buffer, ev->bufferSize);
//	Sleep(1000);
//	on_send("focus on this!", (int)strlen("focus on this!"));
//	return 0;
//}
//
//
//int main(int argc, char** argv)
//{
//	Client client;
//
//	struct sockaddr_in sin;
//	memset(&sin, 0, sizeof(sin));
//	sin.sin_family = AF_INET;
//	sin.sin_addr.s_addr = htonl(0x7f000001); /* 127.0.0.1 */
//	sin.sin_port = htons(5000);
//
//	client.Connect(sin);
//
//	Sleep(100);
//	client.on_send("focus on this!", (int)strlen("focus on this!"));
//	client.Start();
//	printf("program over.\n");
//	return 0;
//}


#include "Client.h"

int main(int argc, char** argv)
{
	lts::Client client;

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(0x7f000001); /* 127.0.0.1 */
	sin.sin_port = htons(5000);

	client.Connect(sin);

	Sleep(100);
	client.on_send("focus on this!", (int)strlen("focus on this!"));
	client.Start();
	printf("program over.\n");
	return 0;
}