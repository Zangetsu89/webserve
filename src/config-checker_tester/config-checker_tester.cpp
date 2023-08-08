/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   config-checker_tester.cpp                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/08 14:43:26 by lizhang       #+#    #+#                 */
/*   Updated: 2023/08/08 14:56:22 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ConfigChecker.hpp"

int main(void)
{
	Config C1("src/config-checker_tester/MacOS_test.conf");
	try
	{
		checkConfigFile(C1);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	std::cout<<"test run successfully."<<std::endl;
	return (0);
}