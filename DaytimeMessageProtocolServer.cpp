/*
 * DaytimeMessageProtocolServer.cpp
 *
 *  Created on: 15 de jun. de 2024
 *  Author: ianwillianb
 */

#include "DaytimeMessageProtocolServer.h"

#include <cstdio>
#include <cstring>
#include <ctime>
#include <sys/socket.h>
#include <unistd.h>

DaytimeMessageProtocolServer::DaytimeMessageProtocolServer(const uint16_t port) : m_server_port(port),	m_listen_socket(AF_INET, SOCK_STREAM, 0)
{
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(DAYTIME_SERVICE_PORT);
}

bool DaytimeMessageProtocolServer::Listen()
{
	if(m_listen_socket.IsValid() == false)
	{
		m_listen_socket = SocketWrapper(AF_INET, SOCK_STREAM, 0);
		if(m_listen_socket.IsValid() == false)
		{
			perror("Failed to create server socket");
			return false;
		}
	}

	if(bind(m_listen_socket.GetSocketFD(), (sockaddr *) &server_addr, sizeof(server_addr)) != 0)
	{
		char error_msg[256]{0};
		snprintf(error_msg, sizeof(error_msg), "Could not bind to%sport %d",
				(m_server_port == DAYTIME_SERVICE_PORT) ? " daytime service default " : " ",
						m_server_port);
		perror(error_msg);

		return false;
	}

	if(listen(m_listen_socket.GetSocketFD(), SOMAXCONN) != 0)
	{
		perror("Failed to listen to server socket");
		return false;
	}

	m_server_state = ServerState::LISTEN;

	bool no_error_state = true;

	while(m_server_state != ServerState::ABORTED)
	{
		const int client_fd = accept(m_listen_socket.GetSocketFD(), (sockaddr *) nullptr, 0);
		if(client_fd < 0)
		{
			perror("Aborting server, failed to accept connection");
			m_server_state = ServerState::ABORTED;
			no_error_state = false;
			break;
		}

		char message_buffer[1024]{0};
		const time_t time_now = time(nullptr);
		snprintf(message_buffer, sizeof(message_buffer), "%.24s\r\n", ctime(&time_now));

		if(write(client_fd, (void *) message_buffer, strlen(message_buffer)) == 0)
		{
			perror("Failed to write payload to client");
		}

		close(client_fd);
	}

	m_listen_socket.~SocketWrapper();

	return no_error_state;
}

void DaytimeMessageProtocolServer::Stop()
{
	m_server_state = ServerState::ABORTED;
}
