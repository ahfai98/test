/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 22:57:14 by jyap              #+#    #+#             */
/*   Updated: 2025/01/13 19:53:40 by jyap             ###   ########.fr       */
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
		std::vector<short>			_methods;
		std::string					_return;
		std::string					_alias;
		std::vector<std::string>	_cgi_path;
		std::vector<std::string>	_cgi_ext;
		unsigned long				_client_max_body_size;
		bool						methods_flag;
		bool						autoindex_flag;
		bool						maxsize_flag;

	public:
		std::map<std::string, std::string> _ext_path;
		Location();
		Location(const Location &other);
		Location &operator=(const Location &rhs);
		~Location();
		//setter
		void setPath(std::string parameter);
		void setRootLocation(std::string parameter);
		void setAutoindex(std::string parameter);
		void setIndexLocation(std::string parameter);
		void setMethods(std::vector<std::string> methods);
		void setReturn(std::string parameter);
		void setAlias(std::string parameter);
		void setCgiPath(std::vector<std::string> path);
		void setCgiExtension(std::vector<std::string> extension);
		void setMaxBodySize(std::string parameter);
		void setMaxBodySize(unsigned long parameter);
		void setMethodsFlag(bool parameter);
		void setAutoindexFlag(bool parameter);
		void setMaxSizeFlag(bool parameter);
		
		//getter
		const std::string &getPath() const;
		const std::string &getRootLocation() const;
		const bool &getAutoindex() const;
		const std::string &getIndexLocation() const;
		const std::vector<short> &getMethods() const;
		const std::string &getReturn() const;
		const std::string &getAlias() const;
		const std::vector<std::string> &getCgiPath() const;
		const std::vector<std::string> &getCgiExtension() const;
		const unsigned long &getMaxBodySize() const;
		const bool &getMethodsFlag() const;
		const bool &getAutoIndexFlag() const;
		const bool &getMaxSizeFlag() const;
		
		std::string getPrintMethods() const; //for debug
};

#endif
