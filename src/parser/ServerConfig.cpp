/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 20:24:55 by jyap              #+#    #+#             */
/*   Updated: 2025/01/13 19:54:21 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ServerConfig.hpp"

ServerConfig::ServerConfig()
{
	this->_port = 0;
	this->_host = 0;
	this->_server_name = "";
	this->_root = "";
	this->_client_max_body_size = MAX_CONTENT_LENGTH;
	this->_index = "";
	this->_listen_fd = 0;
	this->_autoindex = false;
	this->initErrorPages();
	this->location_flag = false;
	this->autoindex_flag = false;
	this->maxsize_flag = false;
}

ServerConfig::~ServerConfig() {}

/* copy constructor */
ServerConfig::ServerConfig(const ServerConfig &other)
{
	*this = other;
}

/* assinment operator */
ServerConfig &ServerConfig::operator=(const ServerConfig & rhs)
{
	if (this != &rhs)
	{
		this->_server_name = rhs._server_name;
		this->_root = rhs._root;
		this->_port = rhs._port;
		this->_host = rhs._host;
		this->_client_max_body_size = rhs._client_max_body_size;
		this->_index = rhs._index;
		this->_error_pages = rhs._error_pages;
		this->_locations = rhs._locations;
		this->_listen_fd = rhs._listen_fd;
		this->_autoindex = rhs._autoindex;
		this->_server_address = rhs._server_address;
		this->location_flag = rhs.location_flag;
		this->autoindex_flag = rhs.autoindex_flag;
		this->maxsize_flag = rhs.maxsize_flag;
	}
	return (*this);
}

/* init error page by default */
void ServerConfig::initErrorPages(void)
{
	_error_pages[301] = "";
	_error_pages[302] = "";
	_error_pages[400] = "";
	_error_pages[401] = "";
	_error_pages[402] = "";
	_error_pages[403] = "";
	_error_pages[404] = "";
	_error_pages[405] = "";
	_error_pages[406] = "";
	_error_pages[500] = "";
	_error_pages[501] = "";
	_error_pages[502] = "";
	_error_pages[503] = "";
	_error_pages[505] = "";
	_error_pages[505] = "";
}

const bool &ServerConfig::getLocationSetFlag() const
{
	return (this->location_flag);
}

const bool &ServerConfig::getAutoIndexFlag() const
{
	return (this->autoindex_flag);
}

const bool &ServerConfig::getMaxSizeFlag() const
{
	return (this->maxsize_flag);
}

void ServerConfig::setLocationSetFlag(bool parameter)
{
	this->location_flag = parameter;
}

void ServerConfig::setAutoindexFlag(bool parameter)
{
	this->autoindex_flag = parameter;
}

void ServerConfig::setMaxSizeFlag(bool parameter)
{
	this->maxsize_flag = parameter;
}

/* Set functions */
void ServerConfig::setServerName(std::string server_name)
{
	checkToken(server_name);
	this->_server_name = server_name;
}

void ServerConfig::setHost(std::string parameter)
{
	checkToken(parameter);
	if (parameter == "localhost")
		parameter = "127.0.0.1";
	if (!isValidHost(parameter))
		throw ErrorException("Wrong syntax for host: " + parameter);
	//inet_addr converts IPv4 address in string format into
	//a 32-bit binary form(network byte order)
	//127.0.0.1 converts into 0x7f000001
	// returns INADDR_NONE (-1) if the string is invalid
	this->_host = inet_addr(parameter.c_str());
}

//set root for absolute and relative path
void ServerConfig::setRoot(std::string root)
{
	checkToken(root);
	if (getPathType(root) == IS_DIRECTORY)
	{
		this->_root = root;
		return ;
	}
	char dir[PATH_MAX];
	getcwd(dir, PATH_MAX);
	std::string full_root = dir + root;
	if (getPathType(full_root) != IS_DIRECTORY)
		throw ErrorException("Wrong syntax for root: " + root);
	this->_root = full_root;
}

//set port
void ServerConfig::setPort(std::string parameter)
{
	unsigned int port = 0;
	checkToken(parameter);
	for (size_t i = 0; i < parameter.length(); i++)
	{
		if (!std::isdigit(parameter[i]))
			throw ErrorException("Wrong syntax for port: " + parameter);
	}
	port = ft_stoi(parameter);
	if (port < 1 || port > 65636)
		throw ErrorException("Wrong syntax for port: " + parameter);
	this->_port = (uint16_t)port;
}

