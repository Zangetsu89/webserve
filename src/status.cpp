#include "../include/status.hpp"
#include <map>

std::string set_Status(int num)
{
	switch (num)
	{
	// 1xx informational response
	case 100:
		return "Continue";
	case 101:
		return "Switching Protocols";
	case 102:
		return "Processing";
	case 103:
		return "Early Hints";

	// 2xx successful
	case 200:
		return "OK";
	case 201:
		return "Created";
	case 202:
		return "Accepted";
	case 203:
		return "Non-Authoritative Information";
	case 204:
		return "No Content";
	case 205:
		return "Reset Content";
	case 206:
		return "Partial Content";
	case 207:
		return "Multi-Status";
	case 208:
		return "Already Reported";
	case 226:
		return "IM Used";

	// 3xx redirection
	case 300:
		return "Multiple Choices";
	case 301:
		return "Moved Permanently";
	case 302:
		return "Found";
	case 303:
		return "See Other";
	case 304:
		return "Not Modified";
	case 307:
		return "Temporary Redirect";
	case 308:
		return "Permanent Redirect";

	default:
		return "Unknown";
	}
}

std::string set_ErrorStatus(int num)
{
	switch (num)
	{
	// 4xx client errors
	case 400:
		return "Bad Request";
	case 401:
		return "Unauthorized";
	case 402:
		return "Payment Required";
	case 403:
		return "Forbidden";
	case 404:
		return "Not Found";
	case 405:
		return "Method Not Allowed";
	case 406:
		return "Not Acceptable";
	case 407:
		return "Proxy Authentication Required";
	case 408:
		return "Request Timeout";
	case 409:
		return "Conflict";
	case 410:
		return "Gone";
	case 411:
		return "Length Required";
	case 412:
		return "Precondition Failed";
	case 413:
		return "Payload Too Large";
	case 414:
		return "URI Too Long";
	case 415:
		return "Unsupported Media Type";
	case 416:
		return "Range Not Satisfiable";
	case 417:
		return "Expectation Failed";
	case 418:
		return "I'm a teapot";
	case 421:
		return "Misdirected Request";
	case 422:
		return "Unprocessable Content";
	case 423:
		return "Locked";
	case 424:
		return "Failed Dependency";
	case 425:
		return "Too Early";
	case 426:
		return "Upgrade Required";
	case 428:
		return "Precondition Required";
	case 429:
		return "Too Many Requests";
	case 431:
		return "Request Header Fields Too Large";
	case 451:
		return "Unavailable For Legal Reasons";

	// 5xx server errors
	case 500:
		return "Internal Server Error";
	case 501:
		return "Not Implemented";
	case 502:
		return "Bad Gateway";
	case 503:
		return "Service Unavailable";
	case 504:
		return "Gateway Timeout";
	case 505:
		return "HTTP Version Not Supported";
	case 506:
		return "Variant Also Negotiates";
	case 507:
		return "Insufficient Storage";
	case 508:
		return "Loop Detected";
	case 510:
		return "Not Extended";
	case 511:
		return "Network Authentication Required";

	default:
		return "Unknown";
	}
}

static std::map<std::string, std::string> MIMETypes = {
	{"aac", "audio/aac"},
	{"abw", "application/x-abiword"},
	{"arc", "application/x-freearc"},
	{"avif", "image/avif"},
	{"avi", "video/x-msvideo"},
	{"azw", "application/vnd.amazon.ebook"},
	{"bin", "application/octet-stream"},
	{"bmp", "image/bmp"},
	{"bz", "application/x-bzip"},
	{"bz2", "application/x-bzip2"},
	{"cda", "application/x-cdf"},
	{"csh", "application/x-csh"},
	{"css", "text/css"},
	{"csv", "text/csv"},
	{"doc", "application/msword"},
	{"docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
	{"eot", "application/vnd.ms-fontobject"},
	{"epub", "application/epub+zip"},
	{"gz", "application/gzip"},
	{"gif", "image/gif"},
	{"htm", "text/html"},
	{"html", "text/html"},
	{"ico", "image/vnd.microsoft.icon"},
	{"ics", "text/calendar"},
	{"jar", "application/java-archive"},
	{"jpeg", "image/jpeg"},
	{"jpg", "image/jpeg"},
	{"js", "text/javascript"},
	{"json", "application/json"},
	{"jsonld", "application/ld+json"},
	{"mid", "audio/midi audio/x-midi"},
	{"mjs", "text/javascript"},
	{"mp3", "audio/mpeg"},
	{"mp4", "video/mp4"},
	{"mpeg", "video/mpeg"},
	{"mpkg", "application/vnd.apple.installer+xml"},
	{"odp", "application/vnd.oasis.opendocument.presentation"},
	{"ods", "application/vnd.oasis.opendocument.spreadsheet"},
	{"odt", "application/vnd.oasis.opendocument.text"},
	{"oga", "audio/ogg"},
	{"ogv", "video/ogg"},
	{"ogx", "application/ogg"},
	{"opus", "audio/opus"},
	{"otf", "font/otf"},
	{"png", "image/png"},
	{"pdf", "application/pdf"},
	{"php", "application/x-httpd-php"},
	{"ppt", "application/vnd.ms-powerpoint"},
	{"pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"},
	{"rar", "application/vnd.rar"},
	{"rtf", "application/rtf"},
	{"sh", "application/x-sh"},
	{"svg", "image/svg+xml"},
	{"tar", "application/x-tar"},
	{"tif", "image/tiff"},
	{"ts", "video/mp2t"},
	{"ttf", "font/ttf"},
	{"txt", "text/plain"},
	{"vsd", "application/vnd.visio"},
	{"wav", "audio/wav"},
	{"weba", "audio/webm"},
	{"webm", "video/webm"},
	{"webp", "image/webp"},
	{"woff", "font/woff"},
	{"woff2", "font/woff2"},
	{"xhtml", "application/xhtml+xml"},
	{"xls", "application/vnd.ms-excel"},
	{"xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
	{"xml", "application/xml"},
	{"xul", "application/vnd.mozilla.xul+xml"},
	{"zip", "application/zip"},
	{"3gp", "video/3gpp"},
	{"3g2", "video/3gpp2"},
	{"7z", "application/x-7z-compressed"},
};

std::string get_MIMETypes(std::string file)
{
	std::string extension;
	std::string filename = file;
	size_t pos_slice;

	if (filename.front() == '.')
		filename.erase(0, 1);
	pos_slice = filename.find(".", 0);
	if (pos_slice == std::string::npos)
		return ("text/html");
	extension = filename.substr(pos_slice, filename.size());
	if (extension.front() == '.')
		extension.erase(0, 1);
	std::map<std::string, std::string>::iterator it;
	it = MIMETypes.find(extension);
	if (it != MIMETypes.end())
	{
		// std::cout << "MIMEType is " << it->second << std::endl;
		return (it->second);
	}
	return ("application/octet-stream");
}