/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 18:12:30 by jyap              #+#    #+#             */
/*   Updated: 2024/12/27 14:35:08 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ConfigParser.hpp"

ConfigParser::ConfigParser()
{
	this->_nb_server = 0;
}

ConfigParser::~ConfigParser() {}

/* printing parameters of servers from config file */
int ConfigParser::print()
{
	std::cout << "------------- Config -------------" << std::endl;
	for (size_t i = 0; i < _servers.size(); i++)
	{
		std::cout << "Server #" << i + 1 << std::endl;
		std::cout << "Server name: " << _servers[i].getServerName() << std::endl;
		std::cout << "Host: " << _servers[i].getHost() << std::endl;
		std::cout << "Root: " << _servers[i].getRoot() << std::endl;
		std::cout << "Index: " << _servers[i].getIndex() << std::endl;
		std::cout << "Port: " << _servers[i].getPort() << std::endl;
		std::cout << "Max BSize: " << _servers[i].getClientMaxBodySize() << std::endl;
		std::cout << "Error pages: " << _servers[i].getErrorPages().size() << std::endl;
		std::map<short, std::string>::const_iterator it = _servers[i].getErrorPages().begin();
		while (it != _servers[i].getErrorPages().end())
		{
			std::cout << (*it).first << " - " << it->second << std::endl;
			++it;
		}
		std::cout << "Locations: " << _servers[i].getLocations().size() << std::endl;
		std::vector<Location>::const_iterator itl = _servers[i].getLocations().begin();
		while (itl != _servers[i].getLocations().end())
		{
			std::cout << "name location: " << itl->getPath() << std::endl;
			std::cout << "methods: " << itl->getPrintMethods() << std::endl;
			std::cout << "index: " << itl->getIndexLocation() << std::endl;
			if (itl->getCgiPath().empty())
			{
				std::cout << "root: " << itl->getRootLocation() << std::endl;
				if (!itl->getReturn().empty())
					std::cout << "return: " << itl->getReturn() << std::endl;
				if (!itl->getAlias().empty())
					std::cout << "alias: " << itl->getAlias() << std::endl;
			}
			else
			{
				std::cout << "cgi root: " << itl->getRootLocation() << std::endl;
				std::cout << "sgi_path: " << itl->getCgiPath().size() << std::endl;
				std::cout << "sgi_ext: " << itl->getCgiExtension().size() << std::endl;
			}
			++itl;
		}
		itl = _servers[i].getLocations().begin();
		std::cout << "-----------------------------" << std::endl;
	}
	return (0);
}

/* checking and read config file, split servers to strings and creating vector of servers */
int ConfigParser::createCluster(const std::string &config_file)
{
	std::string		content;

	if (getPathType(config_file) != IS_FILE)
		throw ErrorException("File is invalid");
	if (checkFile(config_file, R_OK) == -1)
		throw ErrorException("File is not accessible");
	content = readFile(config_file);
	if (content.empty())
		throw ErrorException("File is empty");
	removeComments(content);
	removeWhiteSpace(content);
	splitServers(content);
	if (this->_server_config.size() != this->_nb_server)
		throw ErrorException("Something with size");
	for (size_t i = 0; i < this->_nb_server; i++)
	{
		ServerConfig server;
		createServer(this->_server_config[i], server);
		this->_servers.push_back(server);
	}
	if (this->_nb_server > 1)
		checkServers();
	return (0);
}

void ConfigParser::removeComments(std::string &content)
{
	size_t pos = content.find('#');
	while (pos != std::string::npos)
	{
		// Find the end of the line or the end of the string
		size_t pos_end = content.find('\n', pos);
		if (pos_end == std::string::npos) 
		{
			// If no newline is found, erase until the end of the string
			content.erase(pos);
			break;
		}
		else 
			// Erase from '#' to the newline character
			content.erase(pos, pos_end - pos);
		pos = content.find('#');
	}
}

void ConfigParser::removeWhiteSpace(std::string &content)
{
	// Trim leading whitespace
	size_t start = 0;
	while (start < content.length() && isspace(content[start]))
		start++;
	content = content.substr(start);

	// Trim trailing whitespace
	if (!content.empty()) 
	{
		size_t end = content.length() - 1;
		while (end > 0 && isspace(content[end]))
			end--;
		content = content.substr(0, end + 1);
	}

	// Reduce consecutive spaces in the content to a single space
	std::string result;
	bool in_space = false;
	for (size_t i = 0; i < content.length(); ++i) 
	{
		if (isspace(content[i])) 
		{
			if (!in_space) 
			{
				result += ' '; // Add a single space
				in_space = true;
			}
		} 
		else 
		{
			result += content[i];
			in_space = false;
		}
	}

	content = result;
}