void ServerConfig::setClientMaxBodySize(std::string parameter)
{
	unsigned long body_size = 0;
	checkToken(parameter);
	for (size_t i = 0; i < parameter.length(); i++)
	{
		if (!std::isdigit(parameter[i]))
			throw ErrorException("Wrong syntax for client_max_body_size: " + parameter);
	}
	body_size = ft_stoi(parameter);
	if (!body_size)
		throw ErrorException("Wrong syntax for client_max_body_size: " + parameter);
	this->_client_max_body_size = body_size;
}

void ServerConfig::setIndex(std::string index)
{
	checkToken(index);
	this->_index = index;
}

void ServerConfig::setAutoindex(std::string autoindex)
{
	checkToken(autoindex);
	if (autoindex != "on" && autoindex != "off")
		throw ErrorException("Wrong syntax for autoindex: " + autoindex);
	if (autoindex == "on")
		this->_autoindex = true;
}

void ServerConfig::setErrorPages(const std::vector<std::string> &parameter)
{
	if (parameter.empty())
		return;
	if (parameter.size() % 2 != 0)
		throw ErrorException ("Error page initialization faled");
	for (size_t i = 0; i < parameter.size() - 1; i++)
	{
		//check error code is all digits
		for (size_t j = 0; j < parameter[i].size(); j++) {
			if (!std::isdigit(parameter[i][j]))
				throw ErrorException("Error code is invalid");
		}
		//check error code is 3 digits
		if (parameter[i].size() != 3)
			throw ErrorException("Error code is invalid");
		short code_error = ft_stoi(parameter[i]);
		if (statusCodeString(code_error)  == "Undefined" || code_error < 400)
			throw ErrorException ("Incorrect error code: " + parameter[i]);
		i++; //Move to the next string which is the path to the error page
		std::string path = parameter[i];
		checkToken(path);
		if (getPathType(path) == IS_DIRECTORY) //If path is directory
			throw ErrorException ("Incorrect path for error page file: " + path);
		if (getPathType(this->_root + path) != IS_FILE) //If path is not file
			throw ErrorException ("Incorrect path for error page file: " + this->_root + path);
		// If path does not exist or is not accessible
		if (checkFile(this->_root + path, F_OK) == -1 || checkFile(this->_root + path, R_OK) == -1)
			throw ErrorException ("Error page file :" + this->_root + path + " is not accessible");
		std::map<short, std::string>::iterator it = this->_error_pages.find(code_error);
		//If error code mapping is found, overwrite, else add new map entry
		if (it != _error_pages.end())
			this->_error_pages[code_error] = path;
		else
			this->_error_pages.insert(std::make_pair(code_error, path));
	}
}

/* parsing and set locations */
void ServerConfig::setLocation(std::string path, std::vector<std::string> parameter)
{
	Location new_location;
	std::vector<std::string> methods;
	int valid;

	handlers["root"] = &ServerConfig::handleRoot;
	handlers["allow_methods"] = &ServerConfig::handleAllowMethods;
	handlers["methods"] = &ServerConfig::handleAllowMethods;
	handlers["autoindex"] = &ServerConfig::handleAutoIndex;
	handlers["index"] = &ServerConfig::handleIndex;
	handlers["return"] = &ServerConfig::handleReturn;
	handlers["alias"] = &ServerConfig::handleAlias;
	handlers["cgi_ext"] = &ServerConfig::handleCgiExt;
	handlers["cgi_path"] = &ServerConfig::handleCgiPath;
	handlers["client_max_body_size"] = &ServerConfig::handleClientMaxBodySize;
	
	new_location.setPath(path);
	for (size_t i = 0; i < parameter.size(); i++)
	{
		std::map<std::string, Handler>::iterator it = handlers.find(parameter[i]);
		if (it != handlers.end())
		{
			// Call the corresponding handler
			(this->*(it->second))(i, new_location, parameter);
		}
		else
			// Handle invalid parameters
			throw ErrorException("Parameter in a location is invalid");
	}
	if (new_location.getPath() != "/cgi-bin" && new_location.getIndexLocation().empty())
		new_location.setIndexLocation(this->_index); //set index
	if (new_location.getMaxSizeFlag() == false) //set default max body size
		new_location.setMaxBodySize(this->_client_max_body_size);
	valid = isValidLocation(new_location);
	if (valid == 1)
		throw ErrorException("Failed CGI validation");
	else if (valid == 2)
		throw ErrorException("Failed path in location validation");
	else if (valid == 3)
		throw ErrorException("Failed redirection file in location validation");
	else if (valid == 4)
		throw ErrorException("Failed alias file in location validation");
	this->_locations.push_back(new_location);
}

