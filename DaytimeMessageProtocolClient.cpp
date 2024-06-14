/*
 * DaytimeMessageProtocolClient.cpp
 *
 *  Created on: 13 de jun. de 2024
 *  Author: ianwillianb
 */

#include "DaytimeMessageProtocolClient.h"

#include <sstream>
#include <algorithm>
#include "sys/socket.h"
#include "unistd.h"
#include "TFTPUtils.h"
#include "SocketWrapper.h"

DaytimeMessageProtocolClient::DaytimeMessageProtocolClient(const std::string& host, const uint16_t port, DaytimeFormat format)
    : m_host(host), m_port(port), m_format(format) {}

DaytimeMessageProtocolClient::GetResult DaytimeMessageProtocolClient::Get()
{
	if(Fetch() == false)
	{
		m_last_get_result = GetResult::FETCH_ERROR;
		return m_last_get_result;
	}

	if(ParseMessage() == false)
	{
		m_last_get_result = GetResult::PARSE_ERROR;
		return m_last_get_result;
	}

	m_last_get_result = GetResult::OK;
    return m_last_get_result;
}

bool DaytimeMessageProtocolClient::Fetch()
{
	auto servaddr = TFTP::TFTPUtils::ResolveNetworkIPV4Address(m_host);
	if(servaddr.first == false)
	{
		return false;
	}

	SocketWrapper socketfd(AF_INET, SOCK_STREAM, 0);
	if(!socketfd)
	{
		return false;
	}

	constexpr uint16_t MAX_MESSAGE_LENGTH{1024};
	char recvline[MAX_MESSAGE_LENGTH + 1]{};
	int bytes_read{};

	servaddr.second.sin_port = htons(m_port); /* daytime server */

	if(connect(socketfd, (sockaddr *) &servaddr.second, sizeof(servaddr.second)) < 0)
	{
		return false;
	}

	while((bytes_read = read(socketfd, recvline, MAX_MESSAGE_LENGTH)) > 0)
	{
		recvline[bytes_read] = 0; /* null terminate */
	}

	m_message = recvline;

	return true;
}

bool DaytimeMessageProtocolClient::ParseMessage()
{
	std::string message{m_message};
	transform(message.begin(), message.end(), message.begin(), ::toupper);

	switch(m_format)
	{
		case DaytimeFormat::UNKNOWN:
		{
			if(message.find(NIST_MESSAGE_ID) != std::string::npos)
			{
				return ParseNISTFormat();
			}
			else
			{
				return ParseRFCFormat();
			}
		}
		case DaytimeFormat::RFC:
		{
			return ParseRFCFormat();
		}
		case DaytimeFormat::NIST:
		{
			return ParseNISTFormat();
		}
	}

	return false;
}

bool DaytimeMessageProtocolClient::ParseNISTFormat()
{
    std::istringstream ss(m_message);
    ss >> m_modified_julian_date >> m_date >> m_time >> m_daylight_savings_time
       >> m_leap_second_indicator >> m_utc_offset >> m_fractional_seconds >> m_ref_id;
    return true;
}

bool DaytimeMessageProtocolClient::ParseRFCFormat()
{
    std::istringstream ss(m_message);
    std::string weekday, month;
    int day, year;
    ss >> weekday >> month >> day >> m_time >> year;
    m_date = month + " " + std::to_string(day) + " " + std::to_string(year);
    return true;
}

std::string DaytimeMessageProtocolClient::GetMessage() const
{
	return m_message;
}

int DaytimeMessageProtocolClient::GetModifiedJulianDate() const
{
	return m_modified_julian_date;
}

std::string DaytimeMessageProtocolClient::GetDate() const
{
	return m_date;
}

std::string DaytimeMessageProtocolClient::GetTime() const
{
	return m_time;
}

int DaytimeMessageProtocolClient::GetDaylightSavingsTime() const
{
	return m_daylight_savings_time;
}

int DaytimeMessageProtocolClient::GetLeapSecondIndicator() const
{
	return m_leap_second_indicator;
}

std::string DaytimeMessageProtocolClient::GetUtcOffset() const
{
	return m_utc_offset;
}

double DaytimeMessageProtocolClient::GetFractionalSeconds() const
{
	return m_fractional_seconds;
}

std::string DaytimeMessageProtocolClient::GetRefID() const
{
	return m_ref_id;
}

/**
 * @brief Get last operation result.
 * @return Operation result as defined in GetResult.
 */
DaytimeMessageProtocolClient::GetResult DaytimeMessageProtocolClient::GetLastOperationResult() const
{
	return m_last_get_result;
}


