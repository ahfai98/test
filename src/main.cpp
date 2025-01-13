/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 21:15:59 by jyap              #+#    #+#             */
/*   Updated: 2024/12/25 19:30:49 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Webserv.hpp"
#include "../inc/ServerManager.hpp"

// Handles SIGPIPE signals to prevent program termination.
// If a client disconnects while the server is writing data, SIGPIPE would
// terminate the server unexpectedly, leading to downtime.
void handleSigpipe(int sig)
{ 
	if(sig) {}
}

std::string getConfigFilePath(int argc, char** argv)
{
	if (argc > 2)
	{
		Logger::logMsg(RED, "Error: Wrong number of arguments.");
		Logger::logMsg(RED, "Usage: ./webserv or ./webserv [config file path]");
		throw std::invalid_argument("Invalid arguments.");
	}
	return ((argc == 1) ? "configs/default.conf" : argv[1]);
}

int main(int argc, char **argv)
{
	try 
	{
		signal(SIGPIPE, handleSigpipe);
		//Get config file path
		std::string configFilePath = getConfigFilePath(argc, argv);
		//Parse config file for servers
		ConfigParser	configParser;
		configParser.createCluster(configFilePath);
		// Setup and run servers
		ServerManager 	serverManager;
		serverManager.setupServers(configParser.getServers());
		serverManager.runServers();
	}
	catch (std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return (1);
	}
	return (0);
}
