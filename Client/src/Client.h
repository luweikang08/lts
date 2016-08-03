//
// Created by lwk on 16-07-21.
//
// project@light trans service
//
#ifndef LTS_CLIENT_H
#define LTS_CLIENT_H

#ifdef WIN32
#include <WinSock2.h>
#include <sys\types.h>
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#endif

#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/event_struct.h>
#include <event2/event_compat.h>

#include "EventBase.h"

#define MEM_SIZE    1024

#ifdef WIN32
#define CLOSE closesocket
#else
#define CLOSE close
#endif

namespace lts
{

	class Client :public EventBase
	{
	public:
		Client();
		~Client();

		int Connect(struct sockaddr_in sin);
		int Start();
		void AddTimer();
		int on_send(const char*msg, int length);
		int on_close();
		virtual int on_message(struct sock_ev* ev);
	protected:
		int m_socket;
	};
}

#endif