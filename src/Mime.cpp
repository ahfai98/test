/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:20:32 by jyap              #+#    #+#             */
/*   Updated: 2024/12/25 20:48:54 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Mime.hpp"

// Multipurpose Internet Mail Extensions
// Maps file extensions to MIME types
// Format of MIME is type/subtype (like text/html)
// Tells browser/recipient how to handle the file/data

std::map<std::string, std::string> Mime::_mime_types;

Mime::Mime()
{
	// Manually insert MIME types
	_mime_types[".html"] = "text/html";
	_mime_types[".htm"] = "text/html";
	_mime_types[".css"] = "text/css";
	_mime_types[".avi"] = "video/x-msvideo";
	_mime_types[".bmp"] = "image/bmp";
	_mime_types[".doc"] = "application/msword";
	_mime_types[".gif"] = "image/gif";
	_mime_types[".gz"] = "application/x-gzip";
	_mime_types[".ico"] = "image/x-icon";
	_mime_types[".jpg"] = "image/jpeg";
	_mime_types[".jpeg"] = "image/jpeg";
	_mime_types[".png"] = "image/png";
	_mime_types[".txt"] = "text/plain";
	_mime_types[".mp3"] = "audio/mp3";
	_mime_types[".pdf"] = "application/pdf";
	_mime_types[".xml"] = "application/xml";
	_mime_types[".json"] = "application/json";
	_mime_types[".js"] = "application/javascript";
	_mime_types[".svg"] = "image/svg+xml";
	_mime_types[".mp4"] = "video/mp4";
	_mime_types[".zip"] = "application/zip";
	_mime_types[".woff"] = "font/woff";
	_mime_types[".md"] = "text/markdown";
	_mime_types["default"] = "text/html";
}

//handle uppercase of extension
std::string Mime::toLower(const std::string &str)
{
	std::string result = str;
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (result[i] >= 'A' && result[i] <= 'Z')
			result[i] += ('a' - 'A');
	}
	return (result);
}

std::string Mime::getMimeType(const std::string &extension)
{
	std::string ext = Mime::toLower(extension);
	if (_mime_types.count(ext))
		return (_mime_types[ext]);
	std::cerr << "Unknown MIME type for extension: " << ext << std::endl;
	std::cerr << "Using default MIME type: text/html" << std::endl;
	return (_mime_types["default"]);
}
