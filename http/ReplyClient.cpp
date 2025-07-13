/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReplyClient.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pokpalae <pokpalae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 21:28:41 by pokpalae          #+#    #+#             */
/*   Updated: 2025/06/18 20:11:50 by pokpalae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/ReplyClient.hpp"


ReplyClient::ReplyClient() 
{
    _status_code = UNDEFINED;
    Http_version_ = "HTTP/1.1";
}

ReplyClient::~ReplyClient() {

}

void    ReplyClient::ShowReply(void) {
    std::cout << "------Displaying Response------: " << std::endl;
    std::cout << _raw_status_line << std::endl << _raw_headers << std::endl << _raw_body << std::endl;
}

str ReplyClient::getServerReply(void) {
    return (_raw_response);
}

void    ReplyClient::setServerReply(void) {
    _raw_response = _raw_status_line + _raw_headers + _raw_body;
}



void ReplyClient::composeHeaderString()
{
    std::ostringstream ss;
    std::map<std::string, std::string>::const_iterator it = _headers.begin();

    while (it != _headers.end())
    {
        ss << it->first << ": " << it->second << "\r\n";
        ++it;
    }

    ss << "\r\n"; // End of headers
    _raw_headers = ss.str();
}


void ReplyClient::addDateHeader(void)
{
    time_t now = time(NULL);                      // Get current time
    struct tm* timeinfo = gmtime(&now);           // Convert to UTC time
    char buffer[99];                             // Temporary buffer for formatted time

    // Format: "Day, DD Mon YYYY HH:MM:SS GMT"
    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", timeinfo);

    std::string time_str(buffer);                 // Convert C-style buffer to C++ string
    _headers["Date"] = time_str; 
}


void ReplyClient::addContentLengthToHeaders() {
    std::ostringstream oss;
    oss << _raw_body.length();

    std::string content_length = oss.str();
    _headers.insert(std::make_pair("Content-Length", content_length));
}


void ReplyClient::addConnectionHeader(str connection_type) {
    _headers.insert(std::make_pair("Connection", connection_type));
}

void ReplyClient::addContentTypeHeader() {
    const char* content_type = NULL;

    if (hasFileExtension(_resource, ".html") || _status_code == BAD_REQUEST || _status_code == INTERNAL_SERVER_ERROR)
        content_type = "text/html";
    else if (hasFileExtension(_resource, ".css"))
        content_type = "text/css";
    else if (hasFileExtension(_resource, ".ico"))
        content_type = "image/x-icon";
    else if (hasFileExtension(_resource, ".jpeg"))
        content_type = "image/jpeg";
    else if (hasFileExtension(_resource, ".png"))
        content_type = "image/png";
    else if (hasFileExtension(_resource, ".json"))
        content_type = "application/json";
    else if (isDirectory(_resource))
        content_type = "text/html";

    if (content_type)
        _headers.insert(std::make_pair("Content-Type", std::string(content_type)));
}


void ReplyClient::addCacheControlHeader(const char* directive) {
    _headers.insert(std::make_pair("Cache-Control", std::string(directive)));
}


void ReplyClient::addRetryAfterHeader(int seconds)
{
    std::ostringstream oss;
    oss << seconds;
    _headers.insert(std::make_pair("Retry-After", oss.str()));
}


void ReplyClient::setHostHeader(const char* server_name) {
    _headers.insert(std::make_pair("Host", std::string(server_name)));
}


void ReplyClient::initializeResourceFromRequest(ClientRequest& request) {
    _resource = request.getResolvedResourcePath();
    return;
}


void    ReplyClient::setStatusCode(StatusCode s) {
    _status_code = s;
}



void ReplyClient::addSetCookieHeader(const std::string& name, const std::string& value, const std::string& path, int maxAge) {
    std::ostringstream cookieHeader;
    cookieHeader << name << "=" << value << "; Path=" << path << "; Max-Age=" << maxAge << "; HttpOnly";
    _headers["Set-Cookie"] = cookieHeader.str();
}
