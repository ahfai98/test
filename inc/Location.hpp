/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 22:57:14 by jyap              #+#    #+#             */
/*   Updated: 2025/01/13 18:31:41 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "Webserv.hpp"

class Location
{
	private:
		std::string					_path;
		std::string					_root;
		bool						_autoindex;
		std::string					_index;
		std::vector<short>			_methods; // GET+ POST- DELETE- PUT- HEAD-
		std::string					_return;
		std::string					_alias;
		std::vector<std::string>	_cgi_path;
		std::vector<std::string>	_cgi_ext;
		unsigned long				_client_max_body_size;
		bool						allow_flag;
		bool						deny_flag;
		bool						methods_flag;
		bool						autoindex_flag;
		bool						maxsize_flag;

	public:
		std::map<std::string, std::string> _ext_path;

		Location();
		Location(const Location &other);
		Location &operator=(const Location &rhs);
		~Location();

		void setPath(std::string parameter);
		void setRootLocation(std::string parameter);
		void setMethods(std::vector<std::string> methods);
		void setAutoindex(std::string parameter);
		void setIndexLocation(std::string parameter);
		void setReturn(std::string parameter);
		void setAlias(std::string parameter);
		void setCgiPath(std::vector<std::string> path);
		void setCgiExtension(std::vector<std::string> extension);
		void setMaxBodySize(std::string parameter);
		void setMaxBodySize(unsigned long parameter);
		void setAllowFlag(bool parameter);
		void setDenyFlag(bool parameter);
		bool isIPAllowed(const std::string &clientIP);
		bool isIPDenied(const std::string &clientIP);
		void setAutoIndexFlag(bool parameter);
		void setMethodsFlag(bool parameter);
		void setMaxSizeFlag(bool parameter);

		const std::string &getPath() const;
		const std::string &getRootLocation() const;
		const std::vector<short> &getMethods() const;
		const bool &getAutoindex() const;
		const std::string &getIndexLocation() const;
		const std::string &getReturn() const;
		const std::string &getAlias() const;
		const std::vector<std::string> &getCgiPath() const;
		const std::vector<std::string> &getCgiExtension() const;
		const std::map<std::string, std::string> &getExtensionPath() const;
		const unsigned long &getMaxBodySize() const;

		const bool &getMethodsFlag() const;
		const bool &getAutoIndexFlag() const;
		const bool &getMaxSizeFlag() const;
		
		std::string getPrintMethods() const; // for checking only
};

#endif