void	ServerConfig::setFd(int fd)
{
	this->_listen_fd = fd;
}

//inet_pton converts a string representing IPv4 address into binary representation
//AF_INET means IPv4
//Stores the binary representation into sockaddr.sin_addr
//Returns 1 if conversion is successful, 0 if host is not valid IPv4 string
//Returns -1 if not valid address family (AF_INET for IPv4, AF_INET6 for IPv6)
bool ServerConfig::isValidHost(std::string host) const
{
	struct sockaddr_in sockaddr;
  	return (inet_pton(AF_INET, host.c_str(), &(sockaddr.sin_addr)) ? true : false);
}

bool ServerConfig::isValidErrorPages()
{
	std::map<short, std::string>::const_iterator it;
	for (it = this->_error_pages.begin(); it != this->_error_pages.end(); it++)
	{
		if (it->first < 100 || it->first > 599)
			return (false);
		if (checkFile(getRoot() + it->second, F_OK) < 0 || checkFile(getRoot() + it->second, R_OK) < 0)
			return (false);
	}
	return (true);
}

/* check parameters of location */
int ServerConfig::isValidLocation(Location &location) const
{
	if (location.getPath() == "/cgi-bin")
	{
		if (location.getCgiPath().empty() || location.getCgiExtension().empty() || location.getIndexLocation().empty())
			return (1);

		if (checkFile(location.getIndexLocation(), R_OK) < 0)
		{
			//use location's root or cwd and combine with path to get Index location
			std::string path = location.getRootLocation() + location.getPath() + "/" + location.getIndexLocation();
			if (getPathType(path) != IS_FILE)
			{				
				std::string root = getcwd(NULL, 0);
				location.setRootLocation(root);
				path = root + location.getPath() + "/" + location.getIndexLocation();
			}
			if (path.empty() || getPathType(path) != IS_FILE || checkFile(path, R_OK) < 0)
				return (1);
		}
		// check if the number of Cgi paths matches with number of extensions
		if (location.getCgiPath().size() != location.getCgiExtension().size())
			return (1);
		std::vector<std::string>::const_iterator it;
		for (it = location.getCgiPath().begin(); it != location.getCgiPath().end(); ++it)
		{
			if (getPathType(*it) == STATFAIL)
				return (1);
		}
		std::vector<std::string>::const_iterator it_path;
		for (it = location.getCgiExtension().begin(); it != location.getCgiExtension().end(); ++it)
		{
			std::string tmp = *it;
			if (tmp != ".py" && tmp != ".sh" && tmp != "*.py" && tmp != "*.sh")
				return (1);
			//set _ext_path[".py"] == "/usr/bin/python"
			for (it_path = location.getCgiPath().begin(); it_path != location.getCgiPath().end(); ++it_path)
			{
				std::string tmp_path = *it_path;
				if (tmp == ".py" || tmp == "*.py")
				{
					if (tmp_path.find("python") != std::string::npos)
						location._ext_path[".py"] = tmp_path;
				}
				else if (tmp == ".sh" || tmp == "*.sh")
				{
					if (tmp_path.find("bash") != std::string::npos)
						location._ext_path[".sh"] = tmp_path;
				}
			}
		}
	}
	else
	{
		if (isValidLocationPath(location.getPath()) == false)
			return (2);
		if (location.getRootLocation().empty()) //set default root
		{
			location.setRootLocation(this->_root);
		}
		//Validate index, return and alias
		if (fileExistReadable(location.getRootLocation() + location.getPath() + "/", location.getIndexLocation()))
			return (5);
		if (!location.getReturn().empty())
		{
			if (fileExistReadable(location.getRootLocation(), location.getReturn()))
				return (3);
		}
		if (!location.getAlias().empty())
		{
			if (fileExistReadable(location.getRootLocation(), location.getAlias()))
			 	return (4);
		}
	}
	return (0);
}

/* Get functions */
const std::string &ServerConfig::getServerName()
{
	return (this->_server_name);
}

const std::string &ServerConfig::getRoot()
{
	return (this->_root);
}

const bool &ServerConfig::getAutoindex()
{
	return (this->_autoindex);
}

const in_addr_t &ServerConfig::getHost()
{
	return (this->_host);
}

const uint16_t &ServerConfig::getPort()
{
	return (this->_port);
}

