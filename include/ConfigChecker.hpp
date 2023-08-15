/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ConfigChecker.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/08 14:05:41 by lizhang       #+#    #+#                 */
/*   Updated: 2023/08/08 14:42:00 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGCHECKER_HPP
#define CONFIGCHECKER_HPP

#include "Config.hpp"

void    checkConfigFile(Config C1);
void	checkDirMethods(DirSettings D1, unsigned int server_number);

#endif