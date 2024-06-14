/*
 * main.cpp
 *
 *  Created on: 13 de jun. de 2024
 *  Author: ianwillianb
 */
#include <iostream>

#include "DaytimeMessageProtocolClient.h"

void fetch_daytime_message(const char * ip_or_host)
{
	DaytimeMessageProtocolClient protocol(ip_or_host);
	protocol.Get();

	std::cout << "Raw Message: " << protocol.GetMessage() << std::endl;
	std::cout << "Modified Julian Date: " << protocol.GetModifiedJulianDate() << std::endl;
	std::cout << "Date: " << protocol.GetDate() << std::endl;
	std::cout << "Time: " << protocol.GetTime() << std::endl;
	std::cout << "Daylight Savings Time: " << protocol.GetDaylightSavingsTime() << std::endl;
	std::cout << "Leap Second Indicator: " << protocol.GetLeapSecondIndicator() << std::endl;
	std::cout << "UTC Offset: " << protocol.GetUtcOffset() << std::endl;
	std::cout << "Fractional Seconds: " << protocol.GetFractionalSeconds() << std::endl;
	std::cout << "Reference ID: " << protocol.GetRefID() << std::endl;
}

int main(int argc, char **argv)
{
	fetch_daytime_message("time.nist.gov");
	puts("");
	fetch_daytime_message("localhost");
}

