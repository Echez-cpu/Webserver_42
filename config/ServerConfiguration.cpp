#include "../includes/ServerConfiguration.hpp"
#include <iostream>

ServerConfiguration::ServerConfiguration()
    : _maxClientBodySize(100000),
      _port("8080"),
      _documentRoot("public/www"),
      _defaultIndex("index.html"),
      _hostname("localhost"),
      _allowGET(true),
      _allowPOST(true),
      _allowDELETE(true) {}

ServerConfiguration::ServerConfiguration(const ServerConfiguration& source)
{
    *this = source;
}

ServerConfiguration::~ServerConfiguration()
{
    for (size_t i = 0; i < _routes.size(); ++i) {
        delete _routes[i];
    }
     _routes.clear();

  std::cout << " ServerConfig doing it's job" << std::endl;
}

ServerConfiguration& ServerConfiguration::operator=(const ServerConfiguration& source)
{
    if (this != &source) {
        _maxClientBodySize = source._maxClientBodySize;
        _port = source._port;
        _documentRoot = source._documentRoot;
        _defaultIndex = source._defaultIndex;
        _hostname = source._hostname;

        _errorPagePaths = source._errorPagePaths;
        _routes = source._routes; // Note: Shallow copy; assumes external route ownership

        _allowGET = source._allowGET;
        _allowPOST = source._allowPOST;
        _allowDELETE = source._allowDELETE;
    }
    return *this;
}

// =====================
// Getter Implementations
// =====================

unsigned long ServerConfiguration::getMaxClientBodySize() const {
    return _maxClientBodySize;
}

std::string ServerConfiguration::getPort() const {
    return _port;
}

std::string ServerConfiguration::getDocumentRoot() const {
    return _documentRoot;
}

std::string ServerConfiguration::getDefaultIndex() const {
    return _defaultIndex;
}

std::string ServerConfiguration::getHostname() const {
    return _hostname;
}

std::vector<LocationBlock*>& ServerConfiguration::getRoutes() {
    return _routes;
}

// =====================
// Setter Implementations
// =====================

void ServerConfiguration::setMaxClientBodySize(unsigned long size) {
    _maxClientBodySize = size;
}

void ServerConfiguration::setPort(const std::string& port) {
    _port = port;
}

void ServerConfiguration::setHostname(const std::string& name) {
    _hostname = name;
}

void ServerConfiguration::setDefaultIndex(const std::string& indexFile) {
    _defaultIndex = indexFile;
}

void ServerConfiguration::setDocumentRoot(const std::string& rootPath) {
    _documentRoot = rootPath;
}

void ServerConfiguration::setAllowedMethods(const std::vector<std::string>& methods) {
    _allowGET = false;
    _allowPOST = false;
    _allowDELETE = false;

    for (std::vector<std::string>::const_iterator it = methods.begin(); it != methods.end(); ++it) {
        if (*it == "GET") _allowGET = true;
        else if (*it == "POST") _allowPOST = true;
        else if (*it == "DELETE") _allowDELETE = true;
    }
}


// utility function to convert string to HttpResponseStatus
StatusCode stringToStatusCode(const std::string& statusCodeString) {
    if (statusCodeString == "200") return OK;
    if (statusCodeString == "201") return CREATED;
    if (statusCodeString == "204") return NO_CONTENT;
    if (statusCodeString == "302") return FOUND;
    if (statusCodeString == "400") return BAD_REQUEST;
    if (statusCodeString == "401") return UNAUTHORIZED;
    if (statusCodeString == "403") return FORBIDDEN;
    if (statusCodeString == "404") return NOT_FOUND;
    if (statusCodeString == "411") return LENGTH_REQUIRED;
    if (statusCodeString == "413") return PAYLOAD_TOO_LARGE;
    if (statusCodeString == "415") return UNSUPPORTED_MEDIA_TYPE;
    if (statusCodeString == "500") return INTERNAL_SERVER_ERROR;
    if (statusCodeString == "505") return HTTP_VERSION_NOT_SUPPORTED;

    throw std::invalid_argument("Invalid status code string: " + statusCodeString);
}

void ServerConfiguration::setErrorPage(const std::string& statusCodeString, const std::string& filePath) {
    StatusCode code = stringToStatusCode(statusCodeString);
    _errorPagePaths[code] = filePath;
}

void ServerConfiguration::addRoute(LocationBlock* route) {
    _routes.push_back(route);
}

// =====================
// Utility Methods
// =====================

std::string ServerConfiguration::fetchErrorPagePath(StatusCode statusCode) {
    // std::map<StatusCode, std::string>::const_iterator it = _errorPagePaths.find(statusCode);
    // return (it != _errorPagePaths.end()) ? it->second : std::string();
std::string filePath;
    std::map<StatusCode, std::string>::iterator it = _errorPagePaths.find(statusCode);

    // returns the string of the file location OR empty if not found
    if (it != _errorPagePaths.end())
        return (it->second);
    else
        return std::string();

}

void ServerConfiguration::printRoutes() const {
    for (std::vector<LocationBlock*>::const_iterator it = _routes.begin(); it != _routes.end(); ++it) {
        std::cout << "  ROUTE CONFIGURATION:\n";
        std::cout << "    Path:               " << (*it)->getPathUri() << "\n";
        std::cout << "    Root Directory:     " << (*it)->getDocumentRoot() << "\n";
        std::cout << "    Default Index:      " << (*it)->getDefaultIndex() << "\n";
        std::cout << "    Max Body Size:      " << (*it)->getMaxClientBodySize() << "\n";
        std::cout << "    Allow GET:          " << std::boolalpha << (*it)->isGetAllowed()<< "\n";
        std::cout << "    Allow POST:         " << std::boolalpha << (*it)->isPostAllowed() << "\n";
        std::cout << "    Allow DELETE:       " << std::boolalpha << (*it)->isDeleteAllowed() << "\n";

        std::cout << "    Direction Status:   " << (*it)->getRedirectStatus() << "\n";
        std::cout << "    Redirection target: " << (*it)->getRedirectTarget() << "\n";
        std::cout << "    Has Redirection?:   " << std::boolalpha << (*it)->hasRedirect()<< "\n";
    }
}