//split config's server blocks and store in vector of ServerConfig
void ConfigParser::splitServers(std::string &content)
{
	size_t start = 0;
	size_t end = 1;

	if (content.find("server", 0) == std::string::npos)
		throw ErrorException("No server block found");

	while (start < content.length())
	{
		// Find the start of the server block
		start = findStartServer(start, content);
		// Find the end of the server block
		end = findEndServer(start, content);
		if (start >= end)
			throw ErrorException("Problem with scope");
		// Add the server block to the configuration
		this->_server_config.push_back(content.substr(start, end - start + 1));
		this->_nb_server++;
		start = end + 1; //Move to next server block
	}
}

// returns the index of the "{" at the start of a server block
size_t ConfigParser::findStartServer(size_t start, std::string &content)
{
	size_t i = start;
	while (i < content.length() && isspace(content[i]))
		i++; // Skip whitespace and find the start of "server"

	if (i >= content.length() || content.compare(i, 6, "server") != 0)
		throw ErrorException("Wrong character out of server scope{}");
	i += 6; // Move past server
	while (i < content.length() && isspace(content[i]))
		i++; // Skip whitespace after "server"
	// Ensure the block starts with '{'
	if (i >= content.length() || content[i] != '{')
		throw ErrorException("Missing '{' after server declaration");
	return (i); //returns index of "{"
}

// returns the index of } at the end of server block */
size_t ConfigParser::findEndServer(size_t start, std::string &content)
{
	size_t i = start + 1; // Start after the '{'
	int scope = 0;
	while (i < content.length())
	{
		if (content[i] == '{')
			scope++;
		else if (content[i] == '}')
		{
			if (scope == 0)
				return (i); // End of the server block
			scope--;
		}
		i++;
	}
	// If no closing '}' is found, throw an exception
	throw ErrorException("Unmatched '{' in server block");
}


std::vector<std::string> splitparameters(const std::string &line, const std::string &sep)
{
	std::vector<std::string> result;
	std::string::size_type start = 0;
	std::string::size_type end;

	while (start < line.length())
	{
		// Find the first occurrence of any separator
		end = line.find_first_of(sep, start);
		// Extract the substring between start and end
		if (end == std::string::npos)
		{
			result.push_back(line.substr(start));
			break;
		}
		if (end > start)// Avoid adding empty substrings
			result.push_back(line.substr(start, end - start));

		// Move start to the next non-separator character
		start = line.find_first_not_of(sep, end);
	}
	return (result);
}


/* creating Server from string and fill the value */
void ConfigParser::createServer(std::string &config, ServerConfig &server)
{
	std::vector<std::string>	parameters;
	std::vector<std::string>	error_codes;

	parameters = splitparameters(config += ' ', std::string(" \n\t"));
	if (parameters.size() < 3)
		throw  ErrorException("Failed server validation");
	handlers["root"] = &ConfigParser::handleRoot;
	handlers["listen"] = &ConfigParser::handleListen;
	handlers["location"] = &ConfigParser::handleLocation;
	handlers["autoindex"] = &ConfigParser::handleAutoIndex;
	handlers["index"] = &ConfigParser::handleIndex;
	handlers["host"] = &ConfigParser::handleHost;
	handlers["error_page"] = &ConfigParser::handleErrorPage;
	handlers["client_max_body_size"] = &ConfigParser::handleClientMaxBodySize;
	handlers["server_name"] = &ConfigParser::handleServerName;
	
	for (size_t i = 0; i < parameters.size(); i++)
	{
		std::map<std::string, Handler>::iterator it = handlers.find(parameters[i]);
		if (it != handlers.end())
		{
			// Call the corresponding handler
			(this->*(it->second))(i, server, parameters);
		}
		else if (parameters[i] != "}" && parameters[i] != "{")
		{
			if (server.getLocationSetFlag() == true)
				throw  ErrorException("parameters after location");
			else
				throw  ErrorException("Unsupported directive");
		}
	}
	finalizeServerConfig(server);
}

void ConfigParser::finalizeServerConfig(ServerConfig &server)
{
	if (server.getRoot().empty())
		server.setRoot("/;");
	if (server.getHost() == 0)
		server.setHost("localhost;");
	if (server.getIndex().empty())
		server.setIndex("index.html;");
	if (fileExistReadable(server.getRoot(), server.getIndex()))
		throw ErrorException("Index from config file not found or unreadable");
	if (server.checkLocationsDup())
		throw ErrorException("Duplicate locations in server configuration");
	if (!server.getPort())
		throw ErrorException("Port not found in server configuration");
	if (!server.isValidErrorPages())
		throw ErrorException("Invalid error pages in server configuration");
}

//Ensure server port, host and name is unique
void ConfigParser::checkServers()
{
	size_t i, j;
	for (i = 0; i < this->_servers.size() - 1; i++)
	{
		for (j = i + 1; j < this->_servers.size(); j++)
		{
			if (this->_servers[i].getPort() == this->_servers[j].getPort() &&
				this->_servers[i].getHost() == this->_servers[j].getHost() &&
				this->_servers[i].getServerName() == this->_servers[j].getServerName())
			{
				throw ErrorException("Failed server validation");
			}
		}
	}
}

