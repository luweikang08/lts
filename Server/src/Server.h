//
// Created by lwk on 16-07-21.
//
// project@light trans service
//
#ifndef LTS_SERVER_H
#define LTS_SERVER_H

#include "EventBase.h"

#define BACKLOG     5
#define MEM_SIZE    1024

namespace lts
{
	class Server :public EventBase
	{
	public:
		Server();
		~Server();

		int Bind(struct sockaddr_in sin);
		int Start();
		int AddTimer(struct timeval time);
		int on_accept(int socket, struct sock_ev* ev);
		int on_send(int socket, const char*msg, int length);
		int on_close(int socket);
		int on_time();
		virtual int on_message(struct sock_ev* ev);

	protected:
		int m_socket;

	private:

	};
		
}

#endif