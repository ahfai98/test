/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:19:09 by jyap              #+#    #+#             */
/*   Updated: 2025/01/13 19:42:51 by jyap             ###   ########.fr       */
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
		Mime();
		Mime(const Mime&);
		Mime &operator=(const Mime&);
		static std::map<std::string, std::string> _mime_types;
		static std::string toLower(const std::string &str);
};

#endif
