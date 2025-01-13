/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 21:56:57 by jyap              #+#    #+#             */
/*   Updated: 2025/01/13 18:41:23 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include "Webserv.hpp"

class HttpRequest;
class CgiHandler {
	private:
		std::map<std::string, std::string>	_env;
		char**								_ch_env;
		char**								_argv;
		int									_exit_status;
		std::string							_cgi_path;
		pid_t								_cgi_pid;

	public:
		int	pipe_in[2];
		int	pipe_out[2];

		CgiHandler();
		CgiHandler(std::string path);
		~CgiHandler();
		CgiHandler(const CgiHandler &other);
		CgiHandler &operator=(const CgiHandler &rhs);

		void initEnvCgi(HttpRequest& req, const std::vector<Location>::iterator it_loc);
		void execute(short &error_code);
		void clear();
		void setCgiPath(const std::string &cgi_path);

		const pid_t &getCgiPid() const;
		const std::string &getCgiPath() const;
		std::string	getPathInfo(std::string& path, std::vector<std::string> extensions);
		int findStart(const std::string path, const std::string delim);
		std::string decode(std::string &path);
};

#endif
