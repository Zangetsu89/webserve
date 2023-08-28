/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ResponseGen.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/15 17:24:16 by lizhang       #+#    #+#                 */
/*   Updated: 2023/08/28 16:20:05 by lizhang       ########   odam.nl         */
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

Response::Response() {
}

Response::Response(std::string cgiDir, Request R) {
    this->_request = R;
	this->_cgiDir = cgiDir;
}

Response::Response(Response const &source) {
    *this = source;
}

Response::~Response() {
}

Response &Response::operator=(Response const &source) {
    if (this != &source)
    {
        this->_request = source._request;
		this->_cgiDir = source._cgiDir;
    }
    return (*this);
}

void    Response::prepareResponse(char **env) {
    int fd[2];
    int fd2[2];
    int err;

    if (pipe(fd) == -1)
        throw ERR_Response("pipe failed", 500);
    if (pipe(fd2) == -1)
        throw ERR_Response("pipe failed", 500);
    // We need to pipe twice here to first read from the child process and then write to the server. We only fork once.
    int pid = fork();
    if (pid == 0)
    {
        close(fd[0]);
        dup2(fd[1], 1);
        responseGenerate(env);
    }
    else
    {
        close(fd[1]);
        waitpid(pid, &err, 0);
        if (err != 0)
            throw ERR_Response("fork failed", 500);
        char buffer[1024];
        int len = read(fd[0], buffer, 1024);
        if (len == -1)
            throw ERR_Response("read failed", 500);
        buffer[len] = '\0';
        std::cout << buffer << std::endl;
    }

}

//responseGenerate should need the following: 
//get: path
//post: path and content
//delete: path

static char **stringCharArray(std::vector<std::string> strVector)
{
	char *Array[strVector.size() + 1];
	for (unsigned int i = 0; i < strVector.size(); i++)
	{
		Array[i] = (char *)strVector[i].c_str();
	}
	Array[strVector.size()] = NULL;
	return(Array);
}

void    Response::responseGenerate(char **env)
{
	RequestHeader Header = *(_request.getRequestHeader());
	std::string method = Header.getRequestMethod();
	std::string path = Header.getRequestLocation();
	std::string content = this->_request.getRequestBody();
	bool permission = _request.getRequestDirSettings()->getDirPermission();
	
	std::cout<<"Response Generate function started."<<std::endl;

	if (opendir(path.c_str()) == NULL)
	{
		std::vector<std::string> arg;
		if (errno == ENOTDIR)
		{
			//this is the case with a file
			arg.push_back("/usr/local/bin/python3");
			if (method == "GET")
			{
				arg.push_back(this->_cgiDir + "/file_get.py");
				char **charArg = stringCharArray(arg);
				execve("/usr/local/bin/python3", charArg, env);
			}
			if (method == "POST")
			{
				char *arg[5];
				std::string cgiDir = this->_cgiDir + "file_post.py";
				arg[0] = (char *)("python3");
				arg[1] = (char *)cgiDir.c_str();
				arg[2] = (char *)path.c_str();
				arg[3] = (char *)content.c_str();
				arg[4] = NULL;
				execve("/usr/local/bin/python3", arg, env);
			}
			if (method == "DELETE")
			{
				char *arg[3];
				arg[0] = (char *)("python3");
				arg[1] = (char *)("./cgi/delete.py");
				arg[2] = NULL;
				execve("/usr/local/bin/python3", arg, env);
			}
			else
				return ;
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

void	Response::postRequest(Request R, char **env)
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


// exception
Response::ERR_Response::ERR_Response() : _error_msg("Response setting failed"), _error_num(0) {
}
Response::ERR_Response::ERR_Response(const char *error_msg, int err) : _error_msg(error_msg), _error_num(err){
}

const char *Response::ERR_Response::what() const _NOEXCEPT
{
return (_error_msg);
}