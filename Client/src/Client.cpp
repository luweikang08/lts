#include "Client.h"

namespace lts
{

	Client::Client()
	{
#ifdef WIN32
		WSADATA WSAData;
		WSAStartup(MAKEWORD(2, 2), &WSAData);
#endif

		base = event_base_new();
	}

	Client::~Client()
	{

	}

	int Client::Connect(struct sockaddr_in sin)
	{
		m_socket = ::socket(AF_INET, SOCK_STREAM, 0);
		::connect(m_socket, (SOCKADDR*)&sin, sizeof(SOCKADDR));

		struct sock_ev* ev_read = (struct sock_ev*)malloc(sizeof(struct sock_ev));
		memset(ev_read, 0, sizeof(struct sock_ev));
		ev_read->read_ev = (struct event*)malloc(sizeof(struct event));
		ev_read->close_ev = (struct event*)malloc(sizeof(struct event));
		ev_read->eventBase = this;
		auto func_read = [](int sock, short eventer, void* arg){
			int size;
			struct sock_ev* ev_read_tmp = (struct sock_ev*)arg;
			ev_read_tmp->buffer = (char*)malloc(MEM_SIZE);
			memset(ev_read_tmp->buffer, 0, MEM_SIZE);
			size = recv(sock, ev_read_tmp->buffer, MEM_SIZE, 0);

			if (size <= 0) {
				release_sock_event(ev_read_tmp);
				CLOSE(sock);
				return;
			}
			ev_read_tmp->bufferSize = size;
			ev_read_tmp->eventBase->on_message(ev_read_tmp);

		};
		event_set(ev_read->read_ev, m_socket, EV_READ | EV_PERSIST, func_read, ev_read);
		event_base_set(base, ev_read->read_ev);
		event_add(ev_read->read_ev, NULL);

		//auto func_close = [](int sock, short eventer, void* arg){
		//	int size;
		//	struct sock_ev* ev_close_tmp = (struct sock_ev*)arg;
		//	ev_close_tmp->buffer = (char*)malloc(MEM_SIZE);
		//	memset(ev_close_tmp->buffer, 0, MEM_SIZE);
		//	size = recv(sock, ev_close_tmp->buffer, MEM_SIZE, 0);
		//	if (size <= 0) {
		//		release_sock_event(ev_close_tmp);
		//		CLOSE(sock);
		//	}
		//	ev_close_tmp->bufferSize = size;
		//	ev_close_tmp->eventBase->on_message(ev_close_tmp);
		//};
		//event_set(ev_read->read_ev, m_socket, EV_READ | EV_PERSIST, func_read, ev_read);
		//event_base_set(base, ev_read->read_ev);
		//event_add(ev_read->read_ev, NULL);
		return 0;
	}

	int Client::on_send(const char*msg, int length)
	{
		printf("send message:%s, size:%d\n", msg, length);
		return ::send(m_socket, msg, length, 0);
	}

	int Client::Start()
	{
		event_base_dispatch(base);
		return 0;
	}

	int Client::on_message(struct sock_ev* ev)
	{
		//TODO: my work
		printf("receive message:%s, size:%d\n", ev->buffer, ev->bufferSize);
		//Sleep(1000);
		//::send(m_socket, ev->buffer, ev->bufferSize, 0);
		return 0;
	}

	int Client::on_close()
	{
		return 0;
	}

	int Client::AddTimer(struct timeval time)
	{
		struct sock_ev* event_timer = (struct sock_ev*)malloc(sizeof(struct sock_ev));
		memset(event_timer, 0, sizeof(struct sock_ev));
		event_timer->time_ev = (struct event*)malloc(sizeof(struct event));
		event_timer->eventBase = this;

		auto func_time = [](int sock, short eventer, void* arg)
		{
			struct sock_ev* event_time_l = (struct sock_ev*)arg;
			event_time_l->eventBase->on_time();
		};
		event_set(event_timer->time_ev, 0, EV_TIMEOUT | EV_PERSIST, func_time, event_timer);
		event_base_set(base, event_timer->time_ev);
		event_add(event_timer->time_ev, &time);
		return 0;
	}

	int Client::on_time()
	{
		printf("Client::on_time\n");
		::send(m_socket, "Client::on_time", strlen("Client::on_time"), 0);
		return 0;
	}
}