const size_t &ServerConfig::getClientMaxBodySize()
{
	return (this->_client_max_body_size);
}

const std::vector<Location> &ServerConfig::getLocations()
{
	return (this->_locations);
}

const std::map<short, std::string> &ServerConfig::getErrorPages()
{
	return (this->_error_pages);
}

const std::string &ServerConfig::getIndex()
{
	return (this->_index);
}

int ServerConfig::getFd() 
{ 
	return (this->_listen_fd); 
}

/* the two functions below can be used later for response */
const std::string &ServerConfig::getPathErrorPage(short key)
{
	std::map<short, std::string>::iterator it = this->_error_pages.find(key);
	if (it == this->_error_pages.end())
		throw ErrorException("Error_page does not exist");
	return (it->second);
}

// find location by a name
const std::vector<Location>::iterator ServerConfig::getLocationKey(std::string key)
{
	std::vector<Location>::iterator it;
	for (it = this->_locations.begin(); it != this->_locations.end(); it++)
	{
		if (it->getPath() == key)
			return (it);
	}
	throw ErrorException("Error: path to location not found");
}

/* check is a properly end of parameter */
void ServerConfig::checkToken(std::string &parameter)
{
	size_t pos = parameter.rfind(';'); //; must be at the end of the parameter
	if (pos != parameter.size() - 1)
		throw ErrorException("Token is invalid");
	parameter.erase(pos);
}

/* check location for a duplicate*/
bool ServerConfig::checkLocationsDup() const
{
	if (this->_locations.size() < 2)
		return (false);
	for (size_t i = 0; i < this->_locations.size() - 1; ++i)
	{
		for (size_t j = i + 1; j < this->_locations.size(); ++j)
		{
			if (this->_locations[i].getPath() == this->_locations[j].getPath())
				return (true);
		}
	}
	return (false);
}

