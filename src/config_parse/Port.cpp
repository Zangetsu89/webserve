/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Port.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/16 13:02:35 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/19 15:17:41 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Port.hpp"

Port::Port()
{
}

Port::Port(std::string name, int number)
{
	this->_name = name;
	this->_number = number;
}

Port::Port(Port const &another)
{
	this->_name = another._name;
	this->_number = another._number;
}

Port::~Port()
{
}

Port	&Port::operator=(Port const &another)
{
	this->_name = another._name;
	this->_number = another._number;
	return (*this);
}

std::string Port::getPortName() const
{
	return (this->_name);
}

int Port::getPortNumber() const
{
	return (this->_number);
}