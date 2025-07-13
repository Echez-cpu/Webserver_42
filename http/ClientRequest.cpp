/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientRequest.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pokpalae <pokpalae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 21:28:41 by pokpalae          #+#    #+#             */
/*   Updated: 2025/06/24 20:11:50 by pokpalae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/ClientRequest.hpp"

ClientRequest::ClientRequest() {
   _request_method = NOT_SET;
    _body_bytes_read = 0;
    _has_body = false;
    _resource = "";
    _unparsed_request = "";
    _raw_body = "";
    _redirect_required = false;
    _redirect_status = 0;
    _redirect_target = "";
    _directoryListingEnabled = false;
}

ClientRequest::~ClientRequest() {}


bool     ClientRequest::fullyParsedHeaders() 
{
    if (_unparsed_request.find("\r\n\r\n") != std::string::npos)
        return (true);
    else
        return (false);
}

bool ClientRequest::hasCompleteRequest() {
    if (!fullyParsedHeaders())
        return false;

    if (!_has_body)
        return true;

    return (_raw_body.length() == _body_length);
}

int ClientRequest::parseClientRequest(char *buf, int bytes_read, ServerConfiguration* sb) {

    int start = bytes_read;

    while (start < 2000) {
        char current_char = buf[start];

        if (!fullyParsedHeaders()) {
            _unparsed_request += current_char;

            if (_unparsed_request.find("\r\n\r\n") != std::string::npos) {
                // Complete headers received
                extractRequestMetadata(sb);
                extractHeadersFromRequest();
                debugPrintRequest();
            }

            ++bytes_read;
        } else if (_has_body) 
        {
            _raw_body += current_char;
            ++_body_bytes_read;
            ++bytes_read;

            if (static_cast<unsigned long>(_body_bytes_read) == _body_length) {
                if (_body_length > sb->getMaxClientBodySize()) {
                    throw ClientRequest::PayloadTooLargeException();
                }
                break; // Full body received
            }
        }

        ++start;
    }

    return bytes_read;
}


// LocationBlock* loc = locationExistsInBlock(sb->getRoutes(), path);
// if (!loc)
//     throw UnauthorizedException();

// if (loc->hasRedirect()) {
//     _redirect_required = true;
//     _redirect_status = loc->getRedirectStatus();
//     _redirect_target = loc->getRedirectTarget();
//     return;
// } else {
//     _resource = loc->getDocumentRoot() + "/" + path + "/" + loc->getDefaultIndex();
// }




void ClientRequest::extractRequestMetadata(ServerConfiguration* sb) {
    std::vector<std::string> startline_split;
    std::string keys[] = {"GET", "POST", "DELETE"};

    // Extract raw start line (first line of HTTP request)
    std::size_t line_end = _unparsed_request.find("\r\n");
    _raw_start_line = _unparsed_request.substr(0, line_end);

    // Trim garbage values before the method keyword (GET, POST, DELETE)
    std::size_t pos = _raw_start_line.length();
    for (int i = 0; i < 3; ++i) {
        std::size_t keyPos = _raw_start_line.find(keys[i]);
        if (keyPos != std::string::npos && keyPos < pos)
            pos = keyPos;
    }
    if (pos != _raw_start_line.length())
        _raw_start_line = _raw_start_line.substr(pos);

    // Remove the processed start line from the unparsed request
    _unparsed_request = _unparsed_request.substr(line_end + 2);

    // Split the start line into method, URI, and HTTP version
    startline_split = ft_split(_raw_start_line, ' ');
    if (startline_split.size() != 3) 
    {
        throw BadRequestException();
    }

    // Parse HTTP method
    if (startline_split[0] == "GET")
        _request_method = GET;
    else if (startline_split[0] == "POST")
        _request_method = POST;
    else if (startline_split[0] == "DELETE")
        _request_method = DELETE;
    else
        throw BadRequestException();

    // Enforce limit_except checks
    if ((_request_method == GET && !sb->_allowGET) ||
        (_request_method == POST && !sb->_allowPOST) ||
        (_request_method == DELETE && !sb->_allowDELETE)) 
    {
        throw UnauthorizedException();
    }

    // Parse and validate the URI
    decomposeURI(startline_split[1]);

    str path = remove_slashes(_uri.getPath());

    LocationBlock* loc = locationExistsInBlock(sb->getRoutes(), path);
      
    if(loc)
    {
        if (loc->hasRedirect())  // Patrick
        {
            std::cout << "Redirect found! Status: " << loc->getRedirectStatus()
                  << ", Target: " << loc->getRedirectTarget() << std::endl;
           _redirect_required = true;
           _redirect_status = loc->getRedirectStatus();
           _redirect_target = loc->getRedirectTarget();
            return;
        }
    }

    if (is_folder_in_tree(sb->getDocumentRoot(), path))
    {
        if (path == "images")
            throw RedirectException(); // i might have to remove this...

        LocationBlock* loc = locationExistsInBlock(sb->getRoutes(), path);
        if (!loc)
            throw UnauthorizedException();
        
      std::string indexPath = loc->getDocumentRoot() + "/" + path + "/" + loc->getDefaultIndex();

     if (ft_is_avail(indexPath))
      {
        _resource = indexPath;
      } 
       
        else if (loc->isDirectoryListingEnabled())
        {
           _resource = loc->getDocumentRoot() + "/" + path;
           _directoryListingEnabled = true;
        } 
         
    //    else 
    //    {
    //         throw NotFoundException();
    //    }
        
    }

    else if (startline_split[1].size() > 1 && startline_split[1].substr(0,2) == "//" ) {
        throw NotFoundException();
    }
    
    else if (startline_split[1] == "/") {
        _resource = sb->getDocumentRoot() + _uri.getPath() + sb->getDefaultIndex();
    }
    else if (hasFileExtension(_uri.getPath(), ".php")) {
        _resource = _uri.getPath();
    }
    else {
        _resource = sb->getDocumentRoot() + _uri.getPath();
    }
  
    
    // if (!hasFileExtension(_uri.getPath(), ".php") && !_directoryListingEnabled && !ft_is_avail(_resource))
    //      throw NotFoundException();

    std::cout << "" << _resource << " IS ME AND I AM TESTING" << std::endl;

     if (!ft_is_avail(_resource) && !hasFileExtension(_uri.getPath(), ".php"))
         throw NotFoundException();

     std::cout << "" << _resource << " is available" << std::endl;

    // Validate HTTP version
    if (startline_split[2] != "HTTP/1.1")
        throw HttpVersionNotSupportedException();
}


void ClientRequest::decomposeURI(str uri) 
{
    std::size_t query_pos = uri.find('?'); 
    std::size_t hash_pos = uri.find('#'); // to store fragment...

    str path;

    // Extract path: if there's a '?', path is everything before it
    if (query_pos != str::npos) 
    {
        path = uri.substr(0, query_pos);
    } 
    else 
    {
        path = uri;
    }
    _uri.setPath(path);

    // Extract query string if a '?' is present
    if (query_pos != str::npos) {
        str query; 

        if (hash_pos != str::npos && hash_pos > query_pos) {
            query = uri.substr(query_pos + 1, hash_pos - query_pos - 1);
        } else {
            query = uri.substr(query_pos + 1);
        }

        _uri.setQuery(query);
    }

    // // Throw 404 error
    // if (!ft_is_avail(_uri.getPath()) &&
    //     !hasFileExtension(_uri.getPath(), ".php")) {
    //     throw NotFoundException();
    // }
}



const char * ClientRequest::RedirectException::what() const throw ()
{
    return ("The requested resource has been temporarily moved to a different URI");
}
const char* ClientRequest::BadRequestException::what() const throw() {
    return ("The server could not understand the request due to invalid syntax.");
}

const char* ClientRequest::NotFoundException::what() const throw() {
    return ("The requested resource could not be found on this server.");
}

const char* ClientRequest::HttpVersionNotSupportedException::what() const throw() {
    return ("Only HTTP/1.1 requests are accepted.");
}

const char* ClientRequest::UnauthorizedException::what() const throw() {
    return ("Authentication is required to access this resource.");
}

void ClientRequest::extractHeadersFromRequest() {
    std::string line;

    std::size_t header_end = _unparsed_request.find("\r\n\r\n");
    _raw_headers = _unparsed_request.substr(0, header_end);
    std::stringstream header_stream(_raw_headers);

    while (std::getline(header_stream, line, '\r'))
    {
        // Remove the trailing '\n' e.g-> "\nContent-Type: text/plain"
        if (!line.empty() && line[0] == '\n')
            line.erase(0, 1);

        str key, value;
        std::size_t colon_pos = line.find(':');
        if (colon_pos == std::string::npos)
            continue; // malformed header or first_line, skip it!

        key = line.substr(0, colon_pos);
        value = line.substr(colon_pos + 1);

        // Trim key and value
        key.erase(key.begin(), std::find_if(key.begin(), key.end(), isNotWhitespace));
        key.erase(std::find_if(key.rbegin(), key.rend(), isNotWhitespace).base(), key.end());

        value.erase(value.begin(), std::find_if(value.begin(), value.end(), isNotWhitespace));
        value.erase(std::find_if(value.rbegin(), value.rend(), isNotWhitespace).base(), value.end());

        _headers.insert(std::make_pair(key, value));
    }

    
    std::map<str, str>::iterator it = _headers.find("Content-Length");
    if (it != _headers.end()) 
    {
        _has_body = true;
        _body_length = std::strtoul(it->second.c_str(), NULL, 10);
    }


std::map<str, str>::iterator cookie_it = _headers.find("Cookie");
if (cookie_it != _headers.end()) {
    std::stringstream cookie_stream(cookie_it->second);
    std::string cookie_pair;

    while (std::getline(cookie_stream, cookie_pair, ';')) {
        std::size_t equal_pos = cookie_pair.find('=');
        if (equal_pos != std::string::npos) {
            std::string key = cookie_pair.substr(0, equal_pos);
            std::string value = cookie_pair.substr(equal_pos + 1);

            // Trim whitespace
            key.erase(key.begin(), std::find_if(key.begin(), key.end(), isNotWhitespace));
            key.erase(std::find_if(key.rbegin(), key.rend(), isNotWhitespace).base(), key.end());

            value.erase(value.begin(), std::find_if(value.begin(), value.end(), isNotWhitespace));
            value.erase(std::find_if(value.rbegin(), value.rend(), isNotWhitespace).base(), value.end());

            _cookies[key] = value;
        }
    }
}

}


void ClientRequest::debugPrintRequest(void) 
{
    std::cout << "\n--- REQUEST PARSED ---\n";
    std::cout << "Start Line:\n" << _raw_start_line << "\n\n";

    std::cout << "Parsed Headers:\n";
    for (std::map<str, str>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
        std::cout << it->first << ": " << it->second << '\n';

    if (_has_body) {
        std::cout << "\nBody Length: " << _body_length << '\n';
        std::cout << "Body Content:\n" << _raw_body << '\n';
    }

    if (!_cookies.empty()) {
    std::cout << "\nParsed Cookies:\n";
    for (std::map<std::string, std::string>::const_iterator it = _cookies.begin(); it != _cookies.end(); ++it)
        std::cout << it->first << " = " << it->second << '\n';
}

    std::cout << "--- END OF REQUEST ---\n\n";
}


void	ClientRequest::setStatusCode(StatusCode status_code)
{
	_status_code = status_code;
}


Http_Method   ClientRequest::get_Http_Method(void) 
{
    return (_request_method);
}

str ClientRequest::getRawHttpRequest() {
    return _unparsed_request;
}

str ClientRequest::getRequestLine() {
    return _raw_start_line;
}

str ClientRequest::getHeaderSection() {
    return _raw_headers;
}

const char * ClientRequest::PayloadTooLargeException::what() const throw () {
	return ("ClientRequest too long!! ");
}

str ClientRequest::getBodyContent() {
    return _raw_body;
}


UR_Locator& ClientRequest::getUriObject() {
    return _uri;
}

std::map<std::string, std::string> ClientRequest::getParsedHeaders() {
    return _headers;
}

StatusCode ClientRequest::getStatusCode() {
    return _status_code;
}

std::string ClientRequest::getResolvedResourcePath() {
    return _resource;
}


bool ClientRequest::isRedirectRequired() const{
    return  _redirect_required;
}

int ClientRequest::getRedirectStatus() const{

    return _redirect_status;
}
std::string ClientRequest::getRedirectTarget() const{
  return (_redirect_target);
}


void ClientRequest::setMatchedLocationBlock(LocationBlock* loc) {
    _matchedLocation = loc;
}

LocationBlock* ClientRequest::getMatchedLocationBlock() const {
    return (_matchedLocation);
}

UR_Locator& ClientRequest::getUriClient(){
    return (_uri);  

}

void ClientRequest::setMyServerBlock(ServerConfiguration* server){
  crook = server;
}

ServerConfiguration* ClientRequest::getMyServerBlock() const{

     return (crook);
}

std::string ClientRequest::getCookieValue(const std::string& key) const {
    std::map<std::string, std::string>::const_iterator it = _cookies.find(key);
    if (it != _cookies.end())
        return it->second;
    return "";
}

std::map<std::string, std::string> ClientRequest::getCookies() const {
    return _cookies;
}
