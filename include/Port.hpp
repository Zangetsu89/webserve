/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Port.hpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/16 12:54:31 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/19 15:17:33 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PORT_HPP
# define PORT_HPP

#include <string>

class Port
{
	private:
	std::string     _name;
	int             _number; //the port numbers are first the ip adress, at the end the port numbers

	public:
		Port();
		Port(std::string name, int number);
		Port(Port const &another);
		~Port();
		Port		&operator=(Port const &another);
		
		std::string	getPortName() const;
		int			getPortNumber() const;
};

#endif