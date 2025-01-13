/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:19:09 by jyap              #+#    #+#             */
/*   Updated: 2024/12/25 20:46:10 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIME_HPP
#define MIME_HPP

#include "Webserv.hpp"

class Mime
{
	public:
		static std::string getMimeType(const std::string &extension);
	private:
		//Prevent instantiation and copying
		Mime();
		Mime(const Mime&);
		Mime &operator=(const Mime&);
		// Static map of MIME types
		static std::map<std::string, std::string> _mime_types;
		static std::string toLower(const std::string &str);
};

#endif
