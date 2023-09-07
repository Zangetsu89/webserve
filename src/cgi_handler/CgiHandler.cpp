/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CgiHandler.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/15 17:24:16 by lizhang       #+#    #+#                 */
/*   Updated: 2023/09/01 13:03:06 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/CgiHandler.hpp"

//The respons generate function should see if the path is a directory or 
//file. If directory, check if there is an index file, if yes, return the index
//file, if not, see the directory permission. If the permission is true, 
//read the directory and give back the list of content.
//If not a directory, check if plain text or a file: actually python handles this allready
//all plain text POST get saved in html/user directory?
//if GET searches for a plain file, look for the file in html/user directory?
//the POST method should contain plain text message?

CgiHandler::CgiHandler() {
}

CgiHandler::CgiHandler(Request R) {
    this->_request = R;
	//this->_cgiDir = R.getRequestServer()->getCGIDirSettings()->getCGIDir();
}

CgiHandler::CgiHandler(CgiHandler const &source) {
    *this = source;
}

CgiHandler::~CgiHandler() {
}

CgiHandler &CgiHandler::operator=(CgiHandler const &source) {
    if (this != &source)
    {
        this->_request = source._request;
		this->_cgiDir = source._cgiDir;
    }
    return (*this);
}

void    CgiHandler::prepareResponse(char **env) {
    int fd[2];
    int fd2[2];
    int err;

    if (pipe(fd) == -1)
        throw ERR_CgiHandler("pipe failed", 500);
    if (pipe(fd2) == -1)
        throw ERR_CgiHandler("pipe failed", 500);
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
            throw ERR_CgiHandler("fork failed", 500);
        char buffer[1024];
        ssize_t len = read(fd[0], buffer, 1024);
        if (len == -1)
            throw ERR_CgiHandler("read failed", 500);
        buffer[len] = '\0';
        std::cout << buffer << std::endl;
    }

}

static char **stringCharArray(std::vector<std::string> strVector)
{
	char **Array;
	Array = (char **)malloc(sizeof (char *)* strVector.size() + 1);
	for (unsigned int i = 0; i < strVector.size(); i++)
	{
		Array[i] = (char *)strVector[i].c_str();
	}
	Array[strVector.size()] = NULL;
	return((char **)Array);
}

static std::vector<std::string> findFolder(char **env)
{
	std::vector<std::string> folders;
	int i = 0;
	while (env[i])
	{
		if (std::string(env[i]).find("PATH=") != std::string::npos)
			break ;
		i++;
	}
	if (!env[i])
		throw(std::invalid_argument("Cannot execute."));
	folders = charSplit(std::string(env[i] + 5), ':');
	return folders;
}

static void exe(std::vector<std::string> arg, std::string command, char **env)
{
	std::vector<std::string> folders = findFolder(env);
	for (unsigned int i = 0; i < folders.size(); i++)
	{
		std::string path = folders[i]+"/"+command;
		arg.insert(arg.begin(), path);
		execve((char *)path.c_str(), stringCharArray(arg), env);
	}
}

void    CgiHandler::responseGenerate(char **env)
{
	RequestHeader Header = *(_request.getRequestHeader());
	std::string method = Header.getRequestMethod();
	std::string path = Header.getRequestLocation();
	std::string content = this->_request.getRequestBody();
	bool permission = _request.getRequestDirSettings()->getDirPermission();

	std::cout<<"Response Generate function started."<<std::endl;

	if (opendir(path.c_str()) == nullptr)
	{
		std::vector<std::string> arg;
		if (errno == ENOTDIR)
		{
			if (method == "GET")
			{
				arg.push_back(this->_cgiDir + "/file_get.py");
				arg.push_back(path);
				exe(arg, "python3", env);
			}
			else if(method == "POST")
			{
				arg.push_back(this->_cgiDir + "/file_post.py");
				arg.push_back(path);
				//arg.push_back(Header.getContentType());
				arg.push_back(content);
				exe(arg, "python3", env);
			}
			else if(method == "DELETE")
			{
				arg.push_back(this->_cgiDir + "/delete.py");
				arg.push_back(path);
				exe(arg, "python3", env);
			}
//			if (err!= 0)
//				arg[3] = (char *)("false");
//			else
//				arg[3] = (char *)("true");
//				arg[1] = (char *)("./cgi/delete.py");
//			}
//			else
//				return ;
//			execve("python3", arg, env);
			else
			{
				arg.push_back(this->_cgiDir + "/request_not_allowd.py");
				exe(arg, "python3", env);
			}
		}
	}
	else
	{
		//this is the case of a directory{
		std::vector<std::string> arg;
		if (method == "GET")
		{
			if (permission == true)
			{
				arg.push_back(this->_cgiDir + "/directory_get.py");
				arg.push_back(path);
				arg.push_back("true");
				exe(arg, "python3", env);
			}
			else
			{
				arg.push_back(this->_cgiDir + "/directory_get.py");
				arg.push_back(path);
				arg.push_back("false");
				exe(arg, "python3", env);
			}
		}
		else if (method == "DELETE")
		{
			arg.push_back(this->_cgiDir + "/delete.py");
			exe(arg, "python3", env);
		}
		else
		{
			arg.push_back(this->_cgiDir + "/request_not_allowd.py");
			exe(arg, "python3", env);
		}
	}
}

//int	CgiHandler::postRequest(Request R, char **env)
//{
//    int err;
//    //get file directory
//    std::string path = R.getRequestFilePath();
//    unsigned int pos = path.rfind("/");
//    std::string dir = path.substr(0, pos);
//    std::string content; //write the Header and typer in content?
//
//    char *command[2];
//    command[0] = (char*)("mkdir");
//    command[1] = (char *)dir.c_str();
//    int pid = fork();
//    if (pid == 0)
//        execve("mkdir", command, env);
//    else
//    {
//        waitpid(pid, &err, 0);
//        if (err!= 0)
//            return (err);
//        else
//        {
//            int pid2;
//            pid2 = fork();
//            if (pid == 0)
//                execve("echo", command, env);
//            else
//            {
//                std::cout << "TEST\n" << std::endl;
////                waidpid(pid2, &err, 0);
//            }
//        }
//    }
// }


CgiHandler::ERR_CgiHandler::ERR_CgiHandler() : _error_msg("Request setting failed"), _error_num(0) {}
CgiHandler::ERR_CgiHandler::ERR_CgiHandler(const char *error_msg, int err) : _error_msg(error_msg), _error_num(err)
{

}

const char *CgiHandler::ERR_CgiHandler::what() const _NOEXCEPT
{
    return (_error_msg);
}