std::vector<ServerConfig>	ConfigParser::getServers()
{
	return (this->_servers);
}

void ConfigParser::handleListen(size_t& i, ServerConfig& server, std::vector<std::string>& parameter)
{
	if ((i + 1) > parameter.size())
		throw ErrorException("Missing value for listen");
	if (server.getLocationSetFlag() == true)
		throw  ErrorException("parameters after location");
	if (server.getPort())
		throw  ErrorException("Port is duplicated");
	server.setPort(parameter[++i]);
}

void ConfigParser::handleLocation(size_t& i, ServerConfig& server, std::vector<std::string>& parameter)
{
	if ((i + 1) > parameter.size())
		throw ErrorException("Missing value for location");
	std::string	path;
	i++;
	if (parameter[i] == "{" || parameter[i] == "}")
		throw  ErrorException("Wrong character in server scope{}");
	path = parameter[i];
	std::vector<std::string> codes;
	if (parameter[++i] != "{")
		throw  ErrorException("Wrong character in server scope{}");
	i++;
	while (i < parameter.size() && parameter[i] != "}")
		codes.push_back(parameter[i++]);
	server.setLocation(path, codes);
	// Check last character is }
	if (i < parameter.size() && parameter[i] != "}")
		throw  ErrorException("Wrong character in server scope{}");
	server.setLocationSetFlag(true);
}

void ConfigParser::handleHost(size_t& i, ServerConfig& server, std::vector<std::string>& parameter)
{
	if ((i + 1) > parameter.size())
		throw ErrorException("Missing value for host");
	if (server.getLocationSetFlag() == true)
		throw  ErrorException("parameters after location");
	if (server.getHost())
		throw  ErrorException("Host is duplicated");
	server.setHost(parameter[++i]);
}

void ConfigParser::handleRoot(size_t& i, ServerConfig& server, std::vector<std::string>& parameter)
{
	if ((i + 1) > parameter.size())
		throw ErrorException("Missing value for root");
	if (server.getLocationSetFlag() == true)
		throw  ErrorException("parameters after location");
	if (!server.getRoot().empty())
		throw  ErrorException("Root is duplicated");
	server.setRoot(parameter[++i]);
}


void ConfigParser::handleIndex(size_t& i, ServerConfig& server, std::vector<std::string>& parameter)
{
	if ((i + 1) > parameter.size())
		throw ErrorException("Missing value for index");
	if (server.getLocationSetFlag() == true)
		throw  ErrorException("parameters after location");
	if (!server.getIndex().empty())
		throw  ErrorException("Index is duplicated");
	server.setIndex(parameter[++i]);
}

void ConfigParser::handleAutoIndex(size_t& i, ServerConfig& server, std::vector<std::string>& parameter)
{
	if ((i + 1) > parameter.size())
		throw ErrorException("Missing value for autoindex");
	if (server.getLocationSetFlag() == true)
		throw  ErrorException("parameters after location");
	if (server.getAutoIndexFlag())
		throw ErrorException("Autoindex of server is duplicated");
	server.setAutoindex(parameter[++i]);
	server.setAutoIndexFlag(true);
}

void ConfigParser::handleErrorPage(size_t& i, ServerConfig& server, std::vector<std::string>& parameter)
{
	if ((i + 1) > parameter.size())
		throw ErrorException("Missing value for error_page");
	if (server.getLocationSetFlag() == true)
		throw  ErrorException("parameters after location");
	std::vector<std::string>	error_codes;
	while (++i < parameter.size())
	{
		error_codes.push_back(parameter[i]);
		if (parameter[i].find(';') != std::string::npos)
			break ;
		if (i + 1 >= parameter.size())
			throw ErrorException("Wrong character out of server scope{}");
	}
	server.setErrorPages(error_codes);
}

void ConfigParser::handleServerName(size_t& i, ServerConfig& server, std::vector<std::string>& parameter)
{
	if ((i + 1) > parameter.size())
		throw ErrorException("Missing value for server name");
	if (server.getLocationSetFlag() == true)
		throw  ErrorException("parameters after location");
	if (!server.getServerName().empty())
		throw  ErrorException("Server_name is duplicated");
	server.setServerName(parameter[++i]);
}

void ConfigParser::handleClientMaxBodySize(size_t& i, ServerConfig& server, std::vector<std::string>& parameter)
{
	if ((i + 1) > parameter.size())
		throw ErrorException("Missing value for client max body size");
	if (server.getLocationSetFlag() == true)
		throw  ErrorException("parameters after location");
	if (server.getMaxSizeFlag())
		throw  ErrorException("Client_max_body_size is duplicated");
	server.setClientMaxBodySize(parameter[++i]);
	server.setMaxSizeFlag(true);
}

