/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 22:58:25 by jyap              #+#    #+#             */
/*   Updated: 2025/01/13 18:33:09 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Location.hpp"

Location::Location()
{
	this->_path = "";
	this->_root = "";
	this->_autoindex = false;
	this->_index = "";
	this->_return = "";
	this->_alias = "";
	this->_client_max_body_size = MAX_CONTENT_LENGTH;
	this->_methods.reserve(5);
	this->_methods.push_back(1);
	this->_methods.push_back(0);
	this->_methods.push_back(0);
	this->_methods.push_back(0);
	this->_methods.push_back(0);
	this->allow_flag = false;
	this->deny_flag = false;
	this->methods_flag = false;
	this->autoindex_flag = false;
	this->maxsize_flag = false;
}

Location::Location(const Location &other)
{
	*this = other;
}

Location &Location::operator=(const Location &rhs)
{
	if (this != &rhs)
	{
		this->_path = rhs._path;
		this->_root = rhs._root;
		this->_autoindex = rhs._autoindex;
		this->_index = rhs._index;
		this->_cgi_path = rhs._cgi_path;
		this->_cgi_ext = rhs._cgi_ext;
		this->_return = rhs._return;
		this->_alias = rhs._alias;
		this->_methods = rhs._methods;
		this->_ext_path = rhs._ext_path;
		this->_client_max_body_size = rhs._client_max_body_size;
		this->allow_flag = rhs.allow_flag;
		this->deny_flag = rhs.deny_flag;
		this->methods_flag = rhs.methods_flag;
		this->maxsize_flag = rhs.maxsize_flag;
		this->autoindex_flag = rhs.autoindex_flag;
	}
	return (*this);
}

Location::~Location() {}

/* set functions */
void Location::setPath(std::string parameter)
{
	this->_path = parameter;
}

void Location::setRootLocation(std::string parameter)
{
	if (getPathType(parameter) != IS_DIRECTORY)
		throw ServerConfig::ErrorException("Wrong root of location: " + parameter);
	this->_root = parameter;
}

void Location::setMethods(std::vector<std::string> methods)
{
	for (size_t i = 0; i < 5; i++)
		this->_methods[i] = 0;

	const char* allowed_methods[] = {"GET", "POST", "DELETE", "PUT", "HEAD"};
	for (size_t i = 0; i < methods.size(); i++)
	{
		bool method_found = false;
		for (size_t j = 0; j < 5; j++)
		{
			if (methods[i] == allowed_methods[j])
			{
				this->_methods[j] = 1;
				method_found = true;
				break;
			}
		}
		if (!method_found)
			throw ServerConfig::ErrorException("Allow method not supported: " + methods[i]);
	}
}

void Location::setAutoindex(std::string parameter)
{
	if (parameter == "on" || parameter == "off")
		this->_autoindex = (parameter == "on");
	else
		throw ServerConfig::ErrorException("Wrong autoindex: " + parameter);
}

void Location::setIndexLocation(std::string parameter)
{
	this->_index = parameter;
}

void Location::setReturn(std::string parameter)
{
	this->_return = parameter;
}

void Location::setAlias(std::string parameter)
{
	this->_alias = parameter;
}

void Location::setCgiPath(std::vector<std::string> path)
{
	this->_cgi_path = path;
}

void Location::setCgiExtension(std::vector<std::string> extension)
{
	this->_cgi_ext = extension;
}

void Location::setAllowFlag(bool parameter)
{
	this->allow_flag = parameter;
}

void Location::setDenyFlag(bool parameter)
{
	this->deny_flag = parameter;
}

void Location::setAutoIndexFlag(bool parameter)
{
	this->autoindex_flag = parameter;
}

void Location::setMaxSizeFlag(bool parameter)
{
	this->maxsize_flag = parameter;
}

void Location::setMethodsFlag(bool parameter)
{
	this->methods_flag = parameter;
}

void Location::setMaxBodySize(std::string parameter)
{
	unsigned long body_size = 0;

	for (size_t i = 0; i < parameter.length(); i++)
	{
		if (parameter[i] < '0' || parameter[i] > '9')
			throw ServerConfig::ErrorException("Wrong syntax: client_max_body_size: " + parameter);
	}
	if (!ft_stoi(parameter))
		throw ServerConfig::ErrorException("Wrong syntax: client_max_body_size:" + parameter);
	body_size = ft_stoi(parameter);
	this->_client_max_body_size = body_size;
}

void Location::setMaxBodySize(unsigned long parameter)
{
	this->_client_max_body_size = parameter;
}

/* get functions */
const std::string &Location::getPath() const
{
	return (this->_path);
}

const std::string &Location::getRootLocation() const
{
	return (this->_root);
}

const std::string &Location::getIndexLocation() const
{
	return (this->_index);
}

const std::vector<short> &Location::getMethods() const
{
	return (this->_methods);
}

const std::vector<std::string> &Location::getCgiPath() const
{
	return (this->_cgi_path);
}

const std::vector<std::string> &Location::getCgiExtension() const
{
	return (this->_cgi_ext);
}

const bool &Location::getAutoindex() const
{
	return (this->_autoindex);
}

const std::string &Location::getReturn() const
{
	return (this->_return);
}

const std::string &Location::getAlias() const
{
	return (this->_alias);
}

const std::map<std::string, std::string> &Location::getExtensionPath() const
{
	return (this->_ext_path);
}

const unsigned long &Location::getMaxBodySize() const
{
	return (this->_client_max_body_size);
}

const bool &Location::getMethodsFlag() const
{
	return (this->methods_flag);
}

const bool &Location::getAutoIndexFlag() const
{
	return (this->autoindex_flag);
}

const bool &Location::getMaxSizeFlag() const
{
	return (this->maxsize_flag);
}

//print allowed methods for Location
std::string Location::getPrintMethods() const
{
	std::string res;
	bool first = true;

	if (_methods[0])
	{ // GET
		if (!first)
			res += ", ";
		res += "GET";
		first = false;
	}
	if (_methods[1])
	{ // POST
		if (!first)
			res += ", ";
		res += "POST";
		first = false;
	}
	if (_methods[2])
	{ // DELETE
		if (!first)
			res += ", ";
		res += "DELETE";
		first = false;
	}
	if (_methods[3])
	{ // PUT
		if (!first)
			res += ", ";
		res += "PUT";
		first = false;
	}
	if (_methods[4])
	{ // HEAD
		if (!first)
			res += ", ";
		res += "HEAD";
	}
	return (res);
}

