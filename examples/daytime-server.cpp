#include <iostream>

#include "../DaytimeMessageProtocolServer.h"

int main(int argc, char **argv)
{
	DaytimeMessageProtocolServer daytime_server{};
	daytime_server.Listen();
}