/* socket setup and binding */
void	ServerConfig::setupServerSocket(void)
{
	//AF_INET means for IPv4
	//SOCK_STREAM: Use TCP, 0 for Default protocol for TCP/IP
	// returns fd for the socket or -1 if fail
	if ((_listen_fd = socket(AF_INET, SOCK_STREAM, 0) )  == -1 )
	{
		Logger::logMsg(RED, "webserv: socket error %s   Closing ....", strerror(errno));
		exit(EXIT_FAILURE);
	}

	int option_value = 1;
	// set socket options
	// SOL_SOCKET applies the option at the socket level
	// SO_REUSEADDR allows reuse of local addresses
	// &option_value Enables the option
	// sizeof(int) size of the option value
	setsockopt(_listen_fd, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int));
	memset(&_server_address, 0, sizeof(_server_address));
	// sin_family sets the  address family
	// sin_addr.s_addr stores the server's host address in network byte order
	// sin_port sets server port(converted to network byte order using htons)
	_server_address.sin_family = AF_INET;
	_server_address.sin_addr.s_addr = _host;
	_server_address.sin_port = htons(_port);
	// bind the socket to specified IP address and port, returns -1 if fail
	if (bind(_listen_fd, (struct sockaddr *) &_server_address, sizeof(_server_address)) == -1)
	{
		Logger::logMsg(RED, "webserv: bind error %s   Closing ....", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void ServerConfig::handleRoot(size_t& i, Location& new_location, std::vector<std::string>& parameter)
{
	if ((i + 1) > parameter.size())
		throw ErrorException("Missing value for root");
	if (!new_location.getRootLocation().empty()) //check if root is already set
		throw ErrorException("Root of location is duplicated");
	checkToken(parameter[++i]);
	if (getPathType(parameter[i]) == IS_DIRECTORY) //if directory
		new_location.setRootLocation(parameter[i]);
	else
		new_location.setRootLocation(this->_root + parameter[i]); //if not directory
}
		
void ServerConfig::handleAllowMethods(size_t& i, Location& new_location, std::vector<std::string>& parameter)
{
	if ((i + 1) > parameter.size())
		throw ErrorException("Missing value for Allow Methods");
	if (new_location.getMethodsFlag()) //check if methods already set
			throw ErrorException("Allow_methods of location is duplicated");
		std::vector<std::string> methods;
		while (++i < parameter.size())
		{
			if (parameter[i].find(";") != std::string::npos)
			{
				checkToken(parameter[i]);
				methods.push_back(parameter[i]);
				break ;
			}
			else
			{
				methods.push_back(parameter[i]);
				if (i + 1 >= parameter.size())
					throw ErrorException("Token is invalid");
			}
		}
		new_location.setMethods(methods);
		new_location.setMethodsFlag(true);
}

void ServerConfig::handleAutoIndex(size_t& i, Location& new_location, std::vector<std::string>& parameter)
{
	if ((i + 1) > parameter.size())
		throw ErrorException("Missing value for autoindex");
	if (new_location.getPath() == "/cgi-bin")
		throw ErrorException("parameter autoindex not allow for CGI");
	if (new_location.getAutoIndexFlag()) //check if autoindex already set
		throw ErrorException("Autoindex of location is duplicated");
	checkToken(parameter[++i]);
	new_location.setAutoindex(parameter[i]);
	new_location.setAutoindexFlag(true);
}

void ServerConfig::handleIndex(size_t& i, Location& new_location, std::vector<std::string>& parameter)
{
	if ((i + 1) > parameter.size())
		throw ErrorException("Missing value for index");
	//check if index already set
	if (!new_location.getIndexLocation().empty()) 
		throw ErrorException("Index of location is duplicated");
	checkToken(parameter[++i]);
	new_location.setIndexLocation(parameter[i]);
}

void ServerConfig::handleAlias(size_t& i, Location& new_location, std::vector<std::string>& parameter)
{
	if ((i + 1) > parameter.size())
		throw ErrorException("Missing value for alias");
	if (new_location.getPath() == "/cgi-bin")
		throw ErrorException("parameter alias not allow for CGI");
	//check if alias already set
	if (!new_location.getAlias().empty())
		throw ErrorException("Alias of location is duplicated");
	checkToken(parameter[++i]);
	new_location.setAlias(parameter[i]);
}

void ServerConfig::handleReturn(size_t& i, Location& new_location, std::vector<std::string>& parameter)
{
	if ((i + 1) > parameter.size())
		throw ErrorException("Missing value for return");
	if (new_location.getPath() == "/cgi-bin")
		throw ErrorException("parameter return not allow for CGI");
	//check if return already set
	if (!new_location.getReturn().empty())
		throw ErrorException("Return of location is duplicated");
	checkToken(parameter[++i]);
	new_location.setReturn(parameter[i]);
}

void ServerConfig::handleCgiExt(size_t& i, Location& new_location, std::vector<std::string>& parameter)
{
	if ((i + 1) > parameter.size())
		throw ErrorException("Missing value for cgi_ext");
	if (new_location.getPath() != "/cgi-bin")
		throw ErrorException("parameter cgi_ext only allowed for /cgi-bin");
	std::vector<std::string> extension;
	while (++i < parameter.size())
	{
		if (parameter[i].find(";") != std::string::npos)
		{
			checkToken(parameter[i]);
			extension.push_back(parameter[i]);
			break ;
		}
		else
		{
			extension.push_back(parameter[i]);
			if (i + 1 >= parameter.size())
				throw ErrorException("Token is invalid");
		}
	}
	new_location.setCgiExtension(extension);
}

void ServerConfig::handleCgiPath(size_t& i, Location& new_location, std::vector<std::string>& parameter)
{
	if ((i + 1) > parameter.size())
		throw ErrorException("Missing value for cgi_path");
	if (new_location.getPath() != "/cgi-bin")
		throw ErrorException("parameter cgi_path only allowed for /cgi-bin");
	std::vector<std::string> path;
	while (++i < parameter.size())
	{
		if (parameter[i].find(";") != std::string::npos)
		{
			checkToken(parameter[i]);
			path.push_back(parameter[i]);
			break ;
		}
		else
		{
			path.push_back(parameter[i]);
			if (i + 1 >= parameter.size())
				throw ErrorException("Token is invalid");
		}
		//check if python or bash is present
		if (parameter[i].find("/python") == std::string::npos && parameter[i].find("/bash") == std::string::npos)
			throw ErrorException("cgi_path is invalid");
	}
	new_location.setCgiPath(path);
}

void ServerConfig::handleClientMaxBodySize(size_t& i, Location& new_location, std::vector<std::string>& parameter)
{
	if ((i + 1) > parameter.size())
		throw ErrorException("Missing value for client max body size");
	if (new_location.getMaxSizeFlag()) //check if max body size already set
		throw ErrorException("Maxbody_size of location is duplicated");
	checkToken(parameter[++i]);
	new_location.setMaxBodySize(parameter[i]);
	new_location.setMaxSizeFlag(true);
}
