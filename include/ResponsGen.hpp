/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Respons.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/15 17:24:41 by lizhang       #+#    #+#                 */
/*   Updated: 2023/08/15 18:01:43 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONS_HPP
#define RESPONS_HPP

#include "Request.hpp"
#include "RequestHeader.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

void    ResponsGenerate(Request R, char **env);

#endif