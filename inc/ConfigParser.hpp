/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 22:47:57 by jyap              #+#    #+#             */
/*   Updated: 2024/12/27 14:31:02 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "Webserv.hpp"

class ServerConfig;

class ConfigParser
{
	private:
		std::vector<ServerConfig>	_servers;
		std::vector<std::string>	_server_config;
		size_t						_nb_server;
		typedef void (ConfigParser::*Handler)(size_t&, ServerConfig&, std::vector<std::string>&);
		std::map<std::string, Handler>	handlers;
		void handleRoot(size_t& i, ServerConfig& server, std::vector<std::string>& parameter);
		
		void handleListen(size_t& i, ServerConfig& server, std::vector<std::string>& parameter);

		void handleLocation(size_t& i, ServerConfig& server, std::vector<std::string>& parameter);

		void handleIndex(size_t& i, ServerConfig& server, std::vector<std::string>& parameter);

		void handleAutoIndex(size_t& i, ServerConfig& server, std::vector<std::string>& parameter);

		void handleErrorPage(size_t& i, ServerConfig& server, std::vector<std::string>& parameter);

		void handleServerName(size_t& i, ServerConfig& server, std::vector<std::string>& parameter);

		void handleClientMaxBodySize(size_t& i, ServerConfig& server, std::vector<std::string>& parameter);

		void handleHost(size_t& i, ServerConfig& server, std::vector<std::string>& parameter);

	public:
		ConfigParser();
		~ConfigParser();

		int createCluster(const std::string &config_file);
		void splitServers(std::string &content);
		void removeComments(std::string &content);
		void removeWhiteSpace(std::string &content);
		size_t findStartServer(size_t start, std::string &content);
		size_t findEndServer(size_t start, std::string &content);
		void createServer(std::string &config, ServerConfig &server);
		void checkServers();
		std::vector<ServerConfig>	getServers();
		int print();
		void finalizeServerConfig(ServerConfig &server);
		
		class ErrorException : public std::exception
		{
			private:
				std::string _message;
			public:
				ErrorException(std::string message) throw()
				{
					_message = "CONFIG PARSER ERROR: " + message;
				}
				virtual const char* what() const throw()
				{
					return (_message.c_str());
				}
				virtual ~ErrorException() throw() {}
		};
};

#endif
