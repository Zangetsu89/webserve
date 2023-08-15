/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Respons.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/15 17:24:16 by lizhang       #+#    #+#                 */
/*   Updated: 2023/08/15 18:11:47 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Respons.hpp"

//The respons generate function should see if the path is a directory or 
//file. If directory, check if there is an index file, if yes, return the index
//file, if not, see the directory permission. If the permission is true, 
//read the directory and give back the list of content.
//If not a directory, check if plain text or a file: actually python handles this allready
//all plain text POST get saved in html/user directory?
//if GET searches for a plain file, look for the file in html/user directory?

void    ResponsGenerate(Request R, char **env)
{
	RequestHeader Header = *(R.getRequestHeader());
	std::string method = Header.getRequestMethod();
	std::string path = Header.getRequestLocation();
	if (opendir(path.c_str()) == NULL)
	{
		if (errno = ENOTDIR)
		{
			//this is the case with a file
			char *arg[3];
			arg[1] = "python";
			arg[3] = (char *)path.c_str();
			if (method == "GET")
			{
				arg[2] = "./cgi/file_get.py";
			}
			if (method == "POST")
			{
				arg[2] = "./cgi/file_post.py";
			}
			if (method == "DELETE")
			{
				arg[2] = "./cgi/standard_delete.py";
			}
			execve("python", arg, env);
		}
	}
	else
	{
		//this is the case of a directory
		if (method == "GET")
		{
			char *arg[3];
			arg[1] = "python";
			arg[3] = (char *)path.c_str();
			if (method == "GET")
			{
				arg[2] = "./cgi/standard_get.py";
			}
			if (method == "POST")
			{
				arg[2] = "./cgi/standard_post.py";
			}
			if (method == "DELETE")
			{
				arg[2] = "./cgi/standard_delete.py";
			}
			execve("python", arg, env);
		}
	}
}