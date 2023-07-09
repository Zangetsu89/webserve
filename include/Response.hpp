#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <vector>

#include "Request.hpp"

class Response {

    private:
        Request _request;
        int _status_code;
        std::string _status_message;
        std::string _content_type;
        int _content_length;
        std::string _body;
        std::string _location;
        std::string _allow;
        std::string _date;
        std::string _last_modified;
        std::string _server;
        std::string _transfer_encoding;
        std::string _www_authenticate;
        std::string _accept_ranges;
        std::string _content_language;
        std::string _content_location;
        std::string _content_disposition;
        std::string _content_range;
        std::string _etag;
        std::string _expires;
        std::string _retry_after;
        std::string _vary;
        std::string _x_frame_options;
        std::string _x_xss_protection;
        std::string _x_content_type_options;
        std::string _x_powered_by;
        std::string _x_ua_compatible;
        std::string _x_content_duration;
        std::string _x_request_id;
        std::string _x_correlation_id;
        std::string _x_ms_request_id;
        std::string _x_ms_client_request_id;
        std::string _x_ms_version;
        std::string _x_ms_client_version;
        std::string _x_ms_client_principal_name;
        std::string _x_ms_client_user_agent;
        std::string _x_ms_client_app_id;
        std::string _x_ms_client_os;

    public:
        Response();
        Response(Request &request);
        Response(Response const &src);
        virtual ~Response();

        Response &operator=(Response const &rhs);

        void set_request(Request &request);
        void set_status_code(int status_code);
        void set_status_message(std::string status_message);
        void set_content_type(std::string content_type);
        void set_content_length(int content_length);
        void set_body(std::string body);
        void set_location(std::string location);
        void set_allow(std::string allow);
        void set_date();
        void set_last_modified(std::string last_modified);
        void set_server(std::string server);
        void set_transfer_encoding(std::string transfer_encoding);
        void set_www_authenticate(std::string www_authenticate);
        void set_accept_ranges(std::string accept_ranges);
        void set_content_language(std::string content_language);
        void set_content_location(std::string content_location);
        void set_content_disposition(std::string content_disposition);
        void set_content_range(std::string content_range);
        void set_etag(std::string etag);
        void set_expires(std::string expires);
        void set_retry_after(std::string retry_after);
        void set_vary(std::string vary);
        void set_x_frame_options(std::string x_frame_options);
        void set_x_xss_protection(std::string x_xss_protection);
        void set_x_content_type_options(std::string x_content_type_options);
        void set_x_powered_by(std::string x_powered_by);
        void set_x_ua_compatible(std::string x_ua_compatible);
        void set_x_content_duration(std::string x_content_duration);
        void set_x_request_id(std::string x_request_id);
        void set_x_correlation_id(std::string x_correlation_id);
        void set_x_ms_request_id(std::string x_ms_request_id);
        void set_x_ms_client_request_id(std::string x_ms_client_request_id);
        void set_x_ms_version(std::string x_ms_version);
        void set_x_ms_client_version(std::string x_ms_client_version);
        void set_x_ms_client_principal_name(std::string x_ms_client_principal_name);
        void set_x_ms_client_user_agent(std::string x_ms_client_user_agent);
        void set_x_ms_client_app_id(std::string x_ms_client_app_id);
        void set_x_ms_client_os(std::string x_ms
};
