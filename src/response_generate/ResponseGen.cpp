/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ResponseGen.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/15 17:24:16 by lizhang       #+#    #+#                 */
/*   Updated: 2023/08/22 17:56:31 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ResponseGen.hpp"

//The respons generate function should see if the path is a directory or 
//file. If directory, check if there is an index file, if yes, return the index
//file, if not, see the directory permission. If the permission is true, 
//read the directory and give back the list of content.
//If not a directory, check if plain text or a file: actually python handles this allready
//all plain text POST get saved in html/user directory?
//if GET searches for a plain file, look for the file in html/user directory?
//the POST method should contain plain text message?

int	postRequest(Request R, char **env)
{
	int err;
	//get file directory
	std::string path = R.getRequestFilePath();
	unsigned int pos = path.rfind("/");
	std::string dir = path.substr(0, pos);
	std::string content; //write the Header and typer in content?

	char *command[2];
	command[0] = (char*)("mkdir");
	command[1] = (char *)dir.c_str();
	int pid = fork();
	if (pid == 0)
		execve("mkdir", command, env);
	else
	{
		waitpid(pid, &err, 0);
		if (err!= 0)
			return (err);
		else
		{
			int pid2;
			pid2 = fork();
			if (pid == 0)
				execve("echo", command, env);
			else
			{
				waidpid(pid2, &err, 0);
			}
		}
	}
}


void    ResponseGenerate(Request R, char **env)
{
	RequestHeader Header = *(R.getRequestHeader());
	std::string method = Header.getRequestMethod();
	std::string path = Header.getRequestLocation();
	bool permission = R.getRequestDirSettings()->getDirPermission();
	
	std::cout<<"Response Generate function started."<<std::endl;

	if (opendir(path.c_str()) == NULL)
	{
		if (errno == ENOTDIR)
		{
			//this is the case with a file
			char *arg[3];
			arg[0] = (char *)("python3");
			arg[2] = (char *)path.c_str();
			if (method == "GET")
			{
				arg[1] = (char *)("./cgi/file_get.py");
			}
			if (method == "DELETE")
			{
				int err;
			int pid = fork();
			if (pid == 0)
			{
				char *command[3];
				command[0] = (char*)("rm");
				command[1] = (char*)("-r");
				command[2] = (char *)path.c_str();
				execve("rm", command, env);
			}
			waitpid(pid, &err, 0);
			if (err!= 0)
				arg[3] = (char *)("false");
			else
				arg[3] = (char *)("true");
				arg[1] = (char *)("./cgi/delete.py");
			}
			else
				return ;
			execve("python3", arg, env);
		}
	}
	else
	{
		//this is the case of a directory{
		char *arg[4];
		arg[0] = (char *)("python3");
		arg[2] = (char *)path.c_str();
		if (method == "GET")
		{
			if (permission == true)
				arg[3] = (char *)("true");
			else
				arg[3] = (char *)("false");
			arg[1] = (char *)("./cgi/directory_get.py");
		}
		if (method == "POST")
		{
			//here should be a function to fork and extract Request_data
			//save it in a specified directory, if not exist, make the directory
			arg[1] = (char *)("./cgi/file_post.py");
		}
		if (method == "DELETE")
		{
			int err;
			int pid = fork();
			if (pid == 0)
			{
				char *command[2];
				command[0] = (char*)("rm");
				command[1] = (char *)path.c_str();
				execve("rm", command, env);
			}
			waitpid(pid, &err, 0);
			if (err!= 0)
				arg[3] = (char *)("false");
			else
				arg[3] = (char *)("true");
			arg[1] = (char *)("./cgi/delete.py");
		}
		execve("python3", arg, env);
	}
}