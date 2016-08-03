//
// Created by lwk on 16-07-21.
//
// project@light trans service
//

#ifndef LTS_EVENTBASE_H
#define LTS_EVENTBASE_H

#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/event_struct.h>
#include <event2/event_compat.h>

#define EV_FUNCTIONSET {return 0;}   // = 0 , pure virtual function

#ifdef WIN32
#define CLOSE closesocket
#else
#define CLOSE close
#endif

namespace lts
{

	class EventBase
	{
	public:
		virtual int Start() EV_FUNCTIONSET;
		virtual int on_accept(int socket, struct sock_ev* ev) EV_FUNCTIONSET;
		virtual int on_send(int socket, const char*msg, int length) EV_FUNCTIONSET;
		virtual int on_close(int socket) EV_FUNCTIONSET;
		virtual int on_message(struct sock_ev* ev) EV_FUNCTIONSET;
	protected:
		struct event_base* base;
	};

	struct sock_ev {
		struct event* read_ev;
		struct event* write_ev;
		struct event* close_ev;
		char* buffer;
		EventBase* eventBase;
		int bufferSize;
		int socket;
	};

	inline static void release_sock_event(struct sock_ev* ev)
	{
		event_del(ev->read_ev);
		event_del(ev->write_ev);
		event_del(ev->close_ev);
		free(ev->read_ev);
		free(ev->write_ev);
		free(ev->close_ev);
		free(ev->buffer);
		free(ev);
	}

}

#endif