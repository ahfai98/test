/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 12:11:53 by jyap              #+#    #+#             */
/*   Updated: 2025/01/13 19:11:34 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Webserv.hpp"

int ft_stoi(std::string str)
{
	std::stringstream ss(str);
	if (str.length() > 10)
		throw std::exception();
	for (size_t i = 0; i < str.length(); ++i)
	{
		if(!isdigit(str[i]))
			throw std::exception();
	}
	int res;
	ss >> res;
	return (res);
}

//std::hex interprets the string as hexadecimal value
//std::hex handles upper and lowercase
unsigned int fromHexToDec(const std::string& hexnb)
{
	unsigned int decnb;
	std::stringstream ss;
	ss << hexnb;
	ss >> std::hex >> decnb;
	return (decnb);
}

std::string statusCodeString(short statusCode)
{
	switch (statusCode)
	{
		case 100:
			return "Continue";
		case 101:
			return "Switching Protocol";
		case 102:
			return "Processing";
		case 200:
			return "OK";
		case 201:
			return "Created";
		case 202:
			return "Accepted";
		case 203:
			return "Non-Authoritative Information";
		case 204:
			return "No Content";
		case 205:
			return "Reset Content";
		case 206:
			return "Partial Content";
		case 207:
			return "Multi_status";
		case 208:
			return "Already Reported";
		case 300:
			return "Multiple Choice";
		case 301:
			return "Moved Permanently";
		case 302:
			return "Moved Temporarily";
		case 303:
			return "See Other";
		case 304:
			return "Not Modified";
		case 305:
			return "Use Proxy";
		case 306:
			return "Switch Proxy";
		case 307:
			return "Temporary Redirect";
		case 308:
			return "Permanent Redirect";
		case 400:
			return "Bad Request";
		case 401:
			return "Unauthorized";
		case 402:
			return "Payment Required";
		case 403:
			return "Forbidden";
		case 404:
			return "Not Found";
		case 405:
			return "Method Not Allowed";
		case 406:
			return "Not Acceptable";
		case 407:
			return "Proxy Authentication Required";
		case 408:
			return "Request Timeout";
		case 409:
			return "Conflict";
		case 410:
			return "Gone";
		case 411:
			return "Length Required";
		case 412:
			return "Precondition Failed";
		case 413:
			return "Payload Too Large";
		case 414:
			return "URI Too Long";
		case 415:
			return "Unsupported Media Type";
		case 416:
			return "Requested Range Not Satisfiable";
		case 417:
			return "Expectation Failed";
		case 418:
			return "I'm a teapot";
		case 421:
			return "Misdirected Request";
		case 425:
			return "Too Early";
		case 426:
			return "Upgrade Required";
		case 428:
			return "Precondition Required";
		case 429:
			return "Too Many Requests";
		case 431:
			return "Request Header Fields Too Large";
		case 444:
			return "No Response";
		case 451:
			return "Unavailable for Legal Reasons";
		case 500:
			return "Internal Server Error";
		case 501:
			return "Not Implemented";
		case 502:
			return "Bad Gateway";
		case 503:
			return "Service Unavailable";
		case 504:
			return "Gateway Timeout";
		case 505:
			return "HTTP Version Not Supported";
		case 506:
			return "Variant Also Negotiates";
		case 507:
			return "Insufficient Storage";
		case 510:
			return "Not Extended";
		case 511:
			return "Network Authentication Required";
		case 520:
			return "Unknown Error";
		case 521:
			return "Web Server is Down";
		case 523:
			return "Origin is Unreachable";
		case 524:
			return "A Timeout Occured";
		case 598:
			return "Network Read Timeout Error";
		case 599:
			return "Network Connect Timeout Error";
		default:
			return "Undefined";
		}
}

//Return a string with HTML code with status code and message as Title and Body
std::string getErrorPage(short statusCode)
{
	std::string statusStr = toString(statusCode);
	std::string statusDesc = statusCodeString(statusCode);
	std::string errorPage = "<html>\r\n";
	errorPage += "<head><title>" + statusStr + " " + statusDesc + "</title></head>\r\n";
	errorPage += "<body>\r\n";
	errorPage += "<center><h1>" + statusStr + " " + statusDesc + "</h1></center>\r\n";
	errorPage += "</body>\r\n";
	errorPage += "</html>\r\n";
	return (errorPage);
}

int buildHtmlIndex(std::string &dir_name, std::vector<uint8_t> &body, size_t &body_len)
{
	struct dirent	*entityStruct;
	DIR				*directory;
	std::string		dirListPage;

	directory = opendir(dir_name.c_str());
	if (directory == NULL)
	{
		std::cerr << "opendir failed" << std::endl;
		return (1);
	}
	// Build the HTML header
	dirListPage = "<html>\n<head>\n<title> Index of " + dir_name + "</title>\n</head>\n<body>\n";
	dirListPage += "<h1> Index of " + dir_name + "</h1>\n";
	dirListPage += "<table style=\"width:80%; font-size: 15px\">\n<hr>\n";
	dirListPage += "<th style=\"text-align:left\"> File Name </th>\n";
	dirListPage += "<th style=\"text-align:left\"> Last Modification  </th>\n";
	dirListPage += "<th style=\"text-align:left\"> File Size </th>\n";

	struct stat file_stat;
	std::string file_path;

	// Iterate through directory entries
	while ((entityStruct = readdir(directory)) != NULL)
	{
		//Skips . as it refers to current directory
		if (strcmp(entityStruct->d_name, ".") == 0)
			continue;
		//create full path to file or directory
		file_path = dir_name + entityStruct->d_name;
		//stat retrieves info about file or directory
		//info like size, permissions, modification time etc
		//stores info in file_stat(st_mode, st_mtime, st_size)
		if (stat(file_path.c_str(), &file_stat) == -1)
			continue; // If stat fails, skip this entry
		//<tr> starts table row, <td> starts table data cell
		dirListPage += "<tr>\n";
		//First column for hyperlink and name of directory entry
		dirListPage += "<td>\n";
		dirListPage += "<a href=\"" + std::string(entityStruct->d_name) + "\"";
		// If directory, append '/' to the link
		if (S_ISDIR(file_stat.st_mode))
			dirListPage += "/";
		dirListPage += "\">" + std::string(entityStruct->d_name) + "</a>\n</td>\n";
		//Second column for modification time
		dirListPage += "<td>\n" + std::string(ctime(&file_stat.st_mtime)) + "</td>\n";
		//Third column for file size, NA if directory
		dirListPage += "<td>\n";
		if (!S_ISDIR(file_stat.st_mode))
		{
			dirListPage += toString(file_stat.st_size);
			dirListPage += " bytes";
		}
		dirListPage += "</td>\n</tr>\n";
	}
	// Close HTML tags
	dirListPage += "</table>\n<hr>\n</body>\n</html>\n";
	// Copy the HTML content into the body vector
	body.insert(body.begin(), dirListPage.begin(), dirListPage.end());
	body_len = body.size();
	// Close the directory
	closedir(directory);
	return (0);
}

// Check path is file, directory or other
PathType getPathType(const std::string &path)
{
	struct stat	file_stat;

	if (stat(path.c_str(), &file_stat) == 0)
	{
		if (file_stat.st_mode & S_IFREG) // File
			return (IS_FILE);
		else if (file_stat.st_mode & S_IFDIR) // Directory
			return (IS_DIRECTORY);
		else // Other types (symlink, socket, etc.)
			return (IS_OTHER);
	}
	return (STATFAIL); // stat failed
}

// Check file is accessible with the mode (F_OK, R_OK, W_OK, X_OK)
//Mode: File exist, read, write, execute permissions
//Returns 0 if accessible, else -1
int	checkFile(const std::string &path, int mode)
{
	return (access(path.c_str(), mode));
}

//Check file exists and is readable for two locations(index or path+index)
int fileExistReadable(const std::string &path, const std::string &index)
{
	std::string fullPath1 = index;
	std::string fullPath2 = path + index;
	if ((getPathType(fullPath1) == IS_FILE && checkFile(fullPath1, R_OK) == 0) ||
		(getPathType(fullPath2) == IS_FILE && checkFile(fullPath2, R_OK) == 0))
		return (0);
	return (-1);
}

//Read file contents into string
std::string readFile(const std::string &path)
{
	if (path.empty())
		return (NULL);
	// Open the file in read mode
	std::ifstream config_file(path.c_str());
	if (!config_file) 
		return (NULL);
	// Read the file into a string
	std::stringstream ss;
	ss << config_file.rdbuf();
	return (ss.str());
}

bool checkdigits(std::string &str)
{
	for (std::string::size_type i = 0; i < str.size(); ++i)
	{
		if (!isdigit(str[i]))  // Check each character if it's not a digit
			return (false);
	}
	return (true);
}

bool isValidIP(const std::string &ip_addr)
{
	size_t dots = 0;
	size_t last_pos = 0;
	size_t slash_pos = ip_addr.find('/');

	if (slash_pos != std::string::npos)
	{
		// If there's a '/', separate the IP address and prefix
		std::string ip_part = ip_addr.substr(0, slash_pos);
		std::string prefix_part = ip_addr.substr(slash_pos + 1);

		// Validate the IP part
		if (!isValidIP(ip_part))
			return (false);

		// Validate the prefix length
		if (!checkdigits(prefix_part))
			return (false);
		int prefix = ft_stoi(prefix_part.c_str());
		if (prefix < 0 || prefix > 32)
			return (false);

		return (true); // Valid CIDR notation
	}

	for (size_t i = 0; i < ip_addr.size(); ++i)
	{
		if (ip_addr[i] == '.')
		{
			if (i == last_pos || i - last_pos > 3 || dots > 3)
				return (false);
			std::string segment = ip_addr.substr(last_pos, i - last_pos);
			if (!checkdigits(segment))
				return (false);
			int value = ft_stoi(segment.c_str());
			if (value < 0 || value > 255)
				return (false);
			last_pos = i + 1;
			dots++;
		}
	}

	// Validate last segment as there is no 4th dot in IP address IPv4
	if (dots != 3 || last_pos >= ip_addr.size())
		return (false);
	std::string last_segment = ip_addr.substr(last_pos);
	int value = ft_stoi(last_segment.c_str());
	if (!checkdigits(last_segment))
		return (false);
	return (value >= 0 && value <= 255);
}

bool isValidLocationPath(const std::string &path)
{
	// Define invalid characters
	static const std::string invalid_chars = "*?<>|\"\\\0";
	static const std::string whitespace = " \t\n\r";

	// Path must not be empty and start with /
	if (path.empty() || path[0] != '/')
		return (false);

	// Check for invalid characters
	if (path.find_first_of(invalid_chars) != std::string::npos)
		return (false);

	// No whitespaces
	if (path.find_first_of(whitespace) != std::string::npos)
		return (false);
		
	// Normalize and validate double slashes (e.g., "//")
	if (path.find("//") != std::string::npos)
		return (false);

	// If all checks pass, path is valid
	return (true);
}
