/*
 * DaytimeMessageProtocolServer.h
 *
 *  Created on: 15 de jun. de 2024
 *  Author: ianwillianb
 */

#ifndef DAYTIMEMESSAGEPROTOCOLSERVER_H_
#define DAYTIMEMESSAGEPROTOCOLSERVER_H_

#include "SocketWrapper.h"
#include <netinet/in.h>

class DaytimeMessageProtocolServer
{
	public:

		enum class ServerState : uint8_t
		{
			IDLE,
			LISTEN,
			ABORTED
		};

		DaytimeMessageProtocolServer(const uint16_t port = DAYTIME_SERVICE_PORT);

		bool Listen();

		void Stop();

		const ServerState GetServerState() const
		{
			return m_server_state;
		}

	private:
		static constexpr const uint16_t DAYTIME_SERVICE_PORT{13};
		const uint16_t m_server_port;

		ServerState m_server_state{ServerState::IDLE};
		SocketWrapper m_listen_socket;
		sockaddr_in server_addr{};
};

#endif /* DAYTIMEMESSAGEPROTOCOLSERVER_H_ */
