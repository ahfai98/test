/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 19:47:29 by jyap              #+#    #+#             */
/*   Updated: 2024/12/25 19:48:52 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Logger.hpp"

pthread_mutex_t Logger::logMutex = PTHREAD_MUTEX_INITIALIZER;

void	Logger::logMsg(const char *color, const char* msg, ...)
{
	char		output[8192];
	va_list		args;
	
	va_start(args, msg);
	// vsnprintf works like printf but more flexible
	// Can take va_list as parameter and
	// Get the output in a buffer instead of printing directly
	// Safer with buffer overflow prevention and more efficient use of memory
	int n = vsnprintf(output, sizeof(output), msg, args);
	va_end(args);
	// Check for buffer overflow
	if (n < 0)
	{
		pthread_mutex_lock(&logMutex);
		std::cerr << "Error: Failed to format log message." << std::endl;
		pthread_mutex_unlock(&logMutex);
		return;
	}
	std::string date = getCurrTime();
	if (static_cast<size_t>(n) >= sizeof(output))
	{
		pthread_mutex_lock(&logMutex);
		// Output was truncated
		std::cerr << "Warning: Log message truncated due to buffer size." << std::endl;
		pthread_mutex_unlock(&logMutex);
	}
	pthread_mutex_lock(&logMutex);
	std::cout << color << date << output << RESET << std::endl;
	pthread_mutex_unlock(&logMutex);
}

std::string Logger::getCurrTime()
{
	char date[50];
	// time(0) retrieves the current time as the number of
	// seconds since the Unix epoch (January 1, 1970)
	// time_t now represents the current time in seconds
	time_t now = time(0);
	// gmtime converts time_t value into a tm struct, and
	// returns a pointer to this tm struct
	// tm struct fields: tm_year, tm_month, tm_hour, etc...
	struct tm tm = *gmtime(&now);
	// Add global time shift(GST) and adjust date 
	tm.tm_hour += GST;
	if (tm.tm_hour >= 24)
	{
		tm.tm_hour -= 24;
		tm.tm_mday += 1;
	}
	// Formats the time stored in tm struct into string
	strftime(date, sizeof(date), "[%Y-%m-%d  %H:%M:%S]   ", &tm);
	return (std::string(date));
}
