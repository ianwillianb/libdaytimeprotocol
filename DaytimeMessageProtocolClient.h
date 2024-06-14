/*
 * DaytimeMessageProtocol.h
 *
 *  Created on: 13 de jun. de 2024
 *      Author: ianwillianb
 */

#ifndef DAYTIME_MESSAGE_PARSER_H
#define DAYTIME_MESSAGE_PARSER_H

#include <string>

/**
 * @brief A class to fetch and parse Daytime Protocol messages.
 */
class DaytimeMessageProtocolClient
{
	public:
		/**
		 * @brief Enum representing the format of the Daytime Protocol message.
		 */
		enum class DaytimeFormat : uint8_t
		{
			UNKNOWN,  ///< Unknown format
			RFC,  ///< Generic format
			NIST      ///< NIST format
		};

		/**
		 * @brief Enum representing the get operation result
		 */
		enum class GetResult : uint8_t
		{
			OK,
			FETCH_ERROR,
			PARSE_ERROR
		};

		static constexpr const uint16_t DAYTIME_SERVICE_PORT{13};
		static constexpr const char * NIST_MESSAGE_ID{"NIST"};

		/**
		 * @brief Constructs a DaytimeMessageProtocolClient object.
		 * @param host The hostname or IP address of the Daytime Protocol server.
		 * @param port The port number of the Daytime Protocol server.
		 * @param format The format of the Daytime Protocol message (default is UNKNOWN).
		 */
		DaytimeMessageProtocolClient(const std::string& host, const uint16_t port = DAYTIME_SERVICE_PORT,
				DaytimeFormat format = DaytimeFormat::UNKNOWN);

		/**
		 * @brief Get the Daytime Protocol message from the server and parses it.
		 * @return OK if the message was successfully fetched and parsed, otherwise error code.
		 */
		GetResult Get();

		/**
		 * @brief Get the Daytime Protocol message.
		 * @return The last message fetched from server, empty if string
		 * no message has been fetched.
		 */
		std::string GetMessage() const;

		/**
		 * @brief Gets the modified Julian date.
		 * @return The modified Julian date.
		 */
		int GetModifiedJulianDate() const;

		/**
		 * @brief Gets the date part of the message.
		 * @return The date as a string.
		 */
		std::string GetDate() const;

		/**
		 * @brief Gets the time part of the message.
		 * @return The time as a string.
		 */
		std::string GetTime() const;

		/**
		 * @brief Gets the daylight savings time flag.
		 * @return The daylight savings time flag.
		 */
		int GetDaylightSavingsTime() const;

		/**
		 * @brief Gets the leap second indicator.
		 * @return The leap second indicator.
		 */
		int GetLeapSecondIndicator() const;

		/**
		 * @brief Gets the UTC offset.
		 * @return The UTC offset as a string.
		 */
		std::string GetUtcOffset() const;

		/**
		 * @brief Gets the fractional seconds.
		 * @return The fractional seconds.
		 */
		double GetFractionalSeconds() const;

		/**
		 * @brief Gets the reference ID.
		 * @return The reference ID as a string.
		 */
		std::string GetRefID() const;

		/**
		 * @brief Get last operation result.
		 * @return Operation result as defined in GetResult.
		 */
		GetResult GetLastOperationResult() const;

	private:
		std::string m_host{};   ///< The hostname or IP address of the Daytime Protocol server.
		uint16_t m_port{DAYTIME_SERVICE_PORT};   ///< The port number of the Daytime Protocol server.
		DaytimeFormat m_format{DaytimeFormat::UNKNOWN}; ///< The format of the Daytime Protocol message.
		GetResult m_last_get_result{GetResult::OK};

		std::string m_message{}; ///< The protocol message.
		std::string m_date{};         ///< The date part of the message.
		std::string m_time{};         ///< The time part of the message.
		std::string m_utc_offset{};    ///< The UTC offset.
		std::string m_ref_id{};        ///< The reference ID.
		int m_modified_julian_date{-1}; ///< The modified Julian date.
		int m_daylight_savings_time{-1}; ///< The daylight savings time flag.
		int m_leap_second_indicator{-1}; ///< The leap second indicator.
		double m_fractional_seconds{-1}; ///< The fractional seconds.

		/**
		 * @brief Fetches the Daytime Protocol message from the server.
		 * @return True if the message was successfully fetched, otherwise false.
		 */
		bool Fetch();

		/**
		 * @brief Parses the Daytime Protocol message.
		 * @return True if the message was successfully parsed, otherwise false.
		 */
		bool ParseMessage();

		/**
		 * @brief Parses a NIST format message.
		 * @return True if the message was successfully parsed, otherwise false.
		 */
		bool ParseNISTFormat();

		/**
		 * @brief Parses a generic RFC format message.
		 * @return True if the message was successfully parsed, otherwise false.
		 */
		bool ParseRFCFormat();
};

#endif // DAYTIME_MESSAGE_PARSER_H
