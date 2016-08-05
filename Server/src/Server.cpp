#include "Server.h"

namespace lts
{
	Server::Server()
	{
	}

	Server::~Server()
	{
	}

	int Server::Bind(struct sockaddr_in sin)
	{
#ifdef WIN32
		WSADATA WSAData;
		WSAStartup(MAKEWORD(2, 2), &WSAData);
#endif

		m_socket = ::socket(AF_INET, SOCK_STREAM, 0);
		char yes = 1;
		setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		::bind(m_socket, (struct sockaddr*)&sin, sizeof(struct sockaddr));
		::listen(m_socket, BACKLOG);

		base = event_base_new();

		struct sock_ev* event_accept = (struct sock_ev*)malloc(sizeof(struct sock_ev));
		event_accept->read_ev = (struct event*)malloc(sizeof(struct event));
		event_accept->write_ev = (struct event*)malloc(sizeof(struct event));
		event_accept->eventBase = this;

		auto func_accept = [](int sock, short eventer, void* arg){
			struct sockaddr_in cli_addr;
			struct sock_ev* event_accept_l = (struct sock_ev*)arg;
			int newfd, sin_size;
			sin_size = sizeof(struct sockaddr_in);
			newfd = accept(sock, (struct sockaddr*)&cli_addr, &sin_size);
			printf("accept a new socket:%d\n", newfd);
			event_accept_l->eventBase->on_accept(newfd, event_accept_l);
		};
		event_set(event_accept->read_ev, m_socket, EV_READ | EV_PERSIST, func_accept, event_accept);
		event_base_set(base, event_accept->read_ev);
		event_add(event_accept->read_ev, NULL);

		return 0;
	}

	int Server::Start()
	{
		event_base_dispatch(base);
		return 0;
	}

	int Server::on_accept(int socket, struct sock_ev* ev)
	{
		printf("on_accept.\n");
		{
			struct sock_ev* event_read = (struct sock_ev*)malloc(sizeof(struct sock_ev));
			memset(event_read, 0, sizeof(struct sock_ev));
			event_read->read_ev = (struct event*)malloc(sizeof(struct event));
			event_read->write_ev = (struct event*)malloc(sizeof(struct event));
			event_read->close_ev = (struct event*)malloc(sizeof(struct event));
			event_read->eventBase = ev->eventBase;
			event_read->socket = socket;
			//event_set(ev->read_ev, m_socket, EV_READ | EV_PERSIST, on_read, ev);
			auto func_read = [](int sock, short eventer, void* arg){
				int size;
				struct sock_ev* event_read_l = (struct sock_ev*)arg;
				event_read_l->buffer = (char*)malloc(MEM_SIZE);
				memset(event_read_l->buffer, 0, MEM_SIZE);
				size = recv(sock, event_read_l->buffer, MEM_SIZE, 0);
				//printf("1111receive data:%s, size:%d\n", event_read_l->buffer, size);
				if (size <= 0) {
					event_del(event_read_l->read_ev);
					//event_del(event_read_l->write_ev);
					//event_del(event_read_l->close_ev);
					free(event_read_l->read_ev);
					free(event_read_l->write_ev);
					free(event_read_l->close_ev);
					free(event_read_l->buffer);
					free(event_read_l);
					//release_sock_event(event_read_l);
					CLOSE(sock);
					event_read_l->eventBase->on_close(event_read_l->socket);
					return;
				}
				event_read_l->bufferSize = size;
				event_read_l->eventBase->on_message(event_read_l);

			};
			event_set(event_read->read_ev, socket, EV_READ | EV_PERSIST, func_read, event_read);
			event_base_set(base, event_read->read_ev);
			event_add(event_read->read_ev, NULL);
		}
		/**************************************************/
		{
			struct sock_ev* event_close = (struct sock_ev*)malloc(sizeof(struct sock_ev));
			memset(event_close, 0, sizeof(struct sock_ev));
			event_close->read_ev = (struct event*)malloc(sizeof(struct event));
			event_close->write_ev = (struct event*)malloc(sizeof(struct event));
			event_close->close_ev = (struct event*)malloc(sizeof(struct event));
			event_close->eventBase = ev->eventBase;
			event_close->socket = socket;
			//event_set(ev->read_ev, m_socket, EV_READ | EV_PERSIST, on_read, ev);
			auto func_close = [](int sock, short eventer, void* arg){
				struct sock_ev* event_close_l = (struct sock_ev*)arg;
				event_close_l->buffer = (char*)malloc(MEM_SIZE);
				memset(event_close_l->buffer, 0, MEM_SIZE);
				printf("close a socket, socket:\n", event_close_l->socket);
				//event_del(event_close_l->read_ev);
				//event_del(event_close_l->write_ev);
				event_del(event_close_l->close_ev);
				free(event_close_l->read_ev);
				free(event_close_l->write_ev);
				free(event_close_l->close_ev);
				free(event_close_l->buffer);
				free(event_close_l);
				//release_sock_event(event_close_l);
				CLOSE(sock);
				event_close_l->eventBase->on_close(event_close_l->socket);
			};
			event_set(event_close->close_ev, socket, EV_CLOSED, func_close, event_close);
			event_base_set(base, event_close->close_ev);
			event_add(event_close->close_ev, NULL);
		}
		return 0;
	}

	int Server::on_send(int socket, const char*msg, int length)
	{
		printf("send to %d message:%s, size:%d\n", socket, msg, length);
		return ::send(socket, msg, length, 0);
		return 0;
	}

	int Server::on_close(int socket)
	{
		printf("socket:%d has closed.\n", socket);
		return 0;
	}

	int Server::on_message(struct sock_ev* ev)
	{
		printf("receive data:%s, size:%d\n", ev->buffer, ev->bufferSize);
		on_send(ev->socket, ev->buffer, ev->bufferSize);
		return 0;
	}

	int Server::on_time()
	{
		printf("Server::on_time\n");
		return 0;
	}

	int Server::AddTimer(struct timeval time)
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

